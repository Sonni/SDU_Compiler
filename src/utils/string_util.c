#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "string_def.h"
#include "string_util.h"
#include "definitions.h"

char *cpy_str(char *str)
{
    char *new_str = malloc(sizeof(char) * strlen(str) + 1);
    assert(new_str != NULL);
    strcpy(new_str, str);
    return new_str;
}

char *add_str(char *str1, char *str2)
{
    size_t len = strlen(str1) + strlen(str2);
    char *new_str = (char*)malloc(len * sizeof(char) + 1);
    *new_str = '\0';
    return strcat(strcat(new_str, str1), str2);
}

char *str_concat(char *str1, char *str2)
{
    char *string = malloc(sizeof(char) * 10000);
    sprintf(string, "%s%s", str1, str2);
    return cpy_str(string);
}

void stderr_print(char *string)
{
    if (RELEASE)
    {
        fprintf(stderr, "%s...\n", string);
    }
}

char *str_replace(char *str, char *delimiter, char *replace_with)
{
    char *sharp = strstr(str, delimiter);
    char *dest = malloc(sizeof(char) * 10000);
    char *suffix = malloc(sizeof(char) * 10000);
    strncpy(dest, str, sharp - str);
    strncpy(suffix, sharp+1, (int) (strlen(str)));

    dest = str_concat(dest, replace_with);
    dest = str_concat(dest, suffix);

    return dest;
}
