#include <stdio.h>

#include "emitter.h"

void sub_rsp(emitter_t * emitter) {
    emitter->stack_pointer += 8;
}

void add_rsp(emitter_t * emitter) {
    emitter->stack_pointer -= 8;
}

void align_stack(emitter_t *emitter)
{
    if (emitter->stack_pointer % 16 != 0)
    {
        printf("subq $8, %%rsp\n");
    }
}

void push_register(emitter_t *emitter, char *name)
{
    printf("pushq %%%s\n", name);
    sub_rsp(emitter);
}

void pop_register(emitter_t *emitter, char *name)
{
    printf("popq %%%s\n", name);
    add_rsp(emitter);
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

int32_t get_offset(emitter_t *emitter, Slice *var)
{
    int32_t offset = get_map_offset(emitter->var_map, var);
    if (offset == 0)
    {
        offset = -(emitter->var_offset++)*8;
        declare_variable(emitter, var, offset);
    }
    return offset;
}

void push_variable(emitter_t *emitter, Slice *var, char *reg)
{
    printf("movq %%%s, %d(%%rbp)\n", reg, get_offset(emitter, var));
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
    sub_rsp(emitter);
}

void emit_end_function(emitter_t *emitter)
{
    // emit(emitter, "movq %rbp %rsp");
    // emit(emitter, "popq %rbp");
    emit(emitter, "retq");
}

void declare_variable(emitter_t *emitter, Slice *var, int16_t index)
{
    add_map_offset(emitter->var_map, var, index);
    emit(emitter, "sub $8, %rsp");
    sub_rsp(emitter);
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
