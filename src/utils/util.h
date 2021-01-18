#ifndef COMPILER_UTIL_H
#define COMPILER_UTIL_H

typedef enum { false, true } bool;

bool gb_collect;
bool runtime_check;
bool strong_inline;
int error_flag;
bool w_all;
bool w;

void print_warning(char *msg, int line_no);
void print_error(char *msg, int line_no, int flag);

#endif
