#include <stdio.h>
#include <stdlib.h>

#include "emitter.h"

void sub_rsp(emitter_t *emitter, int16_t offset)
{
    emitter->stack_pointer += offset * 8;
}

void add_rsp(emitter_t *emitter, int16_t offset)
{
    emitter->stack_pointer -= offset * 8;
}

bool align_stack(emitter_t *emitter)
{
    if (emitter->stack_pointer % 16 != 0)
    {
        printf("subq $8, %%rsp\n");
        sub_rsp(emitter, 1);
        return true;
    }
    return false;
}

bool align_stack_function(emitter_t *emitter, int16_t offset)
{
    if ((emitter->stack_pointer + offset * 8) % 16 != 0)
    {
        printf("subq $8, %%rsp\n");
        sub_rsp(emitter, 1);
        return true;
    }
    return false;
}

void realign_stack(emitter_t *emitter, bool change)
{
    if (change)
    {
        printf("addq $8, %%rsp\n");
        add_rsp(emitter, 1);
    }
}

void push_register(emitter_t *emitter, char *name)
{
    printf("pushq %%%s\n", name);
    sub_rsp(emitter, 1);;
}

void pop_register(emitter_t *emitter, char *name)
{
    printf("popq %%%s\n", name);
    add_rsp(emitter, 1);
}

void emit(emitter_t *emitter, char *instruction)
{
    printf("%s\n", instruction);
}

void emit_number(emitter_t *emitter, char *instruction, uint64_t number)
{
    printf(instruction, number);
    printf("\n");
}

void emit_number_to_reg(emitter_t *emitter, char *instruction, uint64_t number, char *reg)
{
    printf(instruction, number, reg);
    printf("\n");
}

void emit_reg_to_reg(emitter_t *emitter, char *instruction, char *reg1, char *reg2)
{
    printf(instruction, reg1, reg2);
    printf("\n");
}

int32_t get_offset(emitter_t *emitter, Slice *var)
{
    int32_t offset = get_map_offset(emitter->var_map, var);
    if (offset == 0)
    {
        offset = -(emitter->var_offset++) * 8;
        declare_variable(emitter, var, offset);
        emit(emitter, "sub $8, %rsp");
        sub_rsp(emitter, 1);
    }
    return offset;
}

void push_variable(emitter_t *emitter, Slice *var, char *reg)
{
    printf("movq %%%s, %d(%%rbp)\n", reg, get_offset(emitter, var));
}

//makes sure the stack is aligned up to the stored rbp location
void emit_fix_stack(emitter_t* emitter)
{
    if(emitter->stack_pointer != 8)
    {
        shift_stack(emitter, - emitter->stack_pointer / 8 + 1);
    }
}

void pop_variable(emitter_t *emitter, Slice *var, char *reg)
{
    printf("movq %d(%%rbp), %%%s\n", get_offset(emitter, var), reg);
}

void set_up_assembly(emitter_t *emitter)
{
    emit(emitter, "    .data");
    emit(emitter, "format: .byte '%', 'l', 'u', 10, 0");
    emit(emitter, "    .text");
    emit(emitter, "    .global main");
}

void emit_start_function(emitter_t *emitter, Slice *name)
{
    emit_name(emitter, "%.*s:\n", name);
    emit(emitter, "pushq %rbp");
    emit(emitter, "movq %rsp, %rbp");
    sub_rsp(emitter, 1);
}

void emit_end_function(emitter_t *emitter)
{
    emit(emitter, "movq $0, %rax");
    emit(emitter, "movq %rbp, %rsp");
    emit(emitter, "popq %rbp");
    add_rsp(emitter, 1);
    emit(emitter, "retq");
}

void shift_stack(emitter_t *emitter, int16_t offset)
{
    if (offset > 0)
    {
        emit_number(emitter, "subq $%d, %%rsp", offset * 8);
        sub_rsp(emitter, offset);
    }
    else if (offset < 0)
    {
        emit_number(emitter, "addq $%d, %%rsp", -offset * 8);
        add_rsp(emitter, -offset);
    }
}

void declare_variable(emitter_t *emitter, Slice *var, int16_t index)
{
    add_map_offset(emitter->var_map, var, index);
}

void emit_name(emitter_t *emitter, char *instruction, Slice *name)
{
    printf(instruction, name->len, name->start);
}

void emit_string(emitter_t *emitter, char *instruction, char *string)
{
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

emitter_t* create_emitter() {
    emitter_t *em = malloc(sizeof(emitter_t));
    em->if_count = 0;
    em->while_count = 0;
    em->stack_pointer = 0;
    em->registers = malloc(sizeof(char*) * 13);
    em->registers[0] = "r15";
    em->registers[1] = "r14";
    em->registers[2] = "r13";
    em->registers[3] = "r12";
    em->registers[4] = "r11";
    em->registers[5] = "r10";
    em->registers[6] = "r9";
    em->registers[7] = "r8";
    em->registers[8] = "rcx";
    em->registers[9] = "rbx";
    em->registers[10] = "rsi";
    em->registers[11] = "rdi";
    em->registers[12] = "rdx";
    return em;
}