#ifndef COMPILER_STRING_UTIL_H
#define COMPILER_STRING_UTIL_H

#include "util.h"

char *cpy_str(char *t);
char *add_str(char *str1, char *str2);
char *str_concat(char *str1, char *str2);
void stderr_print(char *string);
char *str_replace(char *str, char *delimiter, char *replace_with);

#endif
