#include <stdlib.h>
#include <string.h>

#include "block.h"
#include "statement.h"
#include "expression.h"
#include "emitter.h"
#include "register.h"
#include "hashmap.h"

statement **double_statements(statement **statements, uint32_t index, uint32_t *block_size)
{
    if (index == *block_size)
    {
        *block_size *= 2;
        statement **new_statements = realloc(statements, sizeof(statement *) * *block_size);
        return new_statements;
    }
    return statements;
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
    blocks[*block_index]->has_jump = false;
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
        // REMOVE PRINT STATEMENTS
        if (s->type == s_var || s->type == s_print)
        {
            if (cur_statement_index == cur_statement_size)
            {
                cur_statements = realloc(cur_statements, sizeof(statement *) * cur_statement_size * 2);
                cur_statement_size *= 2;
            }
            cur_statements[cur_statement_index++] = s;
        }
        else
        {

            if (s->type == s_return)
            {
                if (cur_statement_index == cur_statement_size)
                {
                    cur_statements = realloc(cur_statements, sizeof(statement *) * cur_statement_size * 2);
                    cur_statement_size *= 2;
                }
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
                    // EVALUATE THE PARAMETERS
                    cur_statements[cur_statement_index++] = s;
                    cur_statements = add_new_block(blocks, block_index, block_size, cur_statements, &cur_statement_index, &cur_statement_size);
                }
                else if (s->type == s_if)
                {
                    uint32_t current_block = *block_index - 1;
                    blocks[current_block]->has_jump = true;
                    blocks[current_block]->unconditional_jump = false;
                    blocks[current_block]->jump_expression = s->internal->if_statement->condition;

                    parse_block(s->internal->if_statement->body, s->internal->if_statement->size_body, blocks, block_index, block_size, exit_block);
                    uint32_t if_end = *block_index - 1;
                    uint32_t start_after_if = *block_index;

                    if (s->internal->if_statement->has_else)
                    {
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
                else if (s->type == s_while)
                {
                    uint32_t current_block = *block_index - 1;
                    blocks[current_block]->has_jump = true;
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

    if (cur_statement_index > 0)
        add_new_block(blocks, block_index, block_size, cur_statements, &cur_statement_index, &cur_statement_size);
}

method_t *parse_method(struct declare *declare)
{
    method_t *method = malloc(sizeof(method_t));

    block_t *exit_block = malloc(sizeof(block_t));
    exit_block->in_blocks = malloc(sizeof(uint32_t));
    exit_block->in_blocks_index = 0;
    exit_block->in_blocks_size = 2;

    block_t **blocks = malloc(sizeof(block_t *) * 2);
    uint32_t block_size = 2;
    uint32_t block_index = 1;

    struct map *map = create_small_map();

    // The In Block
    blocks[0] = malloc(sizeof(block_t *));
    blocks[0]->out_blocks = malloc(sizeof(uint32_t));
    blocks[0]->out_blocks_index = 0;
    blocks[0]->out_blocks_size = 2;
    blocks[0]->unconditional_jump = true;

    parse_block(declare->body, declare->size_body, blocks, &block_index, &block_size, exit_block);

    for (uint32_t i = 0; i < block_index; i++)
    {
        for (uint32_t j = 0; j < blocks[i]->out_blocks_index; j++)
        {
            add_to_in(blocks[blocks[i]->out_blocks[j]], i);
        }
        create_next_use_information(blocks[i], map);
        blocks[i]->out_blocks_size_dag = blocks[i]->out_blocks_index;
        blocks[i]->out_blocks_dag = malloc(sizeof(bool) * blocks[i]->variables_index);
    }

    if (block_index == block_size)
    {
        block_size *= 2;
        blocks = realloc(blocks, sizeof(block_t *) * block_size);
    }
    blocks[block_index] = exit_block;

    method->blocks = blocks;
    method->block_size = block_index;
    method->name = declare->name;
    method->parameters = declare->parameters;
    method->args = declare->args;

    live_variable_analysis(method);

    return method;
}

void add_to_variables(block_t *block, int32_t input, int32_t hash)
{
    if (hash >= block->variables_index)
    {
        while (block->variables_index >= block->variables_size)
        {
            block->variables_size *= 2;
            block->variables = realloc(block->variables, sizeof(struct var_bin *) * block->variables_size);
        }
        block->variables[hash] = malloc(sizeof(struct var_bin));
        block->variables[hash]->vars = malloc(sizeof(int32_t) * 2);
        block->variables[hash]->index = 0;
        block->variables[hash]->size = 2;
    }
    // Check if the struct is created in the block
    if (block->variables[hash] == NULL)
    {
        block->variables[hash] = malloc(sizeof(struct var_bin));
        block->variables[hash]->vars = malloc(sizeof(int32_t) * 2);
        block->variables[hash]->index = 0;
        block->variables[hash]->size = 2;
    }

    if (block->variables[hash]->index == block->variables[hash]->size)
    {
        block->variables[hash]->size *= 2;
        block->variables[hash]->vars = realloc(block->variables[hash]->vars, sizeof(int32_t) * block->variables[hash]->size);
    }

    block->variables[hash]->vars[block->variables[hash]->index++] = input;
}

void add_to_next_use(block_t *block, Slice *name, int32_t state_index)
{
    int32_t index = get_map_offset(block->variable_map, name);
    if (index == 0)
    {
        index = block->variables_index + 1;
        add_map_offset(block->variable_map, name, index);
    }
    add_to_variables(block, state_index, index);
}

void next_use_expression(expression *expr, block_t *block, int32_t state_index)
{
    switch (expr->type)
    {
    case t_var:
        add_to_next_use(block, expr->character->name, state_index);
        break;
    case t_func:
        for (uint32_t i = 0; i < expr->character->function->args; i++)
        {
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

void next_use_statement(statement *state, block_t *block, int32_t state_index)
{
    switch (state->type)
    {
    case s_func:
        for (uint32_t i = 0; i < state->internal->func->args; i++)
        {
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

void create_next_use_information(block_t *block, struct map *map)
{
    block->variable_map = map;
    block->variables = malloc(sizeof(int32_t *) * 2);
    block->variables_size = 2;
    block->variables_index = 0;

    if (block->has_jump && !block->unconditional_jump)
    {
        next_use_expression(block->jump_expression, block, block->statement_size);
    }

    int32_t i = block->statement_size;
    for (; i > 0; i--)
    {
        statement *s = block->statements[i - 1];
        next_use_statement(s, block, i - 1);
    }

    if (block->variables_index > 0)
    {
        block->defined_in_block = malloc(sizeof(bool) * block->variables_index);
        for (uint32_t i = 0; i < block->variables_index; i++)
            block->defined_in_block[i] = false;

        for (uint32_t j = 0; j < block->variables_index; j++)
        {
            for (uint32_t k = 0; k < block->variables[j]->index; k++)
            {
                if (block->variables[j]->vars[k] < 0)
                {
                    block->defined_in_block[j] = true;
                    break;
                }
            }
        }
    }
}
struct map *create_small_map()
{
    struct map *map = malloc(sizeof(struct map));
    uint16_t size = 40;
    map->map = malloc(size * sizeof(hash_map));
    map->visited = malloc(size * sizeof(bool));
    memset(map->visited, 0, size * sizeof(bool));
    map->size = size;
    map->main = false;
    return map;
}

// say variable is live at the end of the block start of variables[i]->vars[0] with a 1; otherwise false
void live_variable_analysis(method_t *method)
{
    struct queue *start_queue = malloc(sizeof(struct queue));
    start_queue->block_index = method->block_size;
    start_queue->has_next = false;
    struct queue *end_queue = start_queue;

    while (true)
    {
        block_t *blocks = method->blocks[start_queue->block_index];

        for (uint32_t i = 0; i < blocks->variables_index; i++)
        {
            struct var_bin *var = blocks->variables[i];
            if ((var != NULL && var->index > 0 && var->vars[var->index - 1] > 0) || (blocks->out_blocks_dag[i] && !blocks->defined_in_block[i]))
                blocks->in_blocks_dag[i] = true;
        }

        for (uint32_t i = 0; i < blocks->out_blocks_index; i++)
        {
            block_t *block = method->blocks[blocks->in_blocks[i]];
            block->out_blocks_size_dag--;

            for (uint32_t j = 0; j < block->variables_index && j < blocks->variables_index; j++)
            {
                if (blocks->in_blocks_dag[i])
                    block->out_blocks_dag[i] = true;
            }

            if (block->out_blocks_size_dag == 0 || (block->out_blocks_size_dag == 1 && block->loop))
            {
                struct queue *queue = malloc(sizeof(struct queue));
                queue->block_index = i;
                end_queue->next = queue;
                end_queue->has_next = true;
                end_queue = queue;
            }
        }

        if (!start_queue->has_next)
        {
            break;
        }
        start_queue = start_queue->next;
    }
}

void store_instruction(emitter_t *emitter, uint16_t reg)
{
    push_register(emitter, emitter->registers[reg]);
}

void unstore_instruction(emitter_t *emitter, uint16_t reg)
{
    pop_register(emitter, emitter->registers[reg]);
}

void create_function_stack(emitter_t *emitter, struct compile_expr com_expr, registers_t *regs)
{
    // TODO: eval functions and expressions, and generate code, stick the code into stack
}

void move_instruction(emitter_t *emitter, uint16_t src, uint16_t dest)
{
    emit_reg_to_reg(emitter, "movq %%%s, %%%s", emitter->registers[src], emitter->registers[dest]);
}

void generate_variable(emitter_t *emitter, registers_t *regs, Slice *name, uint16_t base)
{
    int32_t reg = is_in_register(regs, name);
    if (reg == -1)
    {
        pop_variable(emitter, name, emitter->registers[base]);
    }
    else
    {
        move_instruction(emitter, reg, base);
    }
}

void generate_number(emitter_t *emitter, uint64_t number, uint16_t base)
{
    emit_number_to_reg(emitter, "movq $%lu, %%%s", number, emitter->registers[base]);
}

void generate_instruction(emitter_t *emitter, registers_t *regs, enum type_of type, uint16_t src, uint16_t dest)
{
    switch (type)
    {
    case t_plus:
        emit_reg_to_reg(emitter, "addq %%%s, %%%s", emitter->registers[src], emitter->registers[dest]);
        break;
    case t_minus:
        emit_reg_to_reg(emitter, "subq %%%s, %%%s", emitter->registers[src], emitter->registers[dest]);
        break;
    default:
        emit(emitter, "ERROR: Not implemented");
        break;
    }
}

/**
 * Assumes the number of registers available is greater than or equal to the number of registers needed
 */
void compile_expression_tree(emitter_t *emitter, registers_t *regs, expression *expr, block_t *block, uint16_t base, uint16_t *available_registers)
{
    switch (expr->type)
    {
    case t_var:
        generate_variable(emitter, regs, expr->character->name, available_registers[base]);
        break;
    case t_num:
        generate_number(emitter, expr->character->value, available_registers[base]);
        break;
    case t_func:
        break;
    case t_print:
        break;
    case t_not:
        compile_expression_tree(emitter, regs, expr->left, block, base, available_registers);
        break;
    default:
    {
        uint16_t l_ershov = expr->left->ershov_number;
        uint16_t r_ershov = expr->right->ershov_number;

        if (l_ershov == r_ershov)
        {
            compile_expression_tree(emitter, regs, expr->right, block, base + 1, available_registers);
            compile_expression_tree(emitter, regs, expr->left, block, base, available_registers);
            generate_instruction(emitter, regs, expr->type, available_registers[base + l_ershov - 1], available_registers[base + l_ershov]);
        }
        else if (l_ershov > r_ershov)
        {
            compile_expression_tree(emitter, regs, expr->left, block, base, available_registers);
            compile_expression_tree(emitter, regs, expr->right, block, base, available_registers);
            generate_instruction(emitter, regs, expr->type, available_registers[base + r_ershov - 1], available_registers[base + l_ershov - 1]);
        }
        else
        {
            compile_expression_tree(emitter, regs, expr->right, block, base, available_registers);
            compile_expression_tree(emitter, regs, expr->left, block, base, available_registers);
            generate_instruction(emitter, regs, expr->type, available_registers[base + r_ershov - 1], available_registers[base + l_ershov - 1]);
            move_instruction(emitter, available_registers[base + l_ershov - 1], available_registers[base + r_ershov - 1]);
        }
        break;
    }
    }
}

void compile_uneven_expression_tree(emitter_t *emitter, registers_t *regs, expression *expr, block_t *block, uint16_t *available_registers, uint16_t available_registers_size)
{
    uint16_t l_ershov = expr->left->ershov_number;
    uint16_t r_ershov = expr->right->ershov_number;

    if (!(l_ershov > available_registers_size || r_ershov > available_registers_size))
    {
        compile_expression_tree(emitter, regs, expr, block, available_registers_size, available_registers);
        return;
    }

    if (l_ershov >= r_ershov)
    {
        compile_uneven_expression_tree(emitter, regs, expr->left, block, available_registers, available_registers_size);
        store_instruction(emitter, available_registers[available_registers_size - 1]);
        if (r_ershov > available_registers_size)
            compile_uneven_expression_tree(emitter, regs, expr->right, block, available_registers, available_registers_size);
        else
            compile_expression_tree(emitter, regs, expr->right, block, available_registers_size - r_ershov, available_registers);
        unstore_instruction(emitter, available_registers[available_registers_size - 2]);
        generate_instruction(emitter, regs, expr->type, available_registers[available_registers_size - 2], available_registers[available_registers_size - 1]);
    }
    else
    {
        compile_uneven_expression_tree(emitter, regs, expr->right, block, available_registers, available_registers_size);
        store_instruction(emitter, available_registers[available_registers_size - 1]);
        if (l_ershov > available_registers_size)
            compile_uneven_expression_tree(emitter, regs, expr->left, block, available_registers, available_registers_size);
        else
            compile_expression_tree(emitter, regs, expr->left, block, available_registers_size - l_ershov, available_registers);
        unstore_instruction(emitter, available_registers[available_registers_size - 2]);
        generate_instruction(emitter, regs, expr->type, available_registers[available_registers_size - 1], available_registers[available_registers_size - 2]);
        move_instruction(emitter, available_registers[available_registers_size - 2], available_registers[available_registers_size - 1]);
    }
}

uint16_t generate_expression(emitter_t *emitter, expression *expr, uint32_t statement_index, block_t *block, registers_t *reg)
{
    expr = preprocess_expression(expr);
    expr_function **functions = malloc(sizeof(expr_function *) * 2);
    struct compile_expr compile_expr = {functions, 0, 2};
    comb_expression(expr, &compile_expr);
    label_ershov_number(expr);

    create_function_stack(emitter, compile_expr, reg);

    uint16_t available_registers_size = available_registers(reg, statement_index);
    uint16_t *available_registers = malloc(sizeof(uint16_t) * available_registers_size);
    uint16_t output_register;
    if (expr->ershov_number > available_registers_size) // Index bounding, need to check
    {
        compile_uneven_expression_tree(emitter, reg, expr, block, available_registers, available_registers_size);
        output_register = available_registers[available_registers_size - 1];
    }
    else
    {
        compile_expression_tree(emitter, reg, expr, block, 0, available_registers);
        output_register = available_registers[expr->ershov_number - 1];
    }

    return output_register;
}

void compile_statement_in_block(emitter_t *emitter, statement *stmt, registers_t *regs, block_t *block, uint32_t statement_index)
{
    switch (stmt->type)
    {
    case s_var:
    {
        uint16_t register_index = generate_expression(emitter, stmt->internal->var->expr, statement_index, block, regs);
        int16_t reg = get_reg(regs, stmt->internal->var->name);
        move_instruction(emitter, reg, register_index);
        break;
    }
    default:
    {
        printf("YA DONE GOOFED");
        break;
    }
    }
}

void compile_method(emitter_t *emitter, struct declare *declare)
{

    struct map *new_map = create_map();
    struct map *old_map = emitter->var_map;
    uint16_t old_offset = emitter->var_offset;
    uint64_t old_rsp = emitter->stack_pointer;
    emitter->var_map = new_map;
    emitter->var_offset = 1;
    emitter->stack_pointer = 8;

    method_t *method = parse_method(declare);

    emit_start_function(emitter, declare->name);

    for (uint32_t i = 0; i < method->block_size; i++)
    {
        block_t *block = method->blocks[i];
        registers_t *reg = declare_register(block);

        for (uint32_t j = 0; j < block->statement_size; j++)
        {
            statement *stmt = block->statements[j];
            compile_statement_in_block(emitter, stmt, reg, block, j);
        }

        free(reg);
    }

    free(method);

    emitter->var_offset = old_offset; // whats the point of this, the emitter is local
    emitter->var_map = old_map;
    emitter->stack_pointer = old_rsp;

    free_map(new_map);
    free(new_map->map);
    free(new_map->visited);
    free(new_map);
    emit_end_function(emitter);
}