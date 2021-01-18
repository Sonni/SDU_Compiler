/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "../frontend/parser/shere_khan.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include "../frontend/parser/tree.h"
#include "../utils/definitions.h"

extern char *yytext;
extern body *program;
extern int line_num;

int yylex();
void yyerror(char const *err)
{
   fprintf(stderr, RED "%s: ", err);
   fprintf(stderr, NORM "before %s at line %d \n", yytext, line_num);
   exit(1);
}

#line 85 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_INT_CONST = 258,
    T_FLOAT = 259,
    T_ID = 260,
    T_SENTENCE = 261,
    T_FUNC = 262,
    T_END = 263,
    T_INT = 264,
    T_BOOL = 265,
    T_TRUE = 266,
    T_FALSE = 267,
    T_NULL = 268,
    T_TYPE = 269,
    T_EQUAL = 270,
    T_RETURN = 271,
    T_WRITE = 272,
    T_ALLOCATE = 273,
    T_OF_LENGTH = 274,
    T_IF = 275,
    T_THEN = 276,
    T_ELSE = 277,
    T_WHILE = 278,
    T_DO = 279,
    T_ARRAY_OF = 280,
    T_RECORD_OF = 281,
    T_VAR = 282,
    T_COLON = 283,
    T_SEMICOLON = 284,
    T_COMMA = 285,
    T_DOT = 286,
    T_OPEN_CURLY_B = 287,
    T_CLOSE_CURLY_B = 288,
    T_OPEN_B = 289,
    T_CLOSE_B = 290,
    T_OPEN_P = 291,
    T_CLOSE_P = 292,
    T_EQ = 293,
    T_NEQ = 294,
    T_GREATER = 295,
    T_LESS = 296,
    T_TILDE_LEFT = 297,
    T_TILDE_RIGHT = 298,
    T_GEQ = 299,
    T_LEQ = 300,
    T_AND = 301,
    T_DEQUAL = 302,
    T_NOT = 303,
    T_PIPE = 304,
    T_PLUS_E = 305,
    T_MINUS_E = 306,
    T_TIMES_E = 307,
    T_DIV_E = 308,
    T_INC = 309,
    T_DEC = 310,
    T_PLUS = 311,
    T_MINUS = 312,
    T_DIV = 313,
    T_TIMES = 314,
    T_BREAK = 315,
    T_CONTINUE = 316,
    T_FOR = 317,
    T_TO = 318,
    T_CLASS = 319,
    T_R_ARROW = 320,
    T_INLINE = 321,
    T_INLINE_NOT = 322,
    T_D_L_ARROW = 323,
    T_IN = 324,
    T_QUESTION = 325,
    T_ENUM = 326,
    T_STRING = 327,
    T_CHAR = 328,
    T_CONST = 329,
    T_EXTEND = 330,
    T_FREE = 331,
    T_LESS_MINUS = 332,
    T_PUBLIC = 333,
    T_PRIVATE = 334,
    T_SWITCH = 335,
    T_CASE = 336,
    T_TEMPLATE = 337,
    NEGATIVE = 338
  };
#endif
/* Tokens.  */
#define T_INT_CONST 258
#define T_FLOAT 259
#define T_ID 260
#define T_SENTENCE 261
#define T_FUNC 262
#define T_END 263
#define T_INT 264
#define T_BOOL 265
#define T_TRUE 266
#define T_FALSE 267
#define T_NULL 268
#define T_TYPE 269
#define T_EQUAL 270
#define T_RETURN 271
#define T_WRITE 272
#define T_ALLOCATE 273
#define T_OF_LENGTH 274
#define T_IF 275
#define T_THEN 276
#define T_ELSE 277
#define T_WHILE 278
#define T_DO 279
#define T_ARRAY_OF 280
#define T_RECORD_OF 281
#define T_VAR 282
#define T_COLON 283
#define T_SEMICOLON 284
#define T_COMMA 285
#define T_DOT 286
#define T_OPEN_CURLY_B 287
#define T_CLOSE_CURLY_B 288
#define T_OPEN_B 289
#define T_CLOSE_B 290
#define T_OPEN_P 291
#define T_CLOSE_P 292
#define T_EQ 293
#define T_NEQ 294
#define T_GREATER 295
#define T_LESS 296
#define T_TILDE_LEFT 297
#define T_TILDE_RIGHT 298
#define T_GEQ 299
#define T_LEQ 300
#define T_AND 301
#define T_DEQUAL 302
#define T_NOT 303
#define T_PIPE 304
#define T_PLUS_E 305
#define T_MINUS_E 306
#define T_TIMES_E 307
#define T_DIV_E 308
#define T_INC 309
#define T_DEC 310
#define T_PLUS 311
#define T_MINUS 312
#define T_DIV 313
#define T_TIMES 314
#define T_BREAK 315
#define T_CONTINUE 316
#define T_FOR 317
#define T_TO 318
#define T_CLASS 319
#define T_R_ARROW 320
#define T_INLINE 321
#define T_INLINE_NOT 322
#define T_D_L_ARROW 323
#define T_IN 324
#define T_QUESTION 325
#define T_ENUM 326
#define T_STRING 327
#define T_CHAR 328
#define T_CONST 329
#define T_EXTEND 330
#define T_FREE 331
#define T_LESS_MINUS 332
#define T_PUBLIC 333
#define T_PRIVATE 334
#define T_SWITCH 335
#define T_CASE 336
#define T_TEMPLATE 337
#define NEGATIVE 338

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 20 "../frontend/parser/shere_khan.y" /* yacc.c:355  */

   int intconst;
   float floatconst;
   char *stringconst;
   struct expression *expression;
   struct type *type;
   struct function *function;
   struct head *head;
   struct tail *tail;
   struct par_decl_list *par_decl_list;
   struct var_decl_list *var_decl_list;
   struct var_type *var_type;
   struct decl_list *decl_list;
   struct body *body;
   struct declaration *declaration;
   struct statement_list *statement_list;
   struct statement *statement;
   struct variable *variable;
   struct term *term;
   struct act_list *act_list;
   struct expression_list *exp_list;
   struct inline_hint *inline_hint;
   struct enum_list *enum_list;
   struct parent_list *parent_list;
   struct access_modifier *access_modifier;
   struct switch_stm *switch_stm;
   struct switch_case *switch_case;
   struct template *template;
   struct temp_list *temp_list;
   struct temp_decl_list *temp_decl_list;
   struct temp_args *temp_args;
   struct temp_args_list *temp_args_list;
   struct temp_args_decl_list *temp_args_decl_list;

