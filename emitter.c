#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "emitter.h"

uint32_t create_label(emitter_t *emitter)
{
    return emitter->label_count++;
}

/**
 * This is for peephole optimization, and it attempts to combine instructions if there is a push and pop to
 * the same register next to each other
*/
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

/**
 * Subtracts 8 from the stack pointer
*/
void sub_rsp(emitter_t *emitter, int16_t offset)
{
    emitter->stack_pointer += offset * 8;
}

void add_rsp(emitter_t *emitter, int16_t offset)
{
    emitter->stack_pointer -= offset * 8;
}

/**
 * Checks if the stack needs to be aligned before a function all
*/
bool align_stack(emitter_t *emitter)
{
    if (emitter->stack_pointer % 16 != 0)
    {
        force_reg_write(emitter);
        printf("subq $8, %%rsp\n");
        sub_rsp(emitter, 1);
        return true;
    }
    return false;
}

/**
 * Checks if the stack needs to be aligned before a function call takign in the number of arguments
*/
bool align_stack_function(emitter_t *emitter, int16_t offset)
{
    if ((emitter->stack_pointer + offset * 8) % 16 != 0)
    {
        force_reg_write(emitter);
        printf("subq $8, %%rsp\n");
        sub_rsp(emitter, 1);
        return true;
    }
    return false;
}

/**
 * Realigns the stack if the align stack+fuctino nad moved it
*/
void realign_stack(emitter_t *emitter, bool change)
{
    if (change)
    {
        force_reg_write(emitter);
        printf("addq $8, %%rsp\n");
        add_rsp(emitter, 1);
    }
}

/**
 * This pushes a register to stack
*/
void push_register(emitter_t *emitter, char *name)
{
    if (emitter->emit_instruction->in_use) //chcekc for peephole
    {
        if (!strcmp(emitter->emit_instruction->reg, name) && emitter->emit_instruction->pop)
            emitter->emit_instruction->in_use = false;
        else
        {
            emitter->emit_instruction->pop = false;
            emitter->emit_instruction->reg = name;
            printf("pushq %%%s", name); //TODO: WARNING THIS WIILL RUN INTO MAJOR ISSUES ON NOT STACK COMPILER
        }
    }
    else
    {
        printf("pushq %%%s\n", name);
    }
    sub_rsp(emitter, 1);;
}

/**
 * This pops the register from the stack
*/
void pop_register(emitter_t *emitter, char *name)
{
    if (emitter->emit_instruction->in_use) //peephold
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
    add_rsp(emitter, 1);
}

/**
 * emits the instruction to output
*/
void emit(emitter_t *emitter, char *instruction)
{
    force_reg_write(emitter);
    printf("%s\n", instruction);
}

/**
 * emits a nubmer with the given instruction to output
*/
void emit_number(emitter_t *emitter, char *instruction, uint64_t number)
{
    force_reg_write(emitter);
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



/**
 * this gets to offset the variable is to the rbp
*/
int32_t get_offset(emitter_t *emitter, Slice *var)
{
    int32_t offset = get_map_offset(emitter->var_map, var);
    if (offset == 0)
    {
        force_reg_write(emitter);
        offset = -(emitter->var_offset++) * 8;
        declare_variable(emitter, var, offset);
        emit(emitter, "sub $8, %rsp");
        sub_rsp(emitter, 1);
    }
    return offset;
}

/**
 * This pushes a register to memory of the variable name var
*/
void push_variable(emitter_t *emitter, Slice *var, char *reg)
{
    force_reg_write(emitter);
    printf("movq %%%s, %d(%%rbp)\n", reg, get_offset(emitter, var));
}

//makes sure the stack is aligned up to the stored rbp location
void emit_fix_stack(emitter_t* emitter)
{
    if(emitter->stack_pointer != 8)
    {
        shift_stack(emitter, -emitter->stack_pointer / 8 + 2);
    }
}

void pop_variable(emitter_t *emitter, Slice *var, char *reg)
{
    printf("movq %d(%%rbp), %%%s\n", get_offset(emitter, var), reg);
}

void pop_variable_to_reg(emitter_t* emitter, uint32_t offset, char* reg)
{
    printf("movq -%d(%%rbp), %%%s\n", offset, reg);
}



/**
 * sets up the assembly starter
*/
void set_up_assembly(emitter_t *emitter)
{
    emit(emitter, "    .data");
    emit(emitter, "format_: .byte '%', 'l', 'u', 10, 0");
    emit(emitter, "    .text");
    emit(emitter, "    .global main");
}

/**
 * This starts the function code in cassembly
*/
void emit_start_function(emitter_t *emitter, Slice *name)
{
    emit_name(emitter, "%.*s:\n", name);
    emit(emitter, "pushq %rbp");
    emit(emitter, "movq %rsp, %rbp");
    sub_rsp(emitter, 1);
}

/**
 * This wraps up the function
*/
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
 
/**
 * This declares a variable in the hashmap being used
*/
void declare_variable(emitter_t *emitter, Slice *var, int16_t index)
{
    add_map_offset(emitter->var_map, var, index);
}

/**
 * This emitss the name of the variable at the instructions request
*/
void emit_name(emitter_t *emitter, char *instruction, Slice *name)
{
    force_reg_write(emitter);
    printf(instruction, name->len, name->start);
}

/**
 * This emits a string with the given instruction
*/
void emit_string(emitter_t *emitter, char *instruction, char *string)
{
    force_reg_write(emitter);
    printf(instruction, string);
}

/**
 * This emits the number of the if statement, used for unique labels
*/
size_t emit_if_number(emitter_t *emitter)
{
    return emitter->if_count++;
}

/**
 * This emits the number of the while statement, used for unique labels
*/
size_t emit_while_number(emitter_t *emitter)
{
    return emitter->while_count++;
}

emitter_t* create_emitter() {
    emitter_t *em = malloc(sizeof(emitter_t));
    em->if_count = 0;
    em->while_count = 0;
    em->stack_pointer = 0;
    em->label_count = 0;
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