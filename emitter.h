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
int16_t get_offset(emitter_t*, Slice* var);

void emit_number(emitter_t* emitter, char* instruction, uint64_t number);

void push_variable(emitter_t* emitter, Slice* var, char* reg);
