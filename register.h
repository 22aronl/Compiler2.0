#include <stdint.h>
#include <stdbool.h>

#include "hashmap.h"
#include "block.h"
#include "slice2.h"

#define REGISTER_SIZE 14



registers_t* declare_register(block_t* block);
uint16_t available_registers(registers_t* reg, uint32_t statement_index);