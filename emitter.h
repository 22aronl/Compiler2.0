#include <stdint.h>
#include "slice2.h"
#include "hashmap.h"
#include "main.h"



/**
 * pushes register name onto stack
*/
void push_register(emitter_t*, char* name);

/**
 * pops the top of the stack onto register name
*/
void pop_register(emitter_t*, char* name);

/**
 * emits the instructions to file
*/
void emit(emitter_t*, char* instruction);

/**
 * returns the offset of the variable compared to the base pointer
*/
int32_t get_offset(emitter_t*, Slice* var);

void emit_number(emitter_t* emitter, char* instruction, uint64_t number);

void push_variable(emitter_t* emitter, Slice* var, char* reg);


void emit_start_function(emitter_t* emitter, Slice* name);

void emit_end_function(emitter_t* emitter);

void declare_variable(emitter_t* emitter, Slice* var, int16_t index);

void set_up_assembly(emitter_t* emitter);

void emit_name(emitter_t* emitter, char* instruction, Slice* name);

void emit_string(emitter_t* emitter, char* instruction, char* string);

size_t emit_if_number(emitter_t* emitter);

size_t emit_while_number(emitter_t* emitter);

uint32_t create_label(emitter_t *emitter);

void emit_fix_stack(emitter_t* emitter);

bool align_stack(emitter_t *emitter);

bool align_stack_function(emitter_t *emitter, int16_t offset);

void realign_stack(emitter_t *emitter, bool change);

void add_rsp(emitter_t *emitter, int16_t add_map_offset);

void shift_stack(emitter_t *emitter, int16_t offset);

emitter_t* create_emitter();

void pop_variable(emitter_t *emitter, Slice *var, char *reg);

void emit_number_to_reg(emitter_t *emitter, char *instruction, uint64_t number, char *reg);

void emit_reg_to_reg(emitter_t *emitter, char *instruction, char *reg1, char *reg2);

void pop_variable_to_reg(emitter_t* emitter, uint32_t offset, char* reg);
