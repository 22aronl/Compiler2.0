#pragma once    
#include "statement.h"

typedef struct {
    struct map* variable_map;
    int32_t* registers;
    bool* used_before;
    int16_t* name_to_register;
    uint32_t variables_index;
    struct var_bin** variables;
    bool* out_blocks_dag;
    Slice** name_array;
} registers_t;

struct queue {
    uint32_t block_index;
    bool has_next;
    struct queue* next;
};

struct var_bin {
    int32_t* vars;
    uint32_t size;
    uint32_t index;
};

typedef struct {
    statement** statements;
    uint32_t statement_size;
    bool unconditional_jump;
    expression* jump_expression;
    // int32_t* expr_vars;
    // uint16_t expr_vars_size;
    uint32_t* in_blocks;
    uint32_t in_blocks_index;
    uint32_t in_blocks_size;
    //If conditional expression is true, jump to index 0
    uint32_t* out_blocks;
    uint32_t out_blocks_index;
    uint32_t out_blocks_size;
    struct map* variable_map;
    struct var_bin** variables;
    uint32_t variables_index;
    uint32_t variables_size;
    bool loop;
    uint32_t out_blocks_size_dag;
    bool* in_blocks_dag;
    bool* out_blocks_dag;
    bool* defined_in_block;
} block_t;


typedef struct {
    block_t** blocks;
    uint32_t block_size;
    Slice *name;
    Slice *parameters;
    uint16_t args;
} method_t;

method_t* parse_method(struct declare * declare);
struct map* create_small_map();
void create_next_use_information(block_t *block, struct map* map);
void live_variable_analysis(method_t *method);