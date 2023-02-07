#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#include "main.h"
#include "expression.h"
#include "hashmap.h"
#include "statement.h"

/**
 * Recursively frees up expression memory
*/
void free_expression(expression *e)
{
    switch(e->type)
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
        printf("%"PRIu64"\n", eval_expr(in, map, e->left));
        return 0;
    default:
        return 0;
    }
}