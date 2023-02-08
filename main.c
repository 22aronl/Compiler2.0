#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "slice2.h"
#include "hashmap.h"
#include "main.h"
#include "expression.h"
#include "statement.h"
#include "emitter.h"

expression *parse_expression(Interpreter *in);
statement *parse_statement(Interpreter *in, bool *effects);

/**
 * creates an interpreter
*/
Interpreter createInterpreter(char const *const program)
{
    bool *visited = malloc(MAX_SYMBOLS * sizeof(bool));
    memset(visited, 0, MAX_SYMBOLS * sizeof(bool));
    bool *visited_func = malloc(MAX_SYMBOLS * sizeof(bool));
    memset(visited_func, 0, MAX_SYMBOLS * sizeof(bool));
    Interpreter in = {program, program, visited, visited_func};
    in.symbol_table = malloc(MAX_SYMBOLS * sizeof(struct symbol_table));
    in.function_table = malloc(MAX_SYMBOLS * sizeof(struct function_table));
    in.size_ast = 100;
    in.ast = malloc(in.size_ast * sizeof(statement));
    in.cur_ast = 0;
    return in;
    // TODO: should interpreter be a pointer?
}

/**
 * skips over all white lines
*/
void skip(Interpreter *in)
{
    while (isspace(*in->current))
    {
        in->current += 1;
    }
}

/**
 * fails the program
*/
void fail(Interpreter *in)
{
    printf("failed at offset %ld\n", (size_t)(in->current - in->program));
    printf("%s\n", in->current);
    exit(1);
}

/**
 * ends the program if the current character is not a space
*/
void end_or_fail(Interpreter *in)
{
    while (isspace(*in->current))
    {
        in->current += 1;
    }
    if (*in->current != 0)
        fail(in);
}

/**
 * checks if the current character is a given character defined by str
*/
bool consume(Interpreter *in, const char *str)
{
    skip(in);

    size_t i = 0;
    while (1)
    {
        char const expected = str[i];
        char const found = in->current[i];
        if (expected == 0)
        {
            /* survived to the end of the expected string */
            in->current += i;
            return 1;
        }
        if (expected != found)
        {
            return 0;
        }
        // assertion: found != 0
        i += 1;
    }
}

/**
 * This eats the string if it is a valid identifier, however, it checks up until there is a space or non-alphanumeric character
*/
bool eat(Interpreter *in, const char *str)
{
    skip(in);
    size_t i = 0;
    while (1)
    {
        char const expected = str[i];
        char const found = in->current[i];
        if (expected == 0)
        {
            /* survived to the end of the expected string */
            if (isalnum(found))
                return 0;
            in->current += i;
            return 1;
        }
        if (expected != found)
        {
            return 0;
        }
        // assertion: found != 0
        i += 1;
    }
}

/**
 * This fails if the string is not a valid identifier
*/
void consume_or_fail(Interpreter *in, const char *str)
{
    if (!consume(in, str))
    {
        fail(in);
    }
}

/**
 * Consumes a single token
*/
Slice *consume_token(Interpreter *in)
{
    skip(in);
    char const *start = in->current;
    do
    {
        in->current += 1;
    } while (isalnum(*in->current));

    Slice *sl = malloc(sizeof(Slice));
    *sl = slice_construct2(start, in->current);
    return sl;
}

// returns true if the literal had a value, otherwise it returns false
uint64_t consume_literal(Interpreter *in)
{
    skip(in);
    uint64_t v = 0;
    do
    {
        v = 10 * v + ((*in->current) - '0');
        in->current += 1;
    } while (isdigit(*in->current));
    return v;
}

/**
 * creates an expression with a left and right expression and a type
*/
expression *create_expression(expression *left, expression *right, enum type_of type)
{
    expression *expr = malloc(sizeof(expression));
    expr->left = left;
    expr->right = right;
    expr->type = type;
    return expr;
}

