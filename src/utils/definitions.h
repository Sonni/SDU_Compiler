#ifndef COMPILER_DEFINITIONS_H
#define COMPILER_DEFINITIONS_H

#define NORM            "\x1B[0m"
#define RED             "\x1B[31m"
#define YEL             "\x1B[33m"

#define RELEASE         true
#define DEBUG           false

#define ENABLE_WARNINGS false
#define PRINT_ERRORS    true

bool PRINT_TYPES;
bool print_liveness;
bool print_use_def;
bool print_debug;

#define SEARCH_FOR_ERR  true

bool WEED;

#define TYPECHECK       true
#define COLLECT         true
#define CALC            true
#define CHECK           true

#define PRINT           true

#endif
