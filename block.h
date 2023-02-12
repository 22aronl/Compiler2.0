
#include "statement.h"


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
void create_next_use_information(block_t *block);