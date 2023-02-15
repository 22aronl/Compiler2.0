#pragma once

#include <stdint.h>
#include "slice2.h"
#include "expression.h"

enum type_of_s
{
    s_declare,
    s_func,
    s_var,
    s_print,
    s_if,
    s_while,
    s_return
};

struct declare
{
    Slice *name;
    Slice *parameters;
    statement **body;
    uint16_t args;
    uint32_t size_body;
};

struct func
{
    Slice *name;
    expression **parameters;
    uint16_t args;
};

struct var
{
    Slice *name;
    expression *expr;
};

struct print
{
    expression *expr;
};

struct if_statement
{
    expression *condition;
    statement **body;
    statement **else_body;
    bool has_else;
    uint32_t size_body;
    uint32_t size_else;
};

struct while_statement
{
    expression *condition;
    statement **body;
    uint32_t size_body;
};

struct return_statement
{
    expression *expr;
};

union internal
{
    struct declare *declare;
    struct func *func;
    struct var *var;
    struct print *print;
    struct if_statement *if_statement;
    struct while_statement *while_statement;
    struct return_statement *return_statement;
};

struct statement
{
    union internal *internal;
    enum type_of_s type;
    // int32_t* variables;
    // uint16_t variables_size;
};

struct map *create_map();
void free_map(struct map *map);
void compile_statement(emitter_t*, statement*, struct declare *declare);
void call_function(emitter_t *emitter, struct func *func);

void free_function(struct func *func);
void free_statement(statement *s);
bool evaluate_statement(Interpreter *in, struct map *map, statement *s, uint64_t *return_value);
uint64_t evaluate_function(Interpreter *in, struct map *map, struct func *func, struct declare *declare);