/**
 * Parses all the arguments of a function and then returns that array
*/
Slice *parse_args(Interpreter *in, Slice *args, uint16_t *num_args)
{
    uint16_t args_size = 0;
    uint16_t args_capacity = 1;
    if (consume(in, ")"))
    {
        *num_args = 0;
        return args;
    }
    do
    {
        if (args_size == args_capacity)
        {
            args_capacity *= 2;
            args = realloc(args, args_capacity * sizeof(Slice));
        }
        Slice *sl = consume_token(in);
        args[args_size++] = *sl;
        free(sl);

    } while (consume(in, ","));
    consume_or_fail(in, ")");
    *num_args = args_size;
    return args;
}

/**
 * parses all the expressions of a function and then returns that array
*/
expression **parse_args2(Interpreter *in, expression **args, uint16_t *num_args)
{
    uint16_t args_size = 0;
    uint16_t args_capacity = 1;
    if (consume(in, ")"))
    {
        *num_args = 0;
        return args;
    }
    do
    {
        if (args_size == args_capacity)
        {
            args_capacity *= 2;
            args = realloc(args, args_capacity * sizeof(expression *));
        }
        args[args_size++] = parse_expression(in);

    } while (consume(in, ","));
    consume_or_fail(in, ")");
    *num_args = args_size;
    return args;
}

/**
 * Parses the value at the end of the expression tree
*/
expression *parse_value(Interpreter *in)
{
    if (consume(in, "("))
    {
        expression *left = parse_expression(in);
        consume_or_fail(in, ")");
        return left;
    }
    expression *expr = malloc(sizeof(expression));
    character *cha = malloc(sizeof(character));
    expr->character = cha;
    if (isdigit(*in->current))
    {
        uint64_t value = consume_literal(in);
        cha->value = value;
        expr->type = t_num;
    }
    else if (isalpha(*in->current))
    {
        Slice *sl = consume_token(in);
        if (consume(in, "("))
        {
            if (slice_eq(sl, "print")) //checks if method is a print statement
            {
                free(sl);
                expr->type = t_print;
                expr->left = parse_expression(in);
                consume_or_fail(in, ")");
            }
            else //otherwise its a normal function
            {
                struct func *func = malloc(sizeof(struct func));

                expression **args = malloc(sizeof(expression *));
                uint16_t num_args = 0;
                args = parse_args2(in, args, &num_args);
                func->name = sl;
                func->parameters = args;
                func->args = num_args;

                cha->function = func;
                expr->type = t_func;
            }
        }
        else
        {
            cha->name = sl;
            expr->type = t_var;
        }
    }
    else
    {
        fail(in);
    }
    return expr;
}

expression *parse_not_expr(Interpreter *in)
{
    if (consume(in, "!"))
    {
        expression *left = parse_not_expr(in);
        return create_expression(left, NULL, t_not);
    }
    return parse_value(in);
}

expression *parse_mult_expr(Interpreter *in)
{
    expression *left = parse_not_expr(in);
    while (true)
    {
        if (consume(in, "*"))
        {
            expression *right = parse_not_expr(in);
            left = create_expression(left, right, t_star);
        }
        else if (consume(in, "/"))
        {
            expression *right = parse_not_expr(in);
            left = create_expression(left, right, t_divide);
        }
        else if (consume(in, "\%"))
        {
            expression *right = parse_not_expr(in);
            left = create_expression(left, right, t_mod);
        }
        else
        {
            break;
        }
    }
    return left;
}

expression *parse_add_expr(Interpreter *in)
{
    expression *left = parse_mult_expr(in);
    while (true)
    {
        if (consume(in, "+"))
        {
            expression *right = parse_mult_expr(in);
            left = create_expression(left, right, t_plus);
        }
        else if (consume(in, "-"))
        {
            expression *right = parse_mult_expr(in);
            left = create_expression(left, right, t_minus);
        }
        else
        {
            break;
        }
    }
    return left;
}

expression *parse_rel_expr(Interpreter *in)
{
    expression *left = parse_add_expr(in);
    while (true)
    {
        if (consume(in, "<="))
        {
            expression *right = parse_add_expr(in);
            left = create_expression(left, right, t_lt);
        }
        else if (consume(in, ">="))
        {
            expression *right = parse_add_expr(in);
            left = create_expression(left, right, t_gt);
        }
        else if (consume(in, "<"))
        {
            expression *right = parse_add_expr(in);
            left = create_expression(left, right, t_l);
        }
        else if (consume(in, ">"))
        {
            expression *right = parse_add_expr(in);
            left = create_expression(left, right, t_g);
        }
        else
        {
            break;
        }
    }
    return left;
}

