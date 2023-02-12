
#include "statement.h"


typedef struct {
    statement** statements;
    uint32_t statement_size;
    bool unconditional_jump;
    expression* jump_expression;
    uint32_t* in_blocks;
    uint32_t in_blocks_index;
    uint32_t in_blocks_size;
    //If conditional expression is true, jump to index 0
    uint32_t* out_blocks;
    uint32_t out_blocks_index;
    uint32_t out_blocks_size;
} block_t;

typedef struct {
    block_t** blocks;
    uint32_t block_size;
    Slice *name;
    Slice *parameters;
    uint16_t args;
} method_t;

method_t* parse_method(struct declare * declare);