#include <stdlib.h>
#include <string.h>

#include "block.h"
#include "statement.h"

statement **double_statements(statement ***statements, uint32_t index, uint32_t *block_size)
{
    if (index == *block_size)
    {
        *block_size *= 2;
        statement **new_statements = realloc(&statements, sizeof(statement *) * *block_size);
        return new_statements;
    }
    return *statements;
}

statement **add_new_block(block_t **blocks, uint32_t *block_index, uint32_t *block_size, statement **cur_statements, uint32_t *cur_statement_index, uint32_t *cur_statement_size)
{
    if (*block_index == *block_size)
    {
        *block_size *= 2;
        blocks = realloc(blocks, sizeof(block_t *) * *block_size);
    }
    blocks[*block_index] = malloc(sizeof(block_t));
    blocks[*block_index]->statements = cur_statements;
    blocks[*block_index]->statement_size = *cur_statement_index;
    blocks[*block_index]->unconditional_jump = true;
    blocks[*block_index]->out_blocks = malloc(sizeof(uint32_t));
    blocks[*block_index]->out_blocks_index = 0;
    blocks[*block_index]->out_blocks_size = 2;
    blocks[*block_index]->in_blocks = malloc(sizeof(uint32_t));
    blocks[*block_index]->in_blocks_index = 0;
    blocks[*block_index]->in_blocks_size = 2;

    (*block_index)++;

    cur_statements = malloc(sizeof(statement *) * 2);
    *cur_statement_size = 2;
    *cur_statement_index = 0;
    return cur_statements;
}

void add_to_out(block_t *block, uint32_t index)
{
    if (block->out_blocks_index == block->out_blocks_size)
    {
        block->out_blocks_size *= 2;
        block->out_blocks = realloc(block->out_blocks, sizeof(uint32_t) * block->out_blocks_size);
    }
    block->out_blocks[block->out_blocks_index++] = index;
}

void add_to_in(block_t *block, uint32_t index)
{
    if (block->in_blocks_index == block->in_blocks_size)
    {
        block->in_blocks_size *= 2;
        block->in_blocks = realloc(block->in_blocks, sizeof(uint32_t) * block->in_blocks_size);
    }
    block->in_blocks[block->in_blocks_index++] = index;
}

void parse_block(statement **body, uint32_t size_body, block_t **block_array, uint32_t *block_index, uint32_t *block_size, block_t *exit_block)
{
    block_t **blocks = block_array;

    statement **cur_statements = malloc(sizeof(statement *) * 2);
    uint32_t cur_statement_size = 2;
    uint32_t cur_statement_index = 0;

    for (uint32_t i = 0; i < size_body; i++)
    {
        statement *s = body[i];
        if (s->type == s_var || s->type == s_print)
        {
            cur_statements = double_statements(&cur_statements, cur_statement_index, &cur_statement_size);
            cur_statements[cur_statement_index++] = s;
        }
        else
        {

            if (s->type == s_return)
            {
                cur_statements = double_statements(&cur_statements, cur_statement_index, &cur_statement_size);
                cur_statements[cur_statement_index++] = s;
            }

            cur_statements = add_new_block(blocks, block_index, block_size, cur_statements, &cur_statement_index, &cur_statement_size);

            if (s->type == s_return)
            {
                add_to_in(exit_block, *block_index - 1);
            }
            else
            {
                if (s->type == s_func)
                {
                    cur_statements[cur_statement_index++] = s;
                    cur_statements = add_new_block(blocks, block_index, block_size, cur_statements, &cur_statement_index, &cur_statement_size);
                }
                else if (s->type == s_if)
                {
                    uint32_t current_block = *block_index - 1;
                    blocks[current_block]->unconditional_jump = false;
                    blocks[current_block]->jump_expression = s->internal->if_statement->condition;
                    
                    parse_block(s->internal->if_statement->body, s->internal->if_statement->size_body, blocks, block_index, block_size, exit_block);
                    uint32_t if_end = *block_index - 1;
                    uint32_t start_after_if = *block_index;

                    if(s->internal->if_statement->has_else) {
                        parse_block(s->internal->if_statement->else_body, s->internal->if_statement->size_else, blocks, block_index, block_size, exit_block);
                        uint32_t else_end = *block_index - 1;

                        add_to_out(blocks[current_block], start_after_if);
                        add_to_out(blocks[if_end], else_end + 1);
                        start_after_if = *block_index;
                        add_to_out(blocks[else_end], start_after_if);
                    }

                    add_to_out(blocks[current_block], current_block + 1);
                    add_to_out(blocks[if_end], start_after_if);
                }
                else if(s->type == s_while) {
                    uint32_t current_block = *block_index - 1;
                    blocks[current_block]->unconditional_jump = false;
                    blocks[current_block]->jump_expression = s->internal->while_statement->condition;

                    parse_block(s->internal->while_statement->body, s->internal->while_statement->size_body, blocks, block_index, block_size, exit_block);
                    uint32_t while_end = *block_index - 1;
                    uint32_t start_after_while = *block_index;

                    add_to_out(blocks[current_block], current_block + 1);
                    add_to_out(blocks[current_block], start_after_while);
                    add_to_out(blocks[while_end], current_block + 1);
                }
            }
        }
    }
}