#line 326 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 343 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   852

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  84
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  113
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  255

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   338

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   158,   158,   162,   166,   169,   172,   175,   178,   180,
     184,   186,   189,   192,   196,   200,   204,   206,   208,   210,
     212,   214,   216,   218,   222,   225,   228,   230,   234,   238,
     240,   244,   246,   248,   250,   252,   254,   256,   258,   260,
     262,   264,   266,   268,   270,   272,   274,   278,   282,   284,
     286,   288,   290,   292,   295,   297,   299,   304,   306,   308,
     310,   312,   314,   316,   318,   320,   322,   324,   326,   328,
     330,   334,   337,   340,   342,   344,   346,   348,   352,   354,
     358,   360,   362,   364,   366,   370,   372,   374,   376,   378,
     380,   382,   384,   386,   388,   390,   394,   397,   400,   403,
     406,   408,   412,   415,   418,   420,   425,   428,   431,   433,
     436,   439,   441,   445
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_INT_CONST", "T_FLOAT", "T_ID",
  "T_SENTENCE", "T_FUNC", "T_END", "T_INT", "T_BOOL", "T_TRUE", "T_FALSE",
  "T_NULL", "T_TYPE", "T_EQUAL", "T_RETURN", "T_WRITE", "T_ALLOCATE",
  "T_OF_LENGTH", "T_IF", "T_THEN", "T_ELSE", "T_WHILE", "T_DO",
  "T_ARRAY_OF", "T_RECORD_OF", "T_VAR", "T_COLON", "T_SEMICOLON",
  "T_COMMA", "T_DOT", "T_OPEN_CURLY_B", "T_CLOSE_CURLY_B", "T_OPEN_B",
  "T_CLOSE_B", "T_OPEN_P", "T_CLOSE_P", "T_EQ", "T_NEQ", "T_GREATER",
  "T_LESS", "T_TILDE_LEFT", "T_TILDE_RIGHT", "T_GEQ", "T_LEQ", "T_AND",
  "T_DEQUAL", "T_NOT", "T_PIPE", "T_PLUS_E", "T_MINUS_E", "T_TIMES_E",
  "T_DIV_E", "T_INC", "T_DEC", "T_PLUS", "T_MINUS", "T_DIV", "T_TIMES",
  "T_BREAK", "T_CONTINUE", "T_FOR", "T_TO", "T_CLASS", "T_R_ARROW",
  "T_INLINE", "T_INLINE_NOT", "T_D_L_ARROW", "T_IN", "T_QUESTION",
  "T_ENUM", "T_STRING", "T_CHAR", "T_CONST", "T_EXTEND", "T_FREE",
  "T_LESS_MINUS", "T_PUBLIC", "T_PRIVATE", "T_SWITCH", "T_CASE",
  "T_TEMPLATE", "NEGATIVE", "$accept", "program", "function", "template",
  "temp_list", "temp_decl_list", "inline_hint", "tail", "head", "body",
  "type", "par_decl_list", "var_decl_list", "var_type", "statement_list",
  "statement", "expression", "decl_list", "declaration", "enum_list",
  "variable", "term", "temp_args", "temp_args_list", "temp_args_decl_list",
  "act_list", "exp_list", "parent_list", "access_modifier", "switch_stm",
  "switch_case", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338
};
# endif

#define YYPACT_NINF -140

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-140)))

