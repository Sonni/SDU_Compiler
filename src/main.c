#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "frontend/parser/pretty.h"
#include "frontend/weeder/first_weed.h"
#include "frontend/weeder/second_weed.h"
#include "frontend/typecheck/type_collect.h"
#include "frontend/typecheck/type_calc.h"
#include "frontend/typecheck/type_check.h"
#include "backend/asm/asm_gen.h"
#include "backend/asm/asm_printer.h"
#include "backend/reg_alloc/liveness.h"
#include "backend/reg_alloc/reg_alloc.h"
#include "backend/reg_alloc/rewrite.h"
#include "backend/optimization/peephole.h"
#include "backend/optimization/inliner/func_inliner.h"
#include "utils/util.h"
#include "utils/vector.h"
#include "utils/string_util.h"
#include "utils/definitions.h"
#include "utils/memory/free_ast.h"
#include "utils/memory/free_aac.h"
#include "frontend/template/gen_template.h"
#include "frontend/template/ast_reset.h"
#include "frontend/template/obj_op_replace.h"
#include "frontend/template/gen_class.h"
#include "frontend/template/basic_class_op_rewrite.h"

unsigned int line_num;
void yyparse();
void yy_scan_string();
int pplex();
body *program;
char *txt_buf;
extern int num_tmp;

void create_enums(body *program)
{
    if (vector_size(enum_ids) != 0)
    {
        statement_list *start = make_sl_statement_list(NULL, NULL);
        statement_list *new_sl = start;
        for (int i = 0; i < (int) vector_size(enum_ids) - 1; i++)
        {
            new_sl->sl = make_sl_statement_list(NULL, NULL);
            new_sl = new_sl->sl;
        }
        new_sl->kind = SL_STM;

        new_sl = start;
        int counter = vector_size(enum_ids) - 1;
        while (vector_size(enum_ids) != 0)
        {

            char *id = vector_pop(enum_ids);
            statement *stm =
                    make_statement_assignment(make_var_id(id),
                                              make_exp_term(
                                                      make_term_int(counter)));
            new_sl->stm = stm;
            new_sl = new_sl->sl;

            counter--;
        }

        new_sl = start;
        while (new_sl->sl != NULL)
        {
            new_sl = new_sl->sl;
        }

        statement_list *tmp = program->sl;
        while (tmp->sl != NULL)
        {
            tmp = tmp->sl;
        }
        tmp->sl = start;
        tmp->kind = SL_STM_SL;
    }
}

int do_type_checking(body *b)
{
    if (error_flag == 0 || (SEARCH_FOR_ERR && COLLECT))
    {
        stderr_print("    - Collect phase");
        collect(b);
        if (error_flag != 0)
            return error_flag;
    }

    if (error_flag == 0 || (SEARCH_FOR_ERR && CALC))
    {
        stderr_print("    - Calculating phase");
        calc(b);
        if (error_flag != 0)
            return error_flag;
    }


    //return 0;
    if (error_flag == 0 || (SEARCH_FOR_ERR && CHECK))
    {
        stderr_print("    - Checking phase");
        check(b);
        if (error_flag != 0)
            return error_flag;
    }

    return 0;
}

void print_help()
{
    fprintf(stderr, "-h           : print help\n");
    fprintf(stderr, "-x           : test for speed\n");
    fprintf(stderr, "-w           : stop weeding\n");
    fprintf(stderr, "-i !         : strong inline / don't inline\n");
  fprintf(stderr, "-p types     : \n");
  fprintf(stderr, "-P           : Stop and print code after pre-processing\n");
    fprintf(stderr, "-Wall        : print all warnings\n");
    fprintf(stderr, "-r           : disable runtime check\n");
    fprintf(stderr, "-g           : enable garbage collection\n");
    fprintf(stderr, "-d  n/ln/du  : enable debug code normal / live in, out / "
                    "def, use\n");

}

extern bool print_errors;

