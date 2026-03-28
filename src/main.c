#include <stdio.h>
#include <stdlib.h>

#include "da.h"

typedef struct
{
    float key;
    char value;
} kv_pair;

int main()
{
    kv_pair* pairs;

    for (size_t i = 0; i < 9; i++)
    {
        kv_pair pair = {5.f, 'a'};
        // da_append(pairs, pair);
        da_append(pairs, ((kv_pair){5.f, 'a'}));
    }

    for (size_t i = 0; i < da_size(pairs); i++)
    {
        printf("\tkey: %f, value: %c\n", pairs[i].key, pairs[i].value);
    }

    printf("count: %zu, capacity: %zu\n", da_size(pairs), da_capacity(pairs));

    return 0;
}
