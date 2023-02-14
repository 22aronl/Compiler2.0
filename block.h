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
    struct map* function_variable_map;
    emitter_t* emitter;

    size_t functions_stack;
    uint16_t functions_size;
    uint16_t functions_counter;
} registers_t;

struct queue {
    uint32_t block_index;
    bool has_next;
    struct queue* next;
};

struct queue_name {
    Slice* name;
    bool has_next;
    struct queue_name* next;
};

struct queue_head {
    struct queue_name* head;
    struct queue_name* tail;
};



struct var_bin {
    int32_t* vars;
    uint32_t size;
    uint32_t index;
};

typedef struct {
    statement** statements;
    uint32_t statement_size;
    bool has_jump;
    bool unconditional_jump;
    expression* jump_expression;
    // int32_t* expr_vars;
    // uint16_t expr_vars_size;
    uint32_t* in_blocks;
    uint32_t in_blocks_index;
    uint32_t in_blocks_size;
    //If conditional expression is false, jump to index 0
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
    uint32_t block_label;
    bool is_while;
    uint32_t while_block_label;
} block_t;


typedef struct {
    block_t** blocks;
    uint32_t block_size;
    Slice *name;
    Slice *parameters;
    uint16_t args;
    struct map* map;
    struct queue_head* queue_head;
} method_t;

method_t* parse_method(struct declare * declare);
struct map* create_small_map();
void create_next_use_information(block_t *block, struct map* map, struct queue_head* queue);
void live_variable_analysis(method_t *method);
uint16_t generate_expression(emitter_t *emitter, expression *expr, uint32_t statement_index, block_t *block, registers_t *reg);

void compile_method(emitter_t *emitter, struct declare *declare);

void function_call_statement(emitter_t *emitter, struct func* func, registers_t *regs, block_t *block, uint32_t statement_index);