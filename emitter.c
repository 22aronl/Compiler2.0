#include <stdio.h>
#include <string.h>
#include "emitter.h"

void force_reg_write(emitter_t *emitter)
{
    if (emitter->emit_instruction->in_use)
    {
        if (emitter->emit_instruction->pop)
            printf("popq %%%s", emitter->emit_instruction->reg);
        else
            printf("pushq %%%s", emitter->emit_instruction->reg);
        emitter->emit_instruction->in_use = false;
    }
}

void sub_rsp(emitter_t *emitter)
{
    emitter->stack_pointer += 8;
}

void add_rsp(emitter_t *emitter)
{
    emitter->stack_pointer -= 8;
}

bool align_stack(emitter_t *emitter)
{
    if (emitter->stack_pointer % 16 != 0)
    {
        force_reg_write(emitter);
        printf("subq $8, %%rsp\n");
        sub_rsp(emitter);
        return true;
    }
    return false;
}

bool align_stack_function(emitter_t *emitter, int16_t offset)
{
    if ((emitter->stack_pointer + offset * 8) % 16 != 0)
    {
        force_reg_write(emitter);
        printf("subq $8, %%rsp\n");
        sub_rsp(emitter);
        return true;
    }
    return false;
}

void realign_stack(emitter_t *emitter, bool change)
{
    if (change)
    {
        force_reg_write(emitter);
        printf("addq $8, %%rsp\n");
        add_rsp(emitter);
    }
}

void push_register(emitter_t *emitter, char *name)
{
    if (emitter->emit_instruction->in_use)
    {
        if (!strcmp(emitter->emit_instruction->reg, name) && emitter->emit_instruction->pop)
            emitter->emit_instruction->in_use = false;
        else
        {
            emitter->emit_instruction->pop = false;
            emitter->emit_instruction->reg = name;
            printf("pushq %%%s", name);
        }
    }
    else
    {
        printf("pushq %%%s\n", name);
    }
    sub_rsp(emitter);
}

void pop_register(emitter_t *emitter, char *name)
{
    if (emitter->emit_instruction->in_use)
    {
        if (!strcmp(emitter->emit_instruction->reg, name) && !emitter->emit_instruction->pop)
            emitter->emit_instruction->in_use = false;
        else
        {
            emitter->emit_instruction->pop = true;
            emitter->emit_instruction->reg = name;
            printf("popq %%%s\n", name);
        }
    }
    else
    {
        printf("popq %%%s\n", name);
    }
    add_rsp(emitter);
}

void emit(emitter_t *emitter, char *instruction)
{
    force_reg_write(emitter);
    printf("%s\n", instruction);
}

void emit_number(emitter_t *emitter, char *instruction, uint64_t number)
{
    force_reg_write(emitter);
    printf(instruction, number);
    printf("\n");
}

int32_t get_offset(emitter_t *emitter, Slice *var)
{
    int32_t offset = get_map_offset(emitter->var_map, var);
    if (offset == 0)
    {
        force_reg_write(emitter);
        offset = -(emitter->var_offset++) * 8;
        declare_variable(emitter, var, offset);
        emit(emitter, "sub $8, %rsp");
        sub_rsp(emitter);
    }
    return offset;
}

void push_variable(emitter_t *emitter, Slice *var, char *reg)
{
    force_reg_write(emitter);
    printf("movq %%%s, %d(%%rbp)\n", reg, get_offset(emitter, var));
}

void set_up_assembly(emitter_t *emitter)
{
    emit(emitter, "    .data");
    emit(emitter, "format_: .byte '%', 'l', 'u', 10, 0");
    emit(emitter, "    .text");
    emit(emitter, "    .global main");
}

void emit_start_function(emitter_t *emitter, Slice *name)
{
    emit_name(emitter, "%.*s:\n", name);
    emit(emitter, "pushq %rbp");
    emit(emitter, "movq %rsp, %rbp");
    sub_rsp(emitter);
}

void emit_end_function(emitter_t *emitter)
{
    emit(emitter, "movq $0, %rax");
    emit(emitter, "movq %rbp, %rsp");
    emit(emitter, "popq %rbp");
    add_rsp(emitter);
    emit(emitter, "retq");
}

void declare_variable(emitter_t *emitter, Slice *var, int16_t index)
{
    add_map_offset(emitter->var_map, var, index);
}

void emit_name(emitter_t *emitter, char *instruction, Slice *name)
{
    force_reg_write(emitter);
    printf(instruction, name->len, name->start);
}

void emit_string(emitter_t *emitter, char *instruction, char *string)
{
    force_reg_write(emitter);
    printf(instruction, string);
}

size_t emit_if_number(emitter_t *emitter)
{
    return emitter->if_count++;
}

size_t emit_while_number(emitter_t *emitter)
{
    return emitter->while_count++;
}
