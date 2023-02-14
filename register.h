#include <stdint.h>
#include <stdbool.h>

#include "hashmap.h"
#include "block.h"
#include "slice2.h"

#define REGISTER_SIZE 13


void clean_up_block(registers_t* reg);
registers_t* declare_register(block_t* block, struct map* variable_map, emitter_t* emitter);
uint16_t available_registers(registers_t* reg, uint32_t statement_index);
int32_t is_in_register(registers_t *reg, Slice* name);
int16_t get_reg(registers_t *reg, Slice *name);
void set_reg(registers_t *regs, Slice *name, int16_t reg);
void get_available_registers(registers_t *reg, uint16_t *array);
void return_register_to_memory(registers_t *reg, uint16_t i);