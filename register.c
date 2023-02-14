#include <stdlib.h>
#include <string.h>
#include "register.h"
#include "block.h"
#include "slice2.h"
#include "emitter.h"

registers_t *declare_register(block_t *block, struct map *variable_map, emitter_t *emitter)
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
    reg->function_variable_map = variable_map;
    reg->emitter = emitter;

    return reg;
}

bool is_register_needed(registers_t *reg, int16_t statement_index, uint16_t variable_index)
{

    for (uint32_t i = reg->variables[variable_index]->index; i > 0; i--)
    {
        int32_t var = reg->variables[variable_index]->vars[i - 1];
        if (var >= statement_index)
        {
            return true;
        }
        else if (var < 0 && -var > statement_index)
        {
            return false;
        }
    }

    return false;
}

void return_register_to_memory(registers_t *reg, uint16_t register_index)
{
    push_variable(reg->emitter, reg->name_array[register_index], reg->emitter->registers[register_index]);
}

void clean_up_block(registers_t *reg)
{
    for (uint16_t i = 0; i < REGISTER_SIZE; i++)
    {
        if (reg->registers[i] != -1)
        // if (reg->registers[i] != -1 && reg->out_blocks_dag[reg->registers[i]])
        {
            return_register_to_memory(reg, reg->registers[i]);
            reg->registers[i] = -1;
        }
    }
}

void get_available_registers(registers_t *reg, uint16_t *array)
{
    uint16_t index = 0;
    for (uint16_t i = 0; i < REGISTER_SIZE; i++)
    {
        if (reg->registers[i] == -1)
        {
            array[index++] = i;
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
        else if (!is_register_needed(reg, statement_index, reg->registers[i]))
        {
            // reg->registers[i] = -1;
            return_register_to_memory(reg, i);
            k++;
        }
    }

    return k;
}

int32_t is_in_register(registers_t *reg, Slice *name)
{
    int32_t name_id = get_map_offset(reg->variable_map, name);

    if (reg->name_to_register[name_id] != -1)
    {
        return reg->name_to_register[name_id];
    }

    return -1;
}

void set_reg(registers_t *regs, Slice *name, int16_t reg)
{
    int32_t name_id = get_map_offset(regs->variable_map, name);
    regs->registers[reg] = name_id;
    regs->name_array[name_id] = name;
    regs->name_to_register[name_id] = reg;
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