method_t *parse_method(struct declare *declare)
{
    method_t *method = malloc(sizeof(method_t));

    block_t *exit_block = malloc(sizeof(block_t));
    exit_block->in_blocks = malloc(sizeof(uint32_t));
    exit_block->in_blocks_index = 0;
    exit_block->in_blocks_size = 2;

    block_t** blocks = malloc(sizeof(block_t *) * 2);
    uint32_t block_size = 2;
    uint32_t block_index = 1;

    //The In Block
    blocks[0] = malloc(sizeof(block_t*));
    blocks[0]->out_blocks = malloc(sizeof(uint32_t));
    blocks[0]->out_blocks_index = 0;
    blocks[0]->out_blocks_size = 2;
    blocks[0]->unconditional_jump = true;

    parse_block(declare->body, declare->size_body, blocks, &block_index, &block_size, exit_block);

    for(uint32_t i = 0; i < block_index; i++) {
        for(uint32_t j = 0; j < blocks[i]->out_blocks_index; j++) {
            add_to_in(blocks[blocks[i]->out_blocks[j]], i);
        }
        create_next_use_information(blocks[i]);
    }

    method->blocks = blocks;
    method->block_size = block_index;
    method->name = declare->name;
    method->parameters = declare->parameters;
    method->args = declare->args;

    return method;
}

void add_to_variables(block_t* block, int32_t input, int32_t hash) {
    if(hash == block->variables_index) {
        if(block->variables_index == block->variables_size) {
            block->variables_size *= 2;
            block->variables = realloc(block->variables, sizeof(struct var_bin*) * block->variables_size);
        }
        block->variables[hash] = malloc(sizeof(struct var_bin));
        block->variables[hash]->vars = malloc(sizeof(int32_t) * 2);
        block->variables[hash]->index = 0;
        block->variables[hash]->size = 2;
    }

    if(block->variables[hash]->index == block->variables[hash]->size) {
        block->variables[hash]->size *= 2;
        block->variables[hash]->vars = realloc(block->variables[hash]->vars, sizeof(int32_t) * block->variables[hash]->size);
    }

    block->variables[hash]->vars[block->variables[hash]->index++] = input;
}

void add_to_next_use(block_t* block, Slice* name, int32_t state_index) {
    int32_t index = get_map_offset(block->variable_map, name);
    if(index == 0) {
        index = block->variables_index + 1;
        add_map_offset(block->variable_map, name, index);
    }
    add_to_variables(block, state_index, index);
}

void next_use_expression(expression* expr, block_t* block, int32_t state_index) {
    switch(expr->type) {
        case t_var:
            add_to_next_use(block, expr->character->name, state_index);
            break;
        case t_func:
            for(uint32_t i = 0; i < expr->character->function->args; i++) {
                next_use_expression(expr->character->function->parameters[i], block, state_index);
            }
            break;
        case t_print:
            next_use_expression(expr->left, block, state_index);
            break;
        case t_not:
            next_use_expression(expr->left, block, state_index);
            break;
        case t_num:
            break;
        default:
            next_use_expression(expr->left, block, state_index);
            next_use_expression(expr->right, block, state_index);
            break;
    }
}

void next_use_statement(statement* state, block_t* block, int32_t state_index) {
    switch(state->type) {
        case s_func:
            for(uint32_t i = 0; i < state->internal->func->args; i++) {
                next_use_expression(state->internal->func->parameters[i], block, state_index);
            }
            break;
        case s_var:
            next_use_expression(state->internal->var->expr, block, state_index);
            add_to_next_use(block, state->internal->var->name, -state_index);
            break;
        case s_print:
            next_use_expression(state->internal->print->expr, block, state_index);
            break;
        case s_return:
            next_use_expression(state->internal->return_statement->expr, block, state_index);
            break;
        default:
            printf("NEXT USE STATEMENT THAT IS NOT IMPLEMENTED");
            break;
    }
}

void create_next_use_information(block_t *block) {
    block->variable_map = create_small_map();
    block->variables = malloc(sizeof(int32_t*) * 2);
    block->variables_size = 2;
    block->variables_index = 0;

    if(!block->unconditional_jump) {
        next_use_expression(block->jump_expression, block, block->statement_size);
    }

    int32_t i = block->statement_size;
    for(; i > 0; i--) {
        statement* s = block->statements[i - 1];
        next_use_statement(s, block, i - 1);
    }
    
}
struct map* create_small_map() {
    struct map *map = malloc(sizeof(struct map));
    map->map = malloc(20 * sizeof(hash_map));
    map->visited = malloc(20 * sizeof(bool));
    memset(map->visited, 0, 20 * sizeof(bool));
    map->size = 20;
    map->main = false;
    return map;
}