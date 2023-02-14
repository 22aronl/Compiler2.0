#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#include "main.h"
#include "expression.h"
#include "hashmap.h"
#include "statement.h"
#include "emitter.h"

void compile_expression(emitter_t *emitter, expression *e)
{
    switch (e->type)
    {
    case t_not:
        compile_expression(emitter, e->left);
        pop_register(emitter, "rax");
        emit(emitter, "movq $0, %rbx");
        emit(emitter, "cmp \%rbx, \%rax");
        emit(emitter, "sete \%al");
        emit(emitter, "movzbq \%al, \%rax");
        push_register(emitter, "rax");
        break;
    case t_star:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rax");
        pop_register(emitter, "rbx");
        emit(emitter, "mul \%rbx");
        push_register(emitter, "rax");
        break;
    case t_divide:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rbx");
        pop_register(emitter, "rax");
        emit(emitter, "movq $0, %rdx");
        emit(emitter, "div \%rbx");
        push_register(emitter, "rax");
        break;
    case t_mod:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rbx");
        pop_register(emitter, "rax");
        emit(emitter, "movq $0, %rdx");
        emit(emitter, "div \%rbx");
        push_register(emitter, "rdx");
        break;
    case t_plus:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rax");
        pop_register(emitter, "rbx");
        emit(emitter, "add \%rbx, \%rax");
        push_register(emitter, "rax");
        break;
    case t_minus:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rbx");
        pop_register(emitter, "rax");
        emit(emitter, "sub \%rbx, \%rax");
        push_register(emitter, "rax");
        break;
    case t_l:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rbx");
        pop_register(emitter, "rax");
        emit(emitter, "cmp \%rbx, \%rax");
        emit(emitter, "setb \%al");
        emit(emitter, "movzbq \%al, \%rax");
        push_register(emitter, "rax");
        break;
    case t_g:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rbx");
        pop_register(emitter, "rax");
        emit(emitter, "cmp \%rbx, \%rax");
        emit(emitter, "seta \%al");
        emit(emitter, "movzbq \%al, \%rax");
        push_register(emitter, "rax");
        break;
    case t_lt:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rbx");
        pop_register(emitter, "rax");
        emit(emitter, "cmp \%rbx, \%rax");
        emit(emitter, "setbe \%al");
        emit(emitter, "movzbq \%al, \%rax");
        push_register(emitter, "rax");
        break;
    case t_gt:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rbx");
        pop_register(emitter, "rax");
        emit(emitter, "cmp \%rbx, \%rax");
        emit(emitter, "setae \%al");
        emit(emitter, "movzbq \%al, \%rax");
        push_register(emitter, "rax");
        break;
    case t_eq:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rbx");
        pop_register(emitter, "rax");
        emit(emitter, "cmp \%rbx, \%rax");
        emit(emitter, "sete \%al");
        emit(emitter, "movzbq \%al, \%rax");
        push_register(emitter, "rax");
        break;
    case t_neq:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rbx");
        pop_register(emitter, "rax");
        emit(emitter, "cmp \%rbx, \%rax");
        emit(emitter, "setne \%al");
        emit(emitter, "movzbq \%al, \%rax");
        push_register(emitter, "rax");
        break;
    case t_and:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rax");
        pop_register(emitter, "rbx");
        emit(emitter, "cmp $0, \%rax");
        emit(emitter, "setg \%al");
        emit(emitter, "cmp $0, \%rbx");
        emit(emitter, "setg \%bl");
        emit(emitter, "and \%bl, \%al");
        emit(emitter, "movzbq \%al, \%rax");
        push_register(emitter, "rax");
        break;
    case t_or:
        compile_expression(emitter, e->left);
        compile_expression(emitter, e->right);
        pop_register(emitter, "rax");
        pop_register(emitter, "rbx");
        emit(emitter, "cmp $0, \%rax");
        emit(emitter, "setg \%al");
        emit(emitter, "cmp $0, \%rbx");
        emit(emitter, "setg \%bl");
        emit(emitter, "or \%bl, \%al");
        emit(emitter, "movzbq \%al, \%rax");
        push_register(emitter, "rax");
        break;
    case t_num:
        emit_number(emitter, "movq $%lu, \%rax", e->character->value);
        push_register(emitter, "rax");
        break;
    case t_var:
        emit_number(emitter, "movq %d(\%rbp), \%rax", get_offset(emitter, e->character->name));
        push_register(emitter, "rax");
        break;
    case t_func:
        call_function(emitter, e->character->function);
        push_register(emitter, "rax");
        break;
    case t_print:
        compile_expression(emitter, e->left);
        pop_register(emitter, "rax");
        emit(emitter, "mov %rax, %rsi");
        emit(emitter, "mov $0, %rax");

        bool change = align_stack(emitter);
        emit(emitter, "lea format(%rip),%rdi");
        emit(emitter, ".extern printf");
        emit(emitter, "call printf");
        emit(emitter, "mov $0, %rax");
        realign_stack(emitter, change);
        push_register(emitter, "rax");
        break;
    default:
        emit(emitter, "implement later");
        break;
    }
}

