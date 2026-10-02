#ifndef DA_ARRAY_H_
#define DA_ARRAY_H_

#include <stdio.h>
#include <stdlib.h>

#define DA_INIT_CAPACITY 10
#define DA_CAPACITY_FACTOR 2

#define da_empty(arr) ((arr)->count == 0)

#define da_reserve(arr, expected_capacity)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((expected_capacity) > (arr)->capacity)                                                                     \
        {                                                                                                              \
            if ((arr)->capacity == 0) (arr)->capacity = DA_INIT_CAPACITY;                                              \
            while ((expected_capacity) > (arr)->capacity) (arr)->capacity *= DA_CAPACITY_FACTOR;                       \
            (arr)->items = realloc((arr)->items, (arr)->capacity * sizeof(*(arr)->items));                             \
        }                                                                                                              \
    } while (0)

#define da_append(arr, x)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        da_reserve((arr), (arr)->count + 1);                                                                           \
        (arr)->items[(arr)->count++] = (x);                                                                            \
    } while (0)

#define da_pop(arr)                                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((arr)->count > 0) (arr)->count--;                                                                          \
    } while (0)

#define da_free(arr)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        free((arr)->items);                                                                                            \
    } while (0)

#endif // DA_ARRAY_H_
