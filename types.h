#ifndef _TYPES_H_
#define _TYPES_H

#define OPERATIONS_COUNT 13

typedef enum
{
    NOT,
    INPUT,
    OUTPUT,
    ADD,
    MULT,
    SUB,
    POW,
    DIV,
    REM,
    XOR,
    AND,
    OR,
    ASSIGN
} operation;

typedef enum
{
    left,
    right,
    middle
} operation_syntax;

typedef enum
{
    unary,
    binary
} operation_type;

static char * operations_names[]             = {"not", "input", "output", "add", "mult", "sub",   "pow",  "div",  "rem",  "xor",  "and",  "or",   "="};
static operation_syntax basic_syntax[]       = {left,   left,   left,     middle, middle, middle, middle, middle, middle, middle, middle, middle, middle};
static const operation_type basic_types[]    = {unary,  unary,  unary,    binary, binary, binary, binary, binary, binary, binary, binary, binary, binary};

static operation_syntax operation_result_type = left;

#endif