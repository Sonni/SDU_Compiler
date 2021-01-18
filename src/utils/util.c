#include <stdio.h>
#include "util.h"
#include "definitions.h"

void print_warning(char *msg, int line_no)
{
    if (ENABLE_WARNINGS)
    {
        fprintf(stderr, YEL "Warning ");
        fprintf(stderr, NORM "line %i: %s\n", line_no, msg);
    }
}

void print_error(char *msg, int line_no, int flag)
{
    error_flag = flag;
    if (PRINT_ERRORS)
    {
        fprintf(stderr, RED "Error ");
        fprintf(stderr, NORM "line %i: %s\n", line_no, msg);
    }
}