int main(int argc, char *argv[])
{
    line_num = 1;
    error_flag = 0;
    int oc;
    char *arg;

    print_liveness = print_use_def = print_debug = false;
    gb_collect = false;
    runtime_check = true;
    strong_inline = false;
    bool do_inlining = false;
    bool do_pretty = false, end_after_pp = false;
    w = w_all = false;
    WEED = true;
    PRINT_TYPES = false;
    abstract_asm *aac;
    num_tmp = NUM_REG;

    /*
     * -help        : print help
     * -x           : test for speed
     * -w           : stop weeding
     * -i !         : strong inline / don't inline
     * -p types     : pretty print and exit
     * -P           : Stop and print code after pre-processing
     * -Wall        : print all warnings
     * -r           : disable runtime check
     * -g           : enable garbage collection
     * -d n/ln/du   :
     */
    while ((oc = getopt(argc, argv, "h:xp:wPi:W:d:pg")) != -1)
    {
        switch (oc)
        {
            case 'h':
                print_help();
                return 0;
            case 'x':
                runtime_check = print_debug = false;
                strong_inline = do_inlining = true;

                break;
            case 'w':
                WEED = false;
                break;
            case 'i':
                arg = optarg;
                strong_inline = true;
                if (strcmp(arg, "!") == 0)
                {
                    do_inlining = false;
                }
                break;
            case 'p':
                do_pretty = true;
                arg = optarg;

                if (strcmp(arg, "types") == 0)
                {
                    PRINT_TYPES = true;
                }
                break;
            case 'W':
                arg = optarg;
                if (strcmp(arg, "all") == 0)
                {
                    w_all = true;
                }
                break;
            case 'P':
                end_after_pp = true;
                break;
            case 'r':
                runtime_check = false;
                break;
            case 'g':
                gb_collect = true;
                break;
            case 'd':
                arg = optarg;
                print_debug = true;
                if (strcmp(arg, "ln") == 0)
                {
                    print_liveness = true;
                }
                else if (strcmp(arg, "du") == 0)
                {
                    print_use_def = true;
                }
                break;
        }
    }
    print_errors = true;
    enum_ids = vector_init();
    txt_buf = (char *)malloc(sizeof(char) * 50000); // TODO make dynamic

    stderr_print("- Preprocessing");
    pplex();                            // Pre-processing

    if (end_after_pp)
    {
        printf("%s", txt_buf);
        free(txt_buf);
        return 0;
    }

    stderr_print("- Scanning and parsing");
    yy_scan_string(txt_buf);            // Load pre-processed text into buffer
    yyparse();                          // Parse
    free(txt_buf);


    create_enums(program);
    stderr_print("- First weeding phase");
    first_weed(program);

    if (error_flag != 0)
    {
        return error_flag;
    }


    print_errors = false;
    if (error_flag == 0 || (SEARCH_FOR_ERR && TYPECHECK))
    {
        stderr_print("- Type checking");
        if (do_type_checking(program) != 0)
            return error_flag;
    }
    print_errors = true;





    stderr_print("- Generate class phase");
    gen_class(program);
    if (error_flag != 0) return error_flag;


    stderr_print("- Generate template phase");
    gen_template();



    // pretty(program);
    //return 0;

    reset(program);
    print_errors = false;
    if (do_type_checking(program) != 0)
        return error_flag;
    print_errors = true;







    stderr_print("- Deleting templates phase");
    delete_templates(program);
    //pretty(program);
    //return 0;




    reset(program);
    print_errors = false;
    if (do_type_checking(program) != 0)
        return error_flag;
    print_errors = true;




    stderr_print("- Replacing basic class operations");
    obj_op_replace_body(program);
    basic_class_op_rewrite(program);






    print_errors = true;
    reset(program);
    if (do_type_checking(program) != 0)
        return error_flag;

   // pretty(program);
   // return 0;



    if (WEED)
    {
        stderr_print("- Second weeding phase");
        second_weed(program);
    }

    if (do_inlining)
    {
        stderr_print("- Inlining functions");
        inliner_inline(program);
    }

    if (do_pretty)
    {
        pretty(program);
        return 0;
    }

    error_flag = 0;
    if (error_flag == 0)
    {
        stderr_print("- Generating AAC");
        aac = asm_gen(program);

        stderr_print("- First peep-hole optimization");
        peephole(aac, true);

        stderr_print("- Register allocation");
        int *color = reg_alloc(aac);

        stderr_print("- Rewriting program");
        aac = rewrite_program(aac, color);

        stderr_print("- Second peep-hole optimization");
        peephole(aac, false);

        stderr_print("- Printing final assembly code");
        print_abstract_asm(aac);
    }

    if (RELEASE && error_flag == 1)
    {
        fprintf(stderr, "error\n");
    }

    return error_flag;
}
