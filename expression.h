#pragma once
#include <stdint.h>

#include "main.h"
#include "slice2.h"
#include "hashmap.h"
#include "emitter.h"
//#include "statement.h"

enum type_of
{
    t_not,
    t_star,
    t_divide,
    t_mod,
    t_plus,
    t_minus,
    t_l,
    t_lt,
    t_g,
    t_gt,
    t_eq,
    t_neq,
    t_and,
    t_or,
    t_num,
    t_var,
    t_func,
    t_print,
    t_error
};

typedef struct expression expression;

typedef union {
    struct func *function;

    Slice* name;
    uint64_t value;
} character;


struct expression {
    expression* left;
    expression* right;
    character* character;
    enum type_of type;
};

void compile(emitter_t*, expression*);

void free_expression(expression*);
uint64_t eval_expr(Interpreter*, struct map*, expression*);