expression *parse_equal_expr(Interpreter *in)
{
    expression *left = parse_rel_expr(in);
    while (true)
    {
        if (consume(in, "=="))
        {
            expression *right = parse_rel_expr(in);
            left = create_expression(left, right, t_eq);
        }
        else if (consume(in, "!="))
        {
            expression *right = parse_rel_expr(in);
            left = create_expression(left, right, t_neq);
        }
        else
        {
            break;
        }
    }
    return left;
}

expression *parse_and_expr(Interpreter *in)
{
    expression *left = parse_equal_expr(in);
    while (consume(in, "&&"))
    {
        expression *right = parse_equal_expr(in);
        left = create_expression(left, right, t_and);
    }
    return left;
}

expression *parse_or_expr(Interpreter *in)
{
    expression *left = parse_and_expr(in);
    while (consume(in, "||"))
    {
        expression *right = parse_and_expr(in);
        left = create_expression(left, right, t_or);
    }
    return left;
}

/**
 * Parses an expression with a recursive descent parser
 */
expression *parse_expression(Interpreter *in)
{
    return parse_or_expr(in);
}

/**
 * Parses the body of a function or if statement or a while sattement
 */
statement **parse_body(Interpreter *in, statement **body, uint32_t *num_body)
{
    uint32_t body_size = 0;
    uint32_t body_capacity = 1;
    bool continue_parsing = true;
    while (!consume(in, "}"))
    {
        if (body_size == body_capacity)
        {
            body_capacity *= 2;
            body = realloc(body, body_capacity * sizeof(statement *));
        }
        body[body_size++] = parse_statement(in, &continue_parsing);
    }
    *num_body = body_size;
    return body;
}

/**
 * Parses each statement with a recursive descent parser. Checks for each language with grammar to insure everything works
 */
statement *parse_statement(Interpreter *in, bool *continue_parsing)
{
    statement *state = malloc(sizeof(statement));
    union internal *internal = malloc(sizeof(union internal));
    state->internal = internal;

    if (eat(in, "if"))
    {
        consume_or_fail(in, "("); //if the statement is an if statement
        expression *condition = parse_expression(in);
        consume_or_fail(in, ")");
        consume_or_fail(in, "{");
        statement **body = malloc(sizeof(statement *));
        uint32_t body_size = 0;
        body = parse_body(in, body, &body_size);

        struct if_statement *if_statement = malloc(sizeof(struct if_statement));
        if_statement->condition = condition;
        if_statement->body = body;
        if_statement->size_body = body_size;

        if (eat(in, "else")) //if the statement has an else
        {
            consume_or_fail(in, "{");
            statement **else_body = malloc(sizeof(statement *));
            uint32_t else_body_size = 0;
            else_body = parse_body(in, else_body, &else_body_size);

            if_statement->has_else = true;
            if_statement->else_body = else_body;
            if_statement->size_else = else_body_size;
        }
        else
        {
            if_statement->has_else = false;
        }

        state->type = s_if;
        internal->if_statement = if_statement;
    }
    else if (eat(in, "while"))
    {
        consume_or_fail(in, "("); //if the statement is a while statement
        expression *condition = parse_expression(in);
        consume_or_fail(in, ")");
        consume_or_fail(in, "{");
        statement **body = malloc(sizeof(statement *));
        uint32_t body_size = 0;
        body = parse_body(in, body, &body_size);

        struct while_statement *while_statement = malloc(sizeof(struct while_statement));
        while_statement->condition = condition;
        while_statement->body = body;
        while_statement->size_body = body_size;

        state->type = s_while;
        internal->while_statement = while_statement;
    }
    else if (eat(in, "fun"))
    {
        Slice *name = consume_token(in);
        consume_or_fail(in, "(");
        Slice *args = malloc(sizeof(Slice));
        uint16_t args_size = 0;
        args = parse_args(in, args, &args_size);
        consume_or_fail(in, "{");
        statement **body = malloc(sizeof(statement **));
        uint32_t body_size = 0;
        body = parse_body(in, body, &body_size);
        struct declare *declare = malloc(sizeof(struct declare));
        declare->name = name;
        declare->parameters = args;
        declare->body = body;
        declare->args = args_size;
        declare->size_body = body_size;

        state->type = s_declare;
        internal->declare = declare;
    }
    else if (eat(in, "return"))
    {
        expression *expr = parse_expression(in);
        struct return_statement *return_statement = malloc(sizeof(struct return_statement));
        return_statement->expr = expr;

        state->type = s_return;
        internal->return_statement = return_statement;
    }
    else if (isalpha(*in->current))
    {
        Slice *name = consume_token(in);
        if (consume(in, "(")) //the statement is a function
        {
            if (slice_eq(name, "print")) //checks to see if the function is print
            {
                free(name);
                expression *expr = parse_expression(in);
                consume_or_fail(in, ")");

                struct print *print = malloc(sizeof(struct print));
                print->expr = expr;

                state->type = s_print;
                internal->print = print;
            }
            else
            {
                expression **args = malloc(sizeof(expression *));
                uint16_t args_size = 0;
                args = parse_args2(in, args, &args_size);
                struct func *func = malloc(sizeof(struct func));
                func->name = name;
                func->parameters = args;
                func->args = args_size;

                state->type = s_func;
                internal->func = func;
            }
        }
        else
        {
            consume_or_fail(in, "="); //statement is assignemtn
            expression *expr = parse_expression(in);
            struct var *var = malloc(sizeof(struct var));
            var->name = name;
            var->expr = expr;

            state->type = s_var;
            internal->var = var;
        }
    }
    else
    {
        *continue_parsing = false;
        free(state->internal);
        free(state);
        return NULL;
    }
    return state;
}

