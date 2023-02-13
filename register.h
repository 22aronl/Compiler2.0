#include <stdint.h>
#include <stdbool.h>

#include "hashmap.h"

#define REGISTER_SIZE 14

typedef struct {
    struct map* variable_map;
    int32_t* registers;
    bool* used_before;
    int16_t* name_to_register;
    uint32_t variables_index;
    struct var_bin** variables;
    bool* out_blocks_dag;
} registers_t;