#define YYTABLE_NINF -111

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     187,   -13,  -140,  -140,    32,  -140,   151,   187,    17,    20,
    -140,  -140,   301,   301,    28,   301,   301,   151,   151,     8,
       9,   151,     6,    16,   151,  -140,   221,  -140,    40,    41,
      57,    13,  -140,   -50,    35,    14,  -140,  -140,   -22,  -140,
    -140,  -140,  -140,   301,   305,   301,   301,   453,     5,  -140,
     474,    29,   338,   364,    36,   127,  -140,  -140,   279,    28,
     301,  -140,   290,    67,   301,   301,   301,   301,   301,    45,
      58,    70,    21,   301,    71,    64,    66,    63,  -140,    54,
    -140,  -140,    57,    74,  -140,    34,   103,   113,  -140,  -140,
     118,    20,    97,    25,    91,   728,  -140,   772,  -140,  -140,
     301,   301,   301,   301,   301,   301,   301,    82,   301,   301,
     301,   301,  -140,   301,  -140,   151,   151,   301,  -140,   301,
     495,   -24,   751,   301,   316,  -140,   705,    98,   516,   537,
     558,   579,  -140,  -140,   101,   105,   600,    57,    57,  -140,
      41,  -140,  -140,    41,    65,   123,   111,    99,  -140,   141,
     187,  -140,  -140,   119,   109,  -140,   301,  -140,  -140,   140,
      46,    46,    46,    46,   793,   140,   301,   -37,   -37,  -140,
    -140,   621,   126,  -140,   642,   388,   151,   124,   122,   125,
    -140,   301,   301,  -140,  -140,  -140,  -140,  -140,   301,  -140,
    -140,   128,  -140,  -140,   129,   150,   131,   301,   113,  -140,
     130,   156,    57,  -140,   135,  -140,   793,  -140,   151,  -140,
     151,   149,  -140,    84,   147,   410,  -140,   142,  -140,  -140,
    -140,    10,   663,  -140,    41,   173,  -140,  -140,  -140,  -140,
    -140,   151,   301,   153,    84,  -140,   301,  -140,   157,  -140,
     145,  -140,  -140,  -140,   432,  -140,  -140,   684,  -140,   163,
     151,  -140,    57,  -140,  -140
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      72,     0,   108,   109,     0,     2,     0,    72,     5,     0,
       1,    80,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    15,    29,     0,    71,     0,     0,
       5,     0,    74,    12,    78,     0,    90,    91,    80,    95,
      92,    93,    94,     0,     0,     0,     0,     0,    85,    69,
       0,     0,     0,     0,     0,     0,    48,    49,     0,     0,
       0,    30,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,    18,    97,
      17,    19,     5,     0,    20,     0,     0,     7,    10,    11,
       0,     0,     0,     5,     0,     0,    88,     0,    70,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    32,     0,    33,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,    82,   104,     0,     0,     0,
       0,     0,    46,    47,    83,     0,     0,     5,     5,    75,
       0,    16,    21,     0,   107,     0,     9,     0,     6,     0,
      72,    79,    76,   101,     0,    98,   103,    87,    89,    62,
      63,    64,    65,    66,    67,    61,     0,    59,    60,    58,
      57,     0,    37,    39,     0,     0,     0,     0,     0,     0,
      35,     0,     0,    81,    42,    43,    44,    45,   103,    52,
      55,     0,    28,    26,     0,     0,     0,     0,     0,     4,
       0,     0,     5,    96,     0,   102,    68,    34,     0,    40,
       0,     0,    54,     0,     0,     0,   105,     0,    73,    22,
     106,   110,     0,     8,    25,     0,     3,   100,    86,    38,
      51,     0,     0,     0,   111,    36,     0,    84,     0,    77,
       0,    24,    13,    50,     0,    56,   112,     0,    23,     0,
       0,    53,     5,   113,    14
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -140,  -140,  -140,   192,  -140,     7,  -140,  -140,  -140,    56,
      -2,  -140,  -139,  -140,   191,   -15,    11,    -4,  -140,   132,
      -6,   166,   136,  -140,    18,    30,   -59,  -140,  -140,   -12,
    -140
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,    32,    85,   147,   148,    90,   226,   150,     5,
     153,   240,    76,    77,    24,    25,   126,     6,     7,    35,
      48,    49,    94,   154,   155,   204,   205,   196,     8,   233,
     234
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      26,   193,    54,    27,   194,   127,    58,    63,    51,    61,
      64,    26,    26,   177,   -97,    26,    88,    89,    26,     9,
      93,   110,   111,    47,    50,    34,    52,    53,    86,    78,
      79,    28,    10,    11,    80,    81,    63,    56,    57,    64,
      61,    71,    59,   -72,    29,    74,    75,    92,   113,    26,
      82,    83,    60,   121,    95,    87,    97,    98,   114,   117,
      63,    78,    79,    64,   179,    91,    80,    81,   -99,   120,
      71,   122,   125,   124,   132,   134,   128,   129,   130,   131,
     142,     1,    82,    83,   136,   241,   137,   133,     2,     3,
     135,    30,   138,   140,    71,   139,    93,    84,   144,    31,
     172,   173,   108,   109,   110,   111,   143,    31,   145,    26,
      26,   159,   160,   161,   162,   163,   164,   165,   146,   167,
     168,   169,   170,   216,   171,   149,   152,   156,   174,    84,
     175,   166,    11,   183,   189,   191,   192,   188,   197,    31,
     195,   198,   199,    12,    13,    14,   200,    15,   208,   202,
      16,    17,   203,   212,   213,   220,    11,   218,   214,    18,
     118,   211,   219,   221,   225,   232,   224,    12,    13,    14,
      26,    15,   228,   231,    16,    17,   235,   206,   242,   237,
     101,   102,   249,    18,   103,   104,   245,    19,    20,    21,
     248,   252,   215,   229,  -110,   230,   108,   109,   110,   111,
      33,  -110,    26,    22,    26,   223,   201,    23,   222,    55,
      96,    19,    20,    21,  -110,   141,   243,   238,   217,     0,
     227,     0,   246,   151,     0,    26,     0,    22,     0,     0,
       0,    23,     0,     0,     0,   253,    62,     0,     0,     0,
       0,     0,     0,   244,    26,     0,     0,   247,     0,     0,
     254,     0,    63,  -110,  -110,    64,     0,     0,     1,     0,
       0,  -110,     0,     0,     0,     2,     3,     0,     0,  -110,
       0,    65,    66,    67,    68,    69,    70,     0,     0,     0,
       0,     0,    36,    37,    38,    39,    71,     0,     0,    72,
      40,    41,    42,    36,    37,    38,    39,     0,    73,     0,
       0,    40,    41,    42,    36,    37,    38,    39,    36,    37,
      38,    39,    40,    41,    42,    43,    40,    41,    42,     0,
       0,     0,   123,     0,     0,     0,    43,    44,    45,     0,
       0,     0,     0,     0,     0,     0,    46,    43,    44,    45,
       0,    43,   119,     0,     0,   180,     0,    46,     0,    44,
      45,     0,     0,    44,    45,   100,   101,   102,    46,   115,
     103,   104,   105,   106,     0,   107,     0,     0,     0,     0,
       0,     0,   108,   109,   110,   111,     0,   100,   101,   102,
       0,     0,   103,   104,   105,   106,   181,   107,   116,     0,
       0,     0,     0,     0,   108,   109,   110,   111,     0,     0,
       0,     0,     0,   100,   101,   102,     0,     0,   103,   104,
     105,   106,   210,   107,     0,     0,     0,     0,     0,     0,
     108,   109,   110,   111,     0,     0,     0,   100,   101,   102,
       0,     0,   103,   104,   105,   106,     0,   107,   236,     0,
       0,     0,     0,     0,   108,   109,   110,   111,     0,   100,
     101,   102,     0,     0,   103,   104,   105,   106,     0,   107,
     250,     0,     0,     0,     0,     0,   108,   109,   110,   111,
       0,   100,   101,   102,     0,     0,   103,   104,   105,   106,
       0,   107,    99,     0,     0,     0,     0,     0,   108,   109,
     110,   111,   100,   101,   102,     0,     0,   103,   104,   105,
     106,     0,   107,   112,     0,     0,     0,     0,     0,   108,
     109,   110,   111,   100,   101,   102,     0,     0,   103,   104,
     105,   106,     0,   107,   176,     0,     0,     0,     0,     0,
     108,   109,   110,   111,   100,   101,   102,     0,     0,   103,
     104,   105,   106,     0,   107,   184,     0,     0,     0,     0,
       0,   108,   109,   110,   111,   100,   101,   102,     0,     0,
     103,   104,   105,   106,     0,   107,   185,     0,     0,     0,
       0,     0,   108,   109,   110,   111,   100,   101,   102,     0,
       0,   103,   104,   105,   106,     0,   107,   186,     0,     0,
       0,     0,     0,   108,   109,   110,   111,   100,   101,   102,
       0,     0,   103,   104,   105,   106,     0,   107,   187,     0,
       0,     0,     0,     0,   108,   109,   110,   111,   100,   101,
     102,     0,     0,   103,   104,   105,   106,     0,   107,   190,
       0,     0,     0,     0,     0,   108,   109,   110,   111,   100,
     101,   102,     0,     0,   103,   104,   105,   106,     0,   107,
     207,     0,     0,     0,     0,     0,   108,   109,   110,   111,
     100,   101,   102,     0,     0,   103,   104,   105,   106,     0,
     107,   209,     0,     0,     0,     0,     0,   108,   109,   110,
     111,   100,   101,   102,     0,     0,   103,   104,   105,   106,
       0,   107,   239,     0,     0,     0,     0,     0,   108,   109,
     110,   111,   100,   101,   102,     0,     0,   103,   104,   105,
     106,     0,   107,   251,     0,     0,     0,     0,     0,   108,
     109,   110,   111,   100,   101,   102,     0,     0,   103,   104,
     105,   106,     0,   107,     0,   182,     0,     0,     0,     0,
     108,   109,   110,   111,   100,   101,   102,     0,     0,   103,
     104,   105,   106,     0,   107,     0,     0,     0,     0,     0,
       0,   108,   109,   110,   111,   157,     0,   100,   101,   102,
       0,     0,   103,   104,   105,   106,     0,   107,     0,     0,
       0,     0,     0,     0,   108,   109,   110,   111,   178,     0,
     100,   101,   102,     0,     0,   103,   104,   105,   106,     0,
     107,     0,     0,     0,     0,     0,     0,   108,   109,   110,
     111,   100,   101,   102,     0,     0,   103,   104,   105,   106,
       0,   158,     0,     0,     0,     0,     0,     0,   108,   109,
     110,   111,   100,   101,   102,     0,     0,   103,   104,     0,
     106,     0,     0,     0,     0,     0,     0,     0,     0,   108,
     109,   110,   111
};

