#ifndef COMPILER_STRING_DEF_H
#define COMPILER_STRING_DEF_H

// Weed warnings
#define NO_EXE_AFTER_RETURN "Code will never be executed after a return"
#define IF_TRUE             "If else statement is always true"
#define IF_FALSE            "If else statement is always false"
#define EXP_MERGE           "Expression can be shorted by merging constants"

// Weed errors
#define MISMATCH_HEAD_TAIL_ID       "Mismatching head and end id"
#define NO_RETURN                   "Function at line has no return"
#define NO_BREAK_CONT_OUTSIDE_LOOP  "Break and continue keywords are not \
                                     allowed outside of a loop"


// Type collect errors
#define FUNC_DEF_FAIL   "Function could not be defined"
#define FUNC_REDEF      "Redefinition of function"
#define VAR_REDEF       "Redefinition of variable"
#define RETURN_NO_FUNC  "Return statement outside function"

// Type calc errors
#define TYPE_RECUR_DEF  "Type is defined recursively and will never end"
#define TYPE_NOT_IN_ST  "Type was not in symbol table"
#define TYPE_NOT_FOUND  "Could not find type"
#define CLASS_NOT_DEF   "Class is not defined"

// Type check errors
#define WRONG_RETURN_TYPE       "Wrong return type"
#define ONLY_INT_BOOL_WRITE     "Only ints and bools are allowed to be written"
#define ONLY_RECORD_ALLOC       "Only records can be allocated"
#define ONLY_ARR_ALLOC          "Only arrays can be allocated"
#define MEM_MUST_BE_INT         "New specified amount of memory should \
                                 be an int"
#define ONLY_ARR_RECORD_NULL    "Only arrays and records can be set to null"
#define VAR_EXP_NOT_SAME_TYPE   "Variable and expression does not have \
                                 the same type"
#define EXP_OF_STM_NOT_BOOL     "Expression of if statement is not a boolean"
#define SYM_NOT_DEFINED         "Symbol is not defined"
#define INDEX_NOT_INT           "The specified index is not of type int"
#define VAR_NOT_ARR             "The variable is not an array"
#define VAR_NOT_RECORD          "The variable is not a record"
#define ID_NOT_FUNC             "specified id does not match any function"
#define FEW_ARG_FUNC_CALL       "Too few parameter in function call"
#define MANY_ARG_FUNC_CALL      "Too many parameter in function call"
#define FUNC_CALL_MISMATCH_TYPE "Function call does not match the \
                                 parameter types correctly"
#define MISMATCH_ARG_NUM        "To many or to few parameter passed"
#define ONLY_NOT_BOOL           "Not operator can only be used on bools"
#define ONLY_PIPE_INT_ARR       "The pipe operator can only be used on \
                                 ints and arrays"
#define EXP_NOT_VALID           "The expression is not valid"
#define EXPS_NOT_INT            "Expression(s) are not of type int"
#define EXP_NOT_BOOL            "Expression is not a bool"

#define IO_ONLY_INT_BOOL        "Only ints and bools can take input"

// Runtime errors
#define ARR_OUT_OF_BOUNDS     "array_out_of_bounds:"
#define DIV_BY_0              "div_by_0:"
#define POSI_ARG_ARR_ALLOC    "positive_arg_arr_alloc:"
#define USE_OF_UNINIT_OR_NULL "use_of_uninit_or_null:"
#define OUT_OF_MEM            "out_of_memory:"

// AAC Gen
#define PRINT_INT   "PRINT_INT"
#define PRINT_TRUE  "PRINT_TRUE"
#define PRINT_FALSE "PRINT_FALSE"

#endif
