#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "statement.h"
#include "main.h"
#include "hashmap.h"
#include "expression.h"
#include "emitter.h"

/**
 * Creates an interanl map to be used in a function
 */
struct map *create_map()
{
    struct map *map = malloc(sizeof(struct map));
    map->map = malloc(MAX_SYMBOLS_FOR_FUNC * sizeof(hash_map));
    map->visited = malloc(MAX_SYMBOLS_FOR_FUNC * sizeof(bool));
    memset(map->visited, 0, MAX_SYMBOLS_FOR_FUNC * sizeof(bool));
    map->size = MAX_SYMBOLS_FOR_FUNC;
    map->main = false;
    return map;
}

/**
 * Frees all the memory of the map
 */
void free_map(struct map *map)
{
    for (uint32_t i = 0; i < map->size; i++)
    {
        if (map->visited[i])
        {
            free(map->map[i].bins);
        }
    }
}

void find_variables(emitter_t *emitter, statement *s)
{
    if (s->type == s_var)
    {
        int32_t offset = get_map_offset(emitter->var_map, s->internal->var->name);
        if (offset == 0)
        {
            offset = -(emitter->var_offset++) * 8;
            declare_variable(emitter, s->internal->var->name, offset);
            emit(emitter, "sub $8, %rsp");
            sub_rsp(emitter);
        }
    }
    else if (s->type == s_if)
    {
        for (uint32_t i = 0; i < s->internal->if_statement->size_body; i++)
        {
            find_variables(emitter, s->internal->if_statement->body[i]);
        }

        if (s->internal->if_statement->has_else)
        {
            for (uint32_t i = 0; i < s->internal->if_statement->size_else; i++)
            {
                find_variables(emitter, s->internal->if_statement->else_body[i]);
            }
        }
    }
    else if (s->type == s_while)
    {
        for (uint32_t i = 0; i < s->internal->while_statement->size_body; i++)
        {
            find_variables(emitter, s->internal->while_statement->body[i]);
        }
    }
}

void declare_function(emitter_t *emitter, struct declare *declare)
{

    struct map *new_map = create_map();
    struct map *old_map = emitter->var_map;
    uint16_t old_offset = emitter->var_offset;
    uint64_t old_rsp = emitter->stack_pointer;
    emitter->var_map = new_map;
    emitter->var_offset = 1;
    emitter->stack_pointer = 8;
    emit_start_function(emitter, declare->name);

    for (uint16_t i = 0; i < declare->args; i++)
    {
        declare_variable(emitter, &declare->parameters[i], (declare->args - i - 1) * 8 + 16);
    }

    for (uint32_t i = 0; i < declare->size_body; i++)
    {
        find_variables(emitter, declare->body[i]);
    }

    for (uint32_t i = 0; i < declare->size_body; i++)
    {
        compile_statement(emitter, declare->body[i], declare);
    }

    emitter->var_offset = old_offset;
    emitter->var_map = old_map;
    emitter->stack_pointer = old_rsp;

    free_map(new_map);
    free(new_map->map);
    free(new_map->visited);
    free(new_map);

    emit_end_function(emitter);
}

void call_function(emitter_t *emitter, struct func *func)
{
    bool change = align_stack_function(emitter, func->args);
    for (uint16_t i = 0; i < func->args; i++)
    {
        compile_expression(emitter, func->parameters[i]);
    }
    emit_name(emitter, "call %.*s\n", func->name);

    for (uint16_t i = 0; i < func->args; i++)
    {
        pop_register(emitter, "rbx");
    }
    realign_stack(emitter, change);
}

void compile_statement(emitter_t *emitter, statement *s, struct declare *declare)
{
    switch (s->type)
    {
    case s_declare:
        declare_function(emitter, s->internal->declare);
        break;
    case s_func:
        call_function(emitter, s->internal->func);
        break;
    case s_var:
        compile_expression(emitter, s->internal->var->expr);
        pop_register(emitter, "rax");
        push_variable(emitter, s->internal->var->name, "rax");
        break;
    case s_print:
        compile_expression(emitter, s->internal->print->expr);
        pop_register(emitter, "rax");
        emit(emitter, "mov %rax, %rsi");
        emit(emitter, "mov $0, %rax");

        bool change = align_stack(emitter);
        emit(emitter, "lea format_(%rip),%rdi");
        emit(emitter, ".extern printf");
        emit(emitter, "call printf");
        realign_stack(emitter, change);
        break;
    case s_if:
    {
        compile_expression(emitter, s->internal->if_statement->condition);
        pop_register(emitter, "rax");
        emit(emitter, "cmp $0, %rax");
        size_t if_number = emit_if_number(emitter);
        emit_number(emitter, "je else%d_", if_number);
        for (uint32_t i = 0; i < s->internal->if_statement->size_body; i++)
            compile_statement(emitter, s->internal->if_statement->body[i], declare);
        if (s->internal->if_statement->has_else)
        {
            emit_number(emitter, "jmp endif%d_", if_number);
            emit_number(emitter, "else%d_:", if_number);
            for (uint32_t i = 0; i < s->internal->if_statement->size_else; i++)
                compile_statement(emitter, s->internal->if_statement->else_body[i], declare);
            emit_number(emitter, "endif%d_:", if_number);
        }
        else
        {
            emit_number(emitter, "else%d_:", if_number);
        }
        break;
    }
    case s_while:
    {
        size_t while_number = emit_while_number(emitter);
        emit_number(emitter, "while%d_:", while_number);
        compile_expression(emitter, s->internal->while_statement->condition);
        pop_register(emitter, "rax");
        emit(emitter, "cmp $0, %rax");
        emit_number(emitter, "je endwhile%d_", while_number);
        for (uint32_t i = 0; i < s->internal->while_statement->size_body; i++)
            compile_statement(emitter, s->internal->while_statement->body[i], declare);
        emit_number(emitter, "jmp while%d_", while_number);
        emit_number(emitter, "endwhile%d_:", while_number);
        break;
    }
    case s_return:
        if (s->internal->return_statement->expr->type == t_func && slice_eq2(s->internal->return_statement->expr->character->function->name, declare->name))
        {
            for (uint16_t i = 0; i < s->internal->return_statement->expr->character->function->args; i++)
            {
                compile_expression(emitter, s->internal->return_statement->expr->character->function->parameters[i]);
                pop_register(emitter, "rax");
                push_variable(emitter, &declare->parameters[i], "rax");
            }
            emit(emitter, "movq %rbp, %rsp");
            emit(emitter, "popq %rbp");
            emit_name(emitter, "jmp %.*s\n", declare->name);
            break;
        }
        compile_expression(emitter, s->internal->return_statement->expr);
        pop_register(emitter, "rax");
        emit(emitter, "movq %rbp, %rsp");
        emit(emitter, "popq %rbp");
        add_rsp(emitter);
        emit(emitter, "retq");
        break;
    }
}