static const yytype_int16 yycheck[] =
{
       6,   140,    17,     7,   143,    64,    21,    31,    14,    24,
      34,    17,    18,    37,    36,    21,    66,    67,    24,    32,
      42,    58,    59,    12,    13,     5,    15,    16,    30,     4,
       5,    14,     0,     5,     9,    10,    31,    29,    29,    34,
      55,    65,    36,    33,    27,     5,     5,    33,    19,    55,
      25,    26,    36,    59,    43,    42,    45,    46,    29,    23,
      31,     4,     5,    34,   123,    30,     9,    10,    43,    58,
      65,    60,     5,    62,    29,     5,    65,    66,    67,    68,
      82,    71,    25,    26,    73,   224,    15,    29,    78,    79,
      69,    74,    28,    30,    65,    29,    42,    72,    64,    82,
     115,   116,    56,    57,    58,    59,    32,    82,     5,   115,
     116,   100,   101,   102,   103,   104,   105,   106,     5,   108,
     109,   110,   111,   182,   113,     7,    29,    36,   117,    72,
     119,    49,     5,    35,    29,   137,   138,    36,    15,    82,
      75,    30,    43,    16,    17,    18,     5,    20,    22,    30,
      23,    24,    43,    29,    32,     5,     5,    29,    33,    32,
      33,   176,    33,    32,     8,    81,    36,    16,    17,    18,
     176,    20,    37,    24,    23,    24,    29,   166,     5,    37,
      40,    41,    37,    32,    44,    45,    33,    60,    61,    62,
      33,    28,   181,   208,     7,   210,    56,    57,    58,    59,
       8,    14,   208,    76,   210,   198,   150,    80,   197,    18,
      44,    60,    61,    62,    27,    79,   231,   221,   188,    -1,
     202,    -1,   234,    91,    -1,   231,    -1,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,   250,    15,    -1,    -1,    -1,
      -1,    -1,    -1,   232,   250,    -1,    -1,   236,    -1,    -1,
     252,    -1,    31,    66,    67,    34,    -1,    -1,    71,    -1,
      -1,    74,    -1,    -1,    -1,    78,    79,    -1,    -1,    82,
      -1,    50,    51,    52,    53,    54,    55,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    65,    -1,    -1,    68,
      11,    12,    13,     3,     4,     5,     6,    -1,    77,    -1,
      -1,    11,    12,    13,     3,     4,     5,     6,     3,     4,
       5,     6,    11,    12,    13,    36,    11,    12,    13,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    36,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    36,    48,    49,
      -1,    36,    63,    -1,    -1,    29,    -1,    57,    -1,    48,
      49,    -1,    -1,    48,    49,    39,    40,    41,    57,    21,
      44,    45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    57,    58,    59,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    46,    47,    70,    49,    24,    -1,
      -1,    -1,    -1,    -1,    56,    57,    58,    59,    -1,    -1,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      46,    47,    24,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    46,    47,    -1,    49,    28,    -1,
      -1,    -1,    -1,    -1,    56,    57,    58,    59,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    46,    47,    -1,    49,
      28,    -1,    -1,    -1,    -1,    -1,    56,    57,    58,    59,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    46,    47,
      -1,    49,    29,    -1,    -1,    -1,    -1,    -1,    56,    57,
      58,    59,    39,    40,    41,    -1,    -1,    44,    45,    46,
      47,    -1,    49,    29,    -1,    -1,    -1,    -1,    -1,    56,
      57,    58,    59,    39,    40,    41,    -1,    -1,    44,    45,
      46,    47,    -1,    49,    29,    -1,    -1,    -1,    -1,    -1,
      56,    57,    58,    59,    39,    40,    41,    -1,    -1,    44,
      45,    46,    47,    -1,    49,    29,    -1,    -1,    -1,    -1,
      -1,    56,    57,    58,    59,    39,    40,    41,    -1,    -1,
      44,    45,    46,    47,    -1,    49,    29,    -1,    -1,    -1,
      -1,    -1,    56,    57,    58,    59,    39,    40,    41,    -1,
      -1,    44,    45,    46,    47,    -1,    49,    29,    -1,    -1,
      -1,    -1,    -1,    56,    57,    58,    59,    39,    40,    41,
      -1,    -1,    44,    45,    46,    47,    -1,    49,    29,    -1,
      -1,    -1,    -1,    -1,    56,    57,    58,    59,    39,    40,
      41,    -1,    -1,    44,    45,    46,    47,    -1,    49,    29,
      -1,    -1,    -1,    -1,    -1,    56,    57,    58,    59,    39,
      40,    41,    -1,    -1,    44,    45,    46,    47,    -1,    49,
      29,    -1,    -1,    -1,    -1,    -1,    56,    57,    58,    59,
      39,    40,    41,    -1,    -1,    44,    45,    46,    47,    -1,
      49,    29,    -1,    -1,    -1,    -1,    -1,    56,    57,    58,
      59,    39,    40,    41,    -1,    -1,    44,    45,    46,    47,
      -1,    49,    29,    -1,    -1,    -1,    -1,    -1,    56,    57,
      58,    59,    39,    40,    41,    -1,    -1,    44,    45,    46,
      47,    -1,    49,    29,    -1,    -1,    -1,    -1,    -1,    56,
      57,    58,    59,    39,    40,    41,    -1,    -1,    44,    45,
      46,    47,    -1,    49,    -1,    30,    -1,    -1,    -1,    -1,
      56,    57,    58,    59,    39,    40,    41,    -1,    -1,    44,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    57,    58,    59,    37,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    46,    47,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    58,    59,    37,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    46,    47,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    58,
      59,    39,    40,    41,    -1,    -1,    44,    45,    46,    47,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,
      58,    59,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      57,    58,    59
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    71,    78,    79,    85,    93,   101,   102,   112,    32,
       0,     5,    16,    17,    18,    20,    23,    24,    32,    60,
      61,    62,    76,    80,    98,    99,   104,   101,    14,    27,
      74,    82,    86,    87,     5,   103,     3,     4,     5,     6,
      11,    12,    13,    36,    48,    49,    57,   100,   104,   105,
     100,   104,   100,   100,    99,    98,    29,    29,    99,    36,
      36,    99,    15,    31,    34,    50,    51,    52,    53,    54,
      55,    65,    68,    77,     5,     5,    96,    97,     4,     5,
       9,    10,    25,    26,    72,    87,    94,    42,    66,    67,
      90,    30,    33,    42,   106,   100,   105,   100,   100,    29,
      39,    40,    41,    44,    45,    46,    47,    49,    56,    57,
      58,    59,    29,    19,    29,    21,    24,    23,    33,    63,
     100,   104,   100,    32,   100,     5,   100,   110,   100,   100,
     100,   100,    29,    29,     5,    69,   100,    15,    28,    29,
      30,   106,    94,    32,    64,     5,     5,    88,    89,     7,
      92,   103,    29,    94,   107,   108,    36,    37,    49,   100,
     100,   100,   100,   100,   100,   100,    49,   100,   100,   100,
     100,   100,    99,    99,   100,   100,    29,    37,    37,   110,
      29,    70,    30,    35,    29,    29,    29,    29,    36,    29,
      29,    94,    94,    96,    96,    75,   111,    15,    30,    43,
       5,    93,    30,    43,   109,   110,   100,    29,    22,    29,
      24,    99,    29,    32,    33,   100,   110,   109,    29,    33,
       5,    32,   100,    89,    36,     8,    91,   108,    37,    99,
      99,    24,    81,   113,   114,    29,    28,    37,   101,    29,
      95,    96,     5,    99,   100,    33,   113,   100,    33,    37,
      28,    29,    28,    99,    94
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    84,    85,    86,    87,    87,    88,    88,    89,    89,
      90,    90,    90,    91,    92,    93,    94,    94,    94,    94,
      94,    94,    94,    94,    95,    95,    96,    96,    97,    98,
      98,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   101,   101,   102,   102,   102,   102,   102,   103,   103,
     104,   104,   104,   104,   104,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   106,   106,   107,   107,
     108,   108,   109,   109,   110,   110,   111,   111,   112,   112,
     112,   113,   113,   114
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     5,     4,     0,     1,     0,     3,     1,
       1,     1,     0,     2,     7,     2,     2,     1,     1,     1,
       1,     2,     4,     6,     1,     0,     3,     1,     3,     1,
       2,     3,     3,     3,     5,     4,     6,     4,     6,     4,
       5,     3,     4,     4,     4,     4,     3,     3,     2,     2,
       7,     6,     4,     8,     5,     4,     7,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     1,
       2,     2,     0,     6,     2,     4,     5,     7,     1,     3,
       1,     4,     3,     3,     6,     1,     5,     3,     2,     3,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     0,
       3,     1,     1,     0,     1,     3,     2,     0,     1,     1,
       0,     1,     2,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 159 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {program = (yyvsp[0].body);}
#line 1727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 163 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.function) = make_function((yyvsp[-4].template), (yyvsp[-3].inline_hint), (yyvsp[-2].head), (yyvsp[-1].body), (yyvsp[0].tail));}
#line 1733 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 167 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.template) = make_template((yyvsp[-1].temp_list)); }
#line 1739 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 169 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.template) = make_template_empty(); }
#line 1745 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 173 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.temp_list) = make_tl((yyvsp[0].temp_decl_list)); }
#line 1751 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 175 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.temp_list) = make_tl_empty(); }
#line 1757 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 179 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.temp_decl_list) = make_tdl_id_list((yyvsp[-2].stringconst), (yyvsp[0].temp_decl_list)); }
#line 1763 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 181 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.temp_decl_list) = make_tdl_id((yyvsp[0].stringconst)); }
#line 1769 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 185 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.inline_hint) = make_inline_hint(IL_IL);}
#line 1775 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 187 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.inline_hint) = make_inline_hint(IL_NOT);}
#line 1781 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 189 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.inline_hint) = make_inline_hint(IL_EMPTY);}
#line 1787 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 193 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.tail) = make_tail((yyvsp[0].stringconst));}
#line 1793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 197 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.head) = make_head((yyvsp[-5].stringconst), (yyvsp[-3].par_decl_list), (yyvsp[0].type));}
#line 1799 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 201 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.body) = make_body((yyvsp[-1].decl_list), (yyvsp[0].statement_list));}
#line 1805 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 205 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.type) = make_type_id((yyvsp[-1].stringconst), (yyvsp[0].temp_args));}
#line 1811 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 207 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.type) = make_type_int();}
#line 1817 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 209 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.type) = make_type_float();}
#line 1823 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 211 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.type) = make_type_bool();}
#line 1829 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 213 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.type) = make_type_string();}
#line 1835 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 215 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.type) = make_type_array((yyvsp[0].type));}
#line 1841 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 217 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.type) = make_type_record((yyvsp[-1].var_decl_list));}
#line 1847 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 219 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.type) = make_type_class((yyvsp[-5].template), (yyvsp[-3].parent_list), (yyvsp[-1].decl_list));}
#line 1853 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 223 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.par_decl_list) = make_pdl_vdl((yyvsp[0].var_decl_list));}
#line 1859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 225 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.par_decl_list) = make_pdl_empty();}
#line 1865 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 229 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.var_decl_list) = make_vdl_vt_list((yyvsp[-2].var_type), (yyvsp[0].var_decl_list));}
#line 1871 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 231 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.var_decl_list) = make_vdl_vt((yyvsp[0].var_type));}
#line 1877 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 235 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.var_type) = make_var_type((yyvsp[-2].stringconst), (yyvsp[0].type));}
#line 1883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 239 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement_list) = make_sl_statement((yyvsp[0].statement));}
#line 1889 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 241 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement_list) = make_sl_statement_list((yyvsp[0].statement), (yyvsp[-1].statement_list));}
#line 1895 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 245 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_return((yyvsp[-1].expression));}
#line 1901 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 247 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_write((yyvsp[-1].expression));}
#line 1907 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 249 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_allocate((yyvsp[-1].variable));}
#line 1913 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 251 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_allocate_length((yyvsp[-3].variable), (yyvsp[-1].expression));}
#line 1919 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 253 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_assignment((yyvsp[-3].variable), (yyvsp[-1].expression));}
#line 1925 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 255 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_assignment_list((yyvsp[-5].variable), (yyvsp[-2].exp_list));}
#line 1931 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 257 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_if((yyvsp[-2].expression), (yyvsp[0].statement));}
#line 1937 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 259 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_else((yyvsp[-4].expression), (yyvsp[-2].statement), (yyvsp[0].statement));}
#line 1943 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 261 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_while((yyvsp[-2].expression), (yyvsp[0].statement), 0);}
#line 1949 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 263 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_while((yyvsp[-1].expression), (yyvsp[-3].statement), 1);}
#line 1955 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 265 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_sl((yyvsp[-1].statement_list));}
#line 1961 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 267 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_op_assignment(EXP_PLUS, (yyvsp[-3].variable), (yyvsp[-1].expression));}
#line 1967 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 269 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_op_assignment(EXP_MINUS, (yyvsp[-3].variable), (yyvsp[-1].expression));}
#line 1973 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 271 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_op_assignment(EXP_TIMES, (yyvsp[-3].variable), (yyvsp[-1].expression));}
#line 1979 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 273 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_op_assignment(EXP_DIV, (yyvsp[-3].variable), (yyvsp[-1].expression));}
#line 1985 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 275 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.statement) = make_statement_assignment((yyvsp[-2].variable), make_exp(EXP_PLUS,
                                            make_exp_term(make_term_var((yyvsp[-2].variable))),
                                            make_exp_term(make_term_int(1))));}
