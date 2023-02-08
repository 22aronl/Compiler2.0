#include "emitter.h"

void push_register(emitter_t* emitter, char* name)
{
    printf("pushq \%%s\n", name);
}

void pop_register(emitter_t* emitter,char* name)
{
    printf("popq \%%s\n", name);
}

void emit(emitter_t* emitter, char* instruction)
{
    printf("%s\n", instruction);
}

void emit_number(emitter_t* emitter, char* instruction, uint64_t number)
{
    printf(instruction, number);
}

int16_t get_offset(emitter_t* emitter, Slice* var)
{
    return get_map_offset(emitter->var_map, var);
}
