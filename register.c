#include <stdlib.h>
#include <string.h>
#include "register.h"
#include "block.h"
#include "slice2.h"

registers_t declare_register_t(block_t* block) {
    registers_t* reg = malloc(sizeof(registers_t));
    reg->variable_map = block->variable_map;
    reg->registers = malloc(sizeof(int32_t) * REGISTER_SIZE);
    for(int i = 0; i < REGISTER_SIZE; i++) {
        reg->registers[i] = -1;
    }
    reg->variables_index = block->variables_index;
    reg->name_to_register = malloc(sizeof(int16_t) * block->variables_index);
    memset(reg->name_to_register, -1, sizeof(int16_t) * block->variables_index);

    reg->variables = block->variables;
    reg->out_blocks_dag = block->out_blocks_dag;

    return *reg;
}

int getReg(registers_t* reg, Slice* name) {
    int32_t name_id = get_map_offset(reg->variable_map, name);

    if(reg->name_to_register[name_id] != -1) {
        return reg->name_to_register[name_id];
    }

    for(int i = 0; i < REGISTER_SIZE; i++) {
        if(reg->registers[i] == -1) {
            reg->registers[i] = name_id;
            reg->name_to_register[name_id] = i;
            return i;
        }
    }

    return 0;
}