#line 1993 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 279 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_assignment((yyvsp[-2].variable), make_exp(EXP_MINUS,
                                            make_exp_term(make_term_var((yyvsp[-2].variable))),
                                            make_exp_term(make_term_int(1))));}
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 283 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_keyword(STM_BREAK);}
#line 2007 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 285 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_keyword(STM_CONTINUE);}
#line 2013 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 287 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_for_loop((yyvsp[-5].statement), (yyvsp[-4].expression), (yyvsp[-2].statement), (yyvsp[0].statement));}
#line 2019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 289 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_to_loop((yyvsp[-4].statement), (yyvsp[-2].expression), (yyvsp[0].statement));}
#line 2025 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 291 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_in((yyvsp[-3].variable)); }
#line 2031 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 294 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_cond_exp((yyvsp[-7].variable), (yyvsp[-5].expression), (yyvsp[-3].expression), (yyvsp[-1].expression));}
#line 2037 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 296 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_free((yyvsp[-2].variable));}
#line 2043 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 298 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_assign_val((yyvsp[-3].variable), (yyvsp[-1].expression));}
#line 2049 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 301 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.statement) = make_statement_switch((yyvsp[-4].expression), (yyvsp[-1].switch_stm));}
#line 2055 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 305 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_TIMES, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2061 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 307 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_DIV, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2067 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 309 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_PLUS, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2073 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 311 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_MINUS, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2079 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 313 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_EQ, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2085 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 315 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_NEQ, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2091 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 317 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_GREATER, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2097 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 319 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_LESS, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2103 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 321 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_GEQ, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2109 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 323 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_LEQ, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2115 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 325 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_AND, (yyvsp[-2].expression), (yyvsp[0].expression));}
#line 2121 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 327 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_OR, (yyvsp[-3].expression), (yyvsp[0].expression));}
#line 2127 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 329 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp_term((yyvsp[0].term));}
#line 2133 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 331 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.expression) = make_exp(EXP_MINUS, make_exp_term(make_term_int(0)), (yyvsp[0].expression));}
#line 2139 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 335 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.decl_list) = make_dl_declaration((yyvsp[-1].declaration), (yyvsp[0].decl_list));}
#line 2145 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 337 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.decl_list) = make_dl_empty();}
#line 2151 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 341 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.declaration) = make_declaration_type((yyvsp[-5].access_modifier), (yyvsp[-3].stringconst), (yyvsp[-1].type));}
#line 2157 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 343 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.declaration) = make_declaration_function((yyvsp[-1].access_modifier), (yyvsp[0].function));}
#line 2163 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 345 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.declaration) = make_declaration_vdl((yyvsp[-3].access_modifier), (yyvsp[-1].var_decl_list));}
#line 2169 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 347 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.declaration) = make_decl_enum((yyvsp[-2].enum_list));}
#line 2175 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 349 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.declaration) = make_decl_const((yyvsp[-6].access_modifier), (yyvsp[-4].type), (yyvsp[-3].stringconst), (yyvsp[-1].expression));}
#line 2181 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 353 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.enum_list) = make_enum_id((yyvsp[0].stringconst));}
#line 2187 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 355 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.enum_list) = make_enum_list((yyvsp[-2].stringconst), (yyvsp[0].enum_list));}
#line 2193 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 359 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.variable) = make_var_id((yyvsp[0].stringconst));}
#line 2199 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 361 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.variable) = make_var_exp((yyvsp[-3].variable), (yyvsp[-1].exp_list));}
#line 2205 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 363 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.variable) = make_var_dot((yyvsp[-2].variable), (yyvsp[0].stringconst));}
#line 2211 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 365 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.variable) = make_var_arrow((yyvsp[-2].variable), (yyvsp[0].stringconst));}
#line 2217 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 367 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.variable) = make_var_func((yyvsp[-5].variable), (yyvsp[-3].stringconst), (yyvsp[-1].act_list));}
#line 2223 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 371 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term_var((yyvsp[0].variable));}
#line 2229 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 373 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term_al((yyvsp[-4].stringconst), (yyvsp[-3].temp_args), (yyvsp[-1].act_list));}
#line 2235 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 375 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term_parentheses((yyvsp[-1].expression));}
#line 2241 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 377 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term_not((yyvsp[0].term));}
#line 2247 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 379 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term_absolute((yyvsp[-1].expression));}
#line 2253 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 381 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term_int((yyvsp[0].intconst));}
#line 2259 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 383 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term_float((yyvsp[0].floatconst));}
#line 2265 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 385 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term(TERM_TRUE);}
#line 2271 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 387 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term(TERM_FALSE);}
#line 2277 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 389 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term(TERM_NULL);}
#line 2283 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 391 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.term) = make_term_string((yyvsp[0].stringconst));}
#line 2289 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 395 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.temp_args) = make_temp_args((yyvsp[-1].temp_args_list)); }
#line 2295 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 397 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.temp_args) = make_temp_args_empty(); }
#line 2301 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 401 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.temp_args_list) = make_temp_args_list((yyvsp[0].temp_args_decl_list)); }
#line 2307 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 403 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.temp_args_list) = make_temp_args_list_empty(); }
#line 2313 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 407 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.temp_args_decl_list) = make_temp_args_decl_type_list((yyvsp[-2].type), (yyvsp[0].temp_args_decl_list)); }
#line 2319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 409 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    { (yyval.temp_args_decl_list) = make_temp_args_decl_type((yyvsp[0].type)); }
#line 2325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 413 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.act_list) = make_act_list((yyvsp[0].exp_list));}
#line 2331 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 415 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.act_list) = make_act_list_empty();}
#line 2337 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 419 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.exp_list) = make_exp_list_exp((yyvsp[0].expression));}
#line 2343 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 421 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.exp_list) = make_exp_list_list((yyvsp[-2].expression), (yyvsp[0].exp_list));}
#line 2349 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 426 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.parent_list) = make_parent_list((yyvsp[0].stringconst));}
#line 2355 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 428 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.parent_list) = make_parent_list_empty();}
#line 2361 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 432 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.access_modifier) = make_access_modifier(AM_PUBLIC);}
#line 2367 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 434 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.access_modifier) = make_access_modifier(AM_PRIVATE);}
#line 2373 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 436 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.access_modifier) = make_access_modifier(AM_PUBLIC);}
#line 2379 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 440 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.switch_stm) = make_switch_stm_case((yyvsp[0].switch_case));}
#line 2385 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 442 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.switch_stm) = make_switch_stm_list((yyvsp[-1].switch_case), (yyvsp[0].switch_stm));}
#line 2391 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 446 "../frontend/parser/shere_khan.y" /* yacc.c:1646  */
    {(yyval.switch_case) = make_switch_case((yyvsp[-2].expression), (yyvsp[0].statement));}
#line 2397 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2401 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 449 "../frontend/parser/shere_khan.y" /* yacc.c:1906  */

