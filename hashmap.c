
#include <stdlib.h>
#include <stdint.h>

#include "hashmap.h"
#include "slice2.h"
#include "main.h"
#include "statement.h"

uint16_t get_map_offset(struct map* map, Slice* key) {
    bool* visited = map->visited;
    hash_map* symbol_table = map->map;
    uint32_t size = map->size;
    int hash = sliceHash(key) % size;
    if (visited[hash])
    {
        struct symbol_table *table = &symbol_table[hash];
        for (int i = 0; i < table->cur; i++)
        {
            if (slice_eq2(table->bins[i].key, key))
            {
                return 1;
            }
        }
    }
    return 0;
}

void add_map_offset(struct map* map, Slice* key, uint32_t value) {

    bool *visited = map->visited;
    hash_map *symbol_table = map->map;
    uint32_t size = map->size;


    int hash = sliceHash(key) % size;
    if (visited[hash])
    {
        struct symbol_table *table = &symbol_table[hash];
        for (int i = 0; i < table->cur; i++)
        {
            if (slice_eq2(table->bins[i].key, key))
            {
                table->bins[i].value = value;
                return;
            }
        }
        // doubles the current bin if it is full
        if (table->cur == table->size)
        {
            table->size *= 2;
            table->bins = realloc(table->bins, table->size * sizeof(struct bin));
        }
        table->bins[table->cur].key = key;
        table->bins[table->cur].value = value;
        table->cur += 1;
    }
    else
    {
        visited[hash] = true;
        symbol_table[hash].size = 2;
        symbol_table[hash].cur = 1;
        symbol_table[hash].bins = malloc(2 * sizeof(struct bin));
        symbol_table[hash].bins[0].key = key;
        symbol_table[hash].bins[0].value = value;
    }
}

/**
 * Checks if a key existins in a given map
*/
bool symbol_exists(bool *visited, hash_map *map, uint32_t size, Slice *key)
{
    int hash = sliceHash(key) % size;
    if (visited[hash])
    {
        struct symbol_table *table = &map[hash];
        for (int i = 0; i < table->cur; i++)
        {
            if (slice_eq2(table->bins[i].key, key))
            {
                return true;
            }
        }
    }
    return false;
}

// adds a key value pair to the map
void addSymbol(Interpreter *in, struct map *map, Slice *key, uint64_t value, bool force_local)
{
    bool *visited;
    hash_map *symbol_table;
    uint32_t size;

    if (map->main || force_local) //Checks whether the symbol should be added to the local map or the global map
    {
        visited = map->visited;
        symbol_table = map->map;
        size = map->size;
    }
    else if (!symbol_exists(map->visited, map->map, map->size, key) && symbol_exists(in->visited, in->symbol_table, MAX_SYMBOLS, key))
    { //checks taht the symbol does not exist in the local map and that it exists in the global map
        visited = in->visited;
        symbol_table = in->symbol_table;
        size = MAX_SYMBOLS;
    }
    else
    {
        visited = map->visited;
        symbol_table = map->map;
        size = map->size;
    }

    int hash = sliceHash(key) % size;
    if (visited[hash])
    {
        struct symbol_table *table = &symbol_table[hash];
        for (int i = 0; i < table->cur; i++)
        {
            if (slice_eq2(table->bins[i].key, key))
            {
                table->bins[i].value = value;
                return;
            }
        }
        // doubles the current bin if it is full
        if (table->cur == table->size)
        {
            table->size *= 2;
            table->bins = realloc(table->bins, table->size * sizeof(struct bin));
        }
        table->bins[table->cur].key = key;
        table->bins[table->cur].value = value;
        table->cur += 1;
    }
    else
    {
        visited[hash] = true;
        symbol_table[hash].size = 2;
        symbol_table[hash].cur = 1;
        symbol_table[hash].bins = malloc(2 * sizeof(struct bin));
        symbol_table[hash].bins[0].key = key;
        symbol_table[hash].bins[0].value = value;
    }
}

// retrives the key from the map, if it is not found it returns 0
uint64_t getSymbol(Interpreter *in, struct map *map, Slice *key)
{
    bool *visited = map->visited;
    hash_map *symbol_table = map->map;
    uint32_t size = map->size;

    int hash = sliceHash(key) % size;
    if (visited[hash]) //checks the local map
    {
        struct symbol_table *table = &symbol_table[hash];
        for (int i = 0; i < table->cur; i++)
        {
            if (slice_eq2(table->bins[i].key, key))
            {
                return table->bins[i].value;
            }
        }
    }

    if (!map->main) //checks the global map
    {
        visited = in->visited;
        symbol_table = in->symbol_table;
        size = MAX_SYMBOLS;

        hash = sliceHash(key) % size;
        if (visited[hash])
        {
            struct symbol_table *table = &symbol_table[hash];
            for (int i = 0; i < table->cur; i++)
            {
                if (slice_eq2(table->bins[i].key, key))
                {
                    return table->bins[i].value;
                }
            }
        }
    }
    return 0;
}

/**
 * Adds a function to the function table
*/
void add_function(Interpreter *in, struct declare *function)
{
    int hash = sliceHash(function->name) % MAX_SYMBOLS;
    if (in->visited_func[hash])
    {
        struct function_table *table = &in->function_table[hash];
        for (int i = 0; i < table->cur; i++)
        {
            if (slice_eq2(table->bins[i].name, function->name))
            {
                table->bins[i] = *function; // Not needed unless we want to change function declaration
                return;
            }
        }
        // doubles the current bin if it is full
        if (table->cur == table->size)
        {
            table->size *= 2;
            table->bins = realloc(table->bins, table->size * sizeof(struct declare));
        }
        table->bins[table->cur] = *function;
        table->cur += 1;
    }
    else
    {
        in->visited_func[hash] = true;
        in->function_table[hash].size = 2;
        in->function_table[hash].cur = 1;
        in->function_table[hash].bins = malloc(2 * sizeof(struct declare));
        in->function_table[hash].bins[0] = *function;
    }
}

/**
 * Gets the function with the Slice* name from the function table
*/
struct declare *get_function(Interpreter *in, Slice *name)
{
    int hash = sliceHash(name) % MAX_SYMBOLS;
    if (in->visited_func[hash])
    {
        struct function_table *table = &in->function_table[hash];
        for (int i = 0; i < table->cur; i++)
        {
            if (slice_eq2(table->bins[i].name, name))
            {
                return &table->bins[i];
            }
        }
    }
    return NULL;
}