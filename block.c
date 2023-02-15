#include <stdlib.h>

#include "block.h"
#include "statement.h"

/**
 * This file was an attempt at building optimizations and does not interact with the rest of the compiler, please to check for code quality :)
*/

/**
 * Checks if the statements array needs to be doubled
*/
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

/**
 * Adds a new block to the block array and returns a new statement array
*/
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

/**
 * Adds a out block to the current block (used for dag analysis)
*/
void add_to_out(block_t *block, uint32_t index)
{
    if (block->out_blocks_index == block->out_blocks_size)
    {
        block->out_blocks_size *= 2;
        block->out_blocks = realloc(block->out_blocks, sizeof(uint32_t) * block->out_blocks_size);
    }
    block->out_blocks[block->out_blocks_index++] = index;
}

/**
 * Adds a in block to the current block (used for dag analysis)
*/
void add_to_in(block_t *block, uint32_t index)
{
    if (block->in_blocks_index == block->in_blocks_size)
    {
        block->in_blocks_size *= 2;
        block->in_blocks = realloc(block->in_blocks, sizeof(uint32_t) * block->in_blocks_size);
    }
    block->in_blocks[block->in_blocks_index++] = index;
}

/**
 * parses the body of a function and creates blocks and stores those blocks into the block_array
*/
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


/**
 * parses a method and its internals and creates a method_t struct
*/
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

    method->blocks = blocks;
    method->block_size = block_index;
    method->name = declare->name;
    method->parameters = declare->parameters;
    method->args = declare->args;
    
    return method;
}