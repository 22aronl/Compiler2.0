#include <stdlib.h>
#include <string.h>
#include "register.h"
#include "block.h"
#include "slice2.h"

registers_t *declare_register(block_t *block)
{
    registers_t *reg = malloc(sizeof(registers_t));
    reg->variable_map = block->variable_map;
    reg->registers = malloc(sizeof(int32_t) * REGISTER_SIZE);
    for (int i = 0; i < REGISTER_SIZE; i++)
    {
        reg->registers[i] = -1;
    }
    reg->variables_index = block->variables_index;
    reg->name_to_register = malloc(sizeof(int16_t) * block->variables_index);
    memset(reg->name_to_register, -1, sizeof(int16_t) * block->variables_index);
    reg->name_array = malloc(sizeof(Slice *) * block->variables_index);

    reg->variables = block->variables;
    reg->out_blocks_dag = block->out_blocks_dag;

    return reg;
}

bool is_register_needed(registers_t *reg, int16_t statement_index, uint16_t variable_index)
{
    for (uint32_t i = reg->variables[variable_index]->index; i >= 0; i--)
    {
        if (reg->variables[variable_index]->vars[i] > statement_index)
        {
            return true;
        }
        else if (reg->variables[variable_index]->vars[i] > -statement_index)
        {
            return false;
        }
    }

    return false;
}

void return_register_to_memory(registers_t *reg, uint16_t variable_index)
{
    // TODO:
}

void get_available_registers(registers_t *reg, uint16_t *array)
{
    uint16_t index = 0;
    for (uint16_t i = 0; i < REGISTER_SIZE; i++)
    {
        if (reg->registers[i] == -1)
        {
            array[index] = i;
        }
    }
}

uint16_t available_registers(registers_t *reg, uint32_t statement_index)
{
    uint16_t k = 0;
    for (uint16_t i = 0; i < REGISTER_SIZE; i++)
    {
        if (reg->registers[i] == -1)
        {
            k++;
        }
    }

    for (uint16_t i = 0; i < reg->variables_index; i++)
    {
        if (!is_register_needed(reg, statement_index, i))
        {
            reg->registers[i] = -1;
            return_register_to_memory(reg, i);
            k++;
        }
    }

    return k;
}

int32_t is_in_register(registers_t *reg, Slice* name)
{
    int32_t name_id = get_map_offset(reg->variable_map, name);

    if (reg->name_to_register[name_id] != -1)
    {
        return reg->name_to_register[name_id];
    }

    return -1;
}



int16_t get_reg(registers_t *reg, Slice *name)
{
    int32_t name_id = get_map_offset(reg->variable_map, name);

    if (reg->name_to_register[name_id] != -1)
    {
        return reg->name_to_register[name_id];
    }

    for (int16_t i = 0; i < REGISTER_SIZE; i++)
    {
        if (reg->registers[i] == -1)
        {
            reg->registers[i] = name_id;
            reg->name_array[name_id] = name;
            reg->name_to_register[name_id] = i;
            return i;
        }
    }

    // TODO: if we have register spilling DDDD:

    return 0;
}