/**
 * adds statement to a statement array
*/
void add_statement(Interpreter *in, statement *state)
{
    if (in->cur_ast == in->size_ast)
    {
        in->size_ast *= 2;
        in->ast = realloc(in->ast, in->size_ast * sizeof(statement *));
    }
    in->ast[in->cur_ast++] = state;
}

/**
 * cleans up all the statements in the ast
*/
void clear_ast(Interpreter *in)
{
    for (uint32_t i = 0; i < in->cur_ast; i++)
    {
        free_statement(in->ast[i]);
    }
    free(in->ast);
}

/**
 * frees the internal statements in an interpereter
*/
void free_interpreter_internal(Interpreter *in)
{
    for (uint32_t i = 0; i < MAX_SYMBOLS; i++)
    {
        if (in->visited[i])
            free(in->symbol_table[i].bins);
    }
    free(in->symbol_table);
    free(in->visited);
    for (uint32_t i = 0; i < MAX_SYMBOLS; i++)
    {
        if (in->visited_func[i])
            free(in->function_table[i].bins);
    }
    free(in->visited_func);
    free(in->function_table);
}

void run_compile(Interpreter* in) {
    expression* exp = parse_expression(in);
    emitter_t* em = {NULL};
    compile_expression(em, exp);

}

void run(Interpreter *in)
{
    bool continue_parsing = true;
    struct map map = {in->symbol_table, in->visited, MAX_SYMBOLS, true};
    uint64_t return_value = 0;
    while (true)
    {
        statement *state = parse_statement(in, &continue_parsing);
        if (!continue_parsing)
        {
            break;
        }
        evaluate_statement(in, &map, state, &return_value);
        add_statement(in, state);
    }

    // frees all the memory
    clear_ast(in);
    free_interpreter_internal(in);
    end_or_fail(in);
}

int main(int argc, const char *const *const argv)
{

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <file name>\n", argv[0]);
        exit(1);
    }

    // open the file
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        exit(1);
    }

    // determine its size (std::filesystem::get_size?)
    struct stat file_stats;
    int rc = fstat(fd, &file_stats);
    if (rc != 0)
    {
        perror("fstat");
        exit(1);
    }

    // map the file in my address space
    char const *prog = (char const *)mmap(
        0,
        file_stats.st_size,
        PROT_READ,
        MAP_PRIVATE,
        fd,
        0);
    if (prog == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    Interpreter x = createInterpreter(prog);

    // run(&x);
    run_compile(&x);

    return 0;
}
