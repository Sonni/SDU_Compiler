%{
#include <string.h>
#include "y.tab.h"
#include "../utils/util.h"
#include "../utils/definitions.h"


int comment_lines = 0;

extern int fileno(FILE *out);
extern unsigned int line_num;
%}

%option noyywrap nounput noinput warn
%x COMMENT MULTI_LINE_COMMENT STRING

TOKENS [;:+-*/<>|]

%%
[ \t]+        /* ignore */;
\n              line_num++;

"template"      return T_TEMPLATE;
"func"          return T_FUNC;
"end"           return T_END;
"int"           return T_INT;
"float"         return T_FLOAT;
"bool"          return T_BOOL;
":"             return T_COLON;
"type"          return T_TYPE;
"="             return T_EQUAL;
"=="            return T_DEQUAL;
";"             return T_SEMICOLON;
"false"         return T_FALSE;
"true"          return T_TRUE;
"null"          return T_NULL;
"return"        return T_RETURN;
"write"         return T_WRITE;
"allocate"      return T_ALLOCATE;
"of length"     return T_OF_LENGTH;
"if"            return T_IF;
"then"          return T_THEN;
"else"          return T_ELSE;
"while"         return T_WHILE;
"do"            return T_DO;
"var"           return T_VAR;
"array of"      return T_ARRAY_OF;
"record of"     return T_RECORD_OF;
"{"             return T_OPEN_CURLY_B;
"}"             return T_CLOSE_CURLY_B;
","             return T_COMMA;
"."             return T_DOT;
"("             return T_OPEN_P;
")"             return T_CLOSE_P;
"["             return T_OPEN_B;
"]"             return T_CLOSE_B;
"!="            return T_NEQ;
"<-"            return T_LESS_MINUS;
">"             return T_GREATER;
"<"             return T_LESS;
"|~"             return T_TILDE_LEFT;
"~|"             return T_TILDE_RIGHT;
">="            return T_GEQ;
"<="            return T_LEQ;
"&&"            return T_AND;
"!"             return T_NOT;
"|"             return T_PIPE;
"+="            return T_PLUS_E;
"-="            return T_MINUS_E;
"/="            return T_DIV_E;
"*="            return T_TIMES_E;
"++"            return T_INC;
"--"            return T_DEC;
"break"         return T_BREAK;
"continue"      return T_CONTINUE;
"for"           return T_FOR;
"to"            return T_TO;
"class"         return T_CLASS;
"->"            return T_R_ARROW;
"inline"        return T_INLINE;
"!inline"       return T_INLINE_NOT;
"<<"            return T_D_L_ARROW;
"in"            return T_IN;
"?"             return T_QUESTION;
"enum"          return T_ENUM;
"string"        return T_STRING;
"char"          return T_CHAR;
"const"         return T_CONST;
"extend"        return T_EXTEND;
"free"          return T_FREE;
"public"        return T_PUBLIC;
"private"       return T_PRIVATE;
"switch"        return T_SWITCH;
"case"          return T_CASE;

"*"             return T_TIMES;
"/"             return T_DIV;
"+"             return T_PLUS;
"-"             return T_MINUS;





0|([1-9][0-9]*)        { yylval.intconst = atoi(yytext);
                       return T_INT_CONST; }

[0-9]+(\.[0-9][0-9]?)?        {
yylval.floatconst = atof(yytext);
                       return T_FLOAT; }


[a-zA-Z_][a-zA-Z0-9_]* { yylval.stringconst = (char *)malloc(strlen(yytext)+1);
                         sprintf(yylval.stringconst,"%s",yytext);
                         return T_ID; }


"\"" {BEGIN(STRING);};

<STRING>{

    "\"" { BEGIN(INITIAL); }
    [^"]*    { yylval.stringconst = (char *)malloc(strlen(yytext)+1);
                                    sprintf(yylval.stringconst,"%s",yytext);
                                    return T_SENTENCE; }
    <<EOF>> {
            if (RELEASE)
                printf("error");
            else
                printf("Comment was not closed\n");
            exit(1);
        return 0;
    }
}

"(*" { BEGIN(MULTI_LINE_COMMENT); comment_lines = 1; }

<MULTI_LINE_COMMENT>{

    "(*" { comment_lines++; }
    "*)" { comment_lines--; if (comment_lines == 0) {BEGIN(INITIAL);} }
    \n   { }
    .    { }
    <<EOF>> {
        if (comment_lines > 0)
        {
            if (RELEASE)
                printf("error");
            else
                printf("Comment was not closed\n");
            exit(1);
        }
        return 0;
    }
}


"#"           { BEGIN(COMMENT); }
<COMMENT>\n   { BEGIN(INITIAL); }
<COMMENT>.    { }



. {
    if (RELEASE)
        printf("error");
    else
        printf("Invalid token at line %i\n", line_num);
    exit(1);
}
<<EOF>> {
    yyterminate();
}
%%