/**
 * Recursively frees up expression memory
 */
void free_expression(expression *e)
{
    switch (e->type)
    {
    case t_not:
        free_expression(e->left);
        break;
    case t_print:
        free_expression(e->left);
        free(e->character);
        break;
    case t_num:
        free(e->character);
        break;
    case t_var:
        free(e->character->name);
        free(e->character);
        break;
    case t_func:
        free_function(e->character->function);
        free(e->character);
        break;
    default:
        free_expression(e->left);
        free_expression(e->right);
        break;
    }
    free(e);
}

/**
 * Recursively evaluates an expression
 */
uint64_t eval_expr(Interpreter *in, struct map *map, expression *e)
{
    switch (e->type)
    {
    case t_not:
        return !eval_expr(in, map, e->left);
    case t_star:
        return eval_expr(in, map, e->left) * eval_expr(in, map, e->right);
    case t_divide:
        return eval_expr(in, map, e->left) / eval_expr(in, map, e->right);
    case t_mod:
        return eval_expr(in, map, e->left) % eval_expr(in, map, e->right);
    case t_plus:
        return eval_expr(in, map, e->left) + eval_expr(in, map, e->right);
    case t_minus:
        return eval_expr(in, map, e->left) - eval_expr(in, map, e->right);
    case t_l:
        return eval_expr(in, map, e->left) < eval_expr(in, map, e->right);
    case t_lt:
        return eval_expr(in, map, e->left) <= eval_expr(in, map, e->right);
    case t_g:
        return eval_expr(in, map, e->left) > eval_expr(in, map, e->right);
    case t_gt:
        return eval_expr(in, map, e->left) >= eval_expr(in, map, e->right);
    case t_eq:
        return eval_expr(in, map, e->left) == eval_expr(in, map, e->right);
    case t_neq:
        return eval_expr(in, map, e->left) != eval_expr(in, map, e->right);
    case t_and:
    {
        uint64_t a = eval_expr(in, map, e->left);
        uint64_t b = eval_expr(in, map, e->right); // I HAD TO REMOVE SHORT CIRCITING :(
        return a && b;
    }
    case t_or:
    {
        uint64_t c = eval_expr(in, map, e->left);
        uint64_t d = eval_expr(in, map, e->right);
        return c || d;
    }
    case t_num:
        return e->character->value;
    case t_var:
        return getSymbol(in, map, e->character->name);
    case t_func:
        return evaluate_function(in, map, e->character->function, get_function(in, e->character->function->name));
    case t_print:
        printf("%" PRIu64 "\n", eval_expr(in, map, e->left));
        return 0;
    default:
        return 0;
    }
}