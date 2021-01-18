%{
#include "../utils/util.h"
extern char *txt_buf;
extern unsigned int line_num;
extern int fileno(FILE *out);

char *defs[100];
char *macros[100];
int def_counter;
char *cur_txt;
int def_pointer;

#define MAX_INCLUDE_DEPTH 10
YY_BUFFER_STATE includes[MAX_INCLUDE_DEPTH];
int include_ptr = 0;

%}

%option noyywrap noinput prefix = "pp"
%x def incl

%%
\n  { strcat(txt_buf, "\n"); }
" " { strcat(txt_buf, " "); }

"define" {
    def_counter++;
    def_pointer = 1;
    BEGIN(def);
}

include { BEGIN(incl);}

[^ \n]* {

    int has_changed = 0;
    for (int i = 1; i < def_counter + 1; i++)
    {
        if (strcmp(pptext, defs[i]) == 0)
        {
            int z;
            for (z = strlen(macros[i]) - 1; z >= 0; z--)
            {
                unput(macros[i][z]);
            }
            has_changed = 1;
        }
    }
    if (has_changed == 0)
    {
        strcat(txt_buf, pptext);
    }
}

<incl>{
    [ \t]*      // Remove white space *
    [^ \t\n]+ {
        if (include_ptr >= MAX_INCLUDE_DEPTH)
        {
            print_error("To deep inclusion", line_num, 1);
            exit(1);
        }
        includes[include_ptr++] = YY_CURRENT_BUFFER;
        yyin = fopen(yytext, "r");

        if (!yyin)
        {
            printf("AA %s\n", yytext);
            print_error("Could not import file\n", line_num, 1);
            exit(1);
        }

        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));

        BEGIN(INITIAL);
    }
}

<def>{
    [a-zA-Z0-9_]* {

        cur_txt = (char *)malloc(strlen(pptext)+1);
        sprintf(cur_txt,"%s",pptext);

        if (def_pointer == 1)
        {
            defs[def_counter] = cur_txt;
        }
        else if (def_pointer == 2)
        {
            macros[def_counter] = cur_txt;
        }
        else if (def_pointer > 2)
        {
            def_pointer = 0; BEGIN(INITIAL);
        }

        def_pointer++;
    }

    "\n"    {
        if (def_pointer < 2)
        {
            printf("Incomplete define statement\n");
            exit(1);
        }

        def_pointer = 0;
        BEGIN(INITIAL);
    }
    <<EOF>> { def_pointer = 0; BEGIN(INITIAL); }
}

<<EOF>> {
    if ( --include_ptr < 0 )
    {
        yyterminate();
    }

    else
    {
        yy_delete_buffer(YY_CURRENT_BUFFER);
        yy_switch_to_buffer(includes[include_ptr]);
    }
}

%%