/**
 * Frees a function struct of all its memory
 */
void free_function(struct func *func)
{
    free(func->name);
    for (uint16_t i = 0; i < func->args; i++)
        free_expression(func->parameters[i]);
    free(func->parameters);
    free(func);
}

/**
 * frees a statement struct of all its memory (recursively)
 */
void free_statement(statement *s)
{
    switch (s->type)
    {
    case s_declare:
        free(s->internal->declare->name);
        free(s->internal->declare->parameters);
        for (uint32_t i = 0; i < s->internal->declare->size_body; i++)
            free_statement(s->internal->declare->body[i]);
        free(s->internal->declare->body);
        free(s->internal->declare);
        break;
    case s_func:
        free_function(s->internal->func);
        break;
    case s_var:
        free_expression(s->internal->var->expr);
        free(s->internal->var->name);
        free(s->internal->var);
        break;
    case s_print:
        free_expression(s->internal->print->expr);
        free(s->internal->print);
        break;
    case s_if:
        free_expression(s->internal->if_statement->condition);
        for (uint32_t i = 0; i < s->internal->if_statement->size_body; i++)
            free_statement(s->internal->if_statement->body[i]);
        free(s->internal->if_statement->body);
        if (s->internal->if_statement->has_else)
        {
            for (uint32_t i = 0; i < s->internal->if_statement->size_else; i++)
                free_statement(s->internal->if_statement->else_body[i]);
            free(s->internal->if_statement->else_body);
        }
        free(s->internal->if_statement);
        break;
    case s_while:
        free_expression(s->internal->while_statement->condition);
        for (uint32_t i = 0; i < s->internal->while_statement->size_body; i++)
            free_statement(s->internal->while_statement->body[i]);
        free(s->internal->while_statement->body);
        free(s->internal->while_statement);
        break;
    case s_return:
        free_expression(s->internal->return_statement->expr);
        free(s->internal->return_statement);
        break;
    }
    free(s->internal);
    free(s);
}

/**
 * Recursively evaluates a function by loading all the parameters into a new map and then evaluating the body
 */
uint64_t evaluate_function(Interpreter *in, struct map *map, struct func *func, struct declare *declare)
{
    struct map *new_map = create_map();

    for (uint16_t i = 0; i < declare->args; i++)
    {
        addSymbol(in, new_map, &declare->parameters[i], eval_expr(in, map, func->parameters[i]), true);
    }
    uint64_t return_value = 0;
    for (uint32_t i = 0; i < declare->size_body; i++)
    {
        if (!evaluate_statement(in, new_map, declare->body[i], &return_value))
            break;
    }
    free_map(new_map);
    free(new_map->map);
    free(new_map->visited);
    free(new_map);
    return return_value;
}

/**
 * Evaluates a statement by calling the correct function
 */
bool evaluate_statement(Interpreter *in, struct map *map, statement *s, uint64_t *return_value)
{
    switch (s->type)
    {
    case s_declare:
        add_function(in, s->internal->declare);
        break;
    case s_func:
        evaluate_function(in, map, s->internal->func, get_function(in, s->internal->func->name));
        break;
    case s_var:
        addSymbol(in, map, s->internal->var->name, eval_expr(in, map, s->internal->var->expr), false);
        break;
    case s_print:
        printf("%" PRIu64 "\n", eval_expr(in, map, s->internal->print->expr));
        break;
    case s_if:
        if (eval_expr(in, map, s->internal->if_statement->condition)) // Code evaluation for if statement
        {
            for (uint32_t i = 0; i < s->internal->if_statement->size_body; i++)
                if (!evaluate_statement(in, map, s->internal->if_statement->body[i], return_value)) // If there is a return value, it should terimnate out
                    return false;
        }
        else if (s->internal->if_statement->has_else)
        {
            for (uint32_t i = 0; i < s->internal->if_statement->size_else; i++)
                if (!evaluate_statement(in, map, s->internal->if_statement->else_body[i], return_value))
                    return false;
        }
        break;
    case s_while:
        while (eval_expr(in, map, s->internal->while_statement->condition))
            for (uint32_t i = 0; i < s->internal->while_statement->size_body; i++)
                if (!evaluate_statement(in, map, s->internal->while_statement->body[i], return_value)) // If there is a return value, it should terimnate out
                    return false;
        break;
    case s_return:
        *return_value = eval_expr(in, map, s->internal->return_statement->expr);
        return false;
    }
    return true;
}
