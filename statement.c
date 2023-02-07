#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "statement.h"
#include "main.h"
#include "hashmap.h"
#include "expression.h"

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
void free_map(struct map* map) {
    for(uint32_t i = 0; i < map->size; i++) {
        if(map->visited[i]) {
            free(map->map[i].bins);
        }
    }
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
        if (eval_expr(in, map, s->internal->if_statement->condition)) //Code evaluation for if statement
        {
            for (uint32_t i = 0; i < s->internal->if_statement->size_body; i++)
                if (!evaluate_statement(in, map, s->internal->if_statement->body[i], return_value)) //If there is a return value, it should terimnate out
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
                if (!evaluate_statement(in, map, s->internal->while_statement->body[i], return_value)) //If there is a return value, it should terimnate out
                    return false;
        break;
    case s_return:
        *return_value = eval_expr(in, map, s->internal->return_statement->expr);
        return false;
    }
    return true;
}