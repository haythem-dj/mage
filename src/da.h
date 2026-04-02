#ifndef DA_H_
#define DA_H_

#include <stdlib.h>

#define INIT_CAPACITY 10
#define CAPACITY_FACTOR 2

typedef struct
{
    size_t count;
    size_t capacity;
} Header;

#define da_init(arr)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        Header* h = (Header*)malloc(sizeof(Header) + INIT_CAPACITY * sizeof(*(arr)));                                  \
        if (!h)                                                                                                        \
        {                                                                                                              \
            perror("malloc failed");                                                                                   \
            exit(EXIT_FAILURE);                                                                                        \
        }                                                                                                              \
        h->capacity = INIT_CAPACITY;                                                                                   \
        h->count = 0;                                                                                                  \
        (arr) = (void*)(h + 1);                                                                                        \
    } while (false)

#define da_append(arr, x)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((arr) == NULL)                                                                                             \
        {                                                                                                              \
            Header* h = (Header*)malloc(sizeof(Header) + INIT_CAPACITY * sizeof(*(arr)));                              \
            h->capacity = INIT_CAPACITY;                                                                               \
            h->count = 0;                                                                                              \
            (arr) = (void*)(h + 1);                                                                                    \
        }                                                                                                              \
        Header* h = ((Header*)(arr) - 1);                                                                              \
        if (h->count >= h->capacity)                                                                                   \
        {                                                                                                              \
            h->capacity *= CAPACITY_FACTOR;                                                                            \
            h = realloc(h, sizeof(Header) + h->capacity * sizeof(*(arr)));                                             \
            if (!h)                                                                                                    \
            {                                                                                                          \
                perror("realloc failed");                                                                              \
                exit(EXIT_FAILURE);                                                                                    \
            }                                                                                                          \
            (arr) = (void*)(h + 1);                                                                                    \
        }                                                                                                              \
        (arr)[h->count++] = (x);                                                                                       \
    } while (false)

#define da_size(arr) ((Header*)(arr) - 1)->count
#define da_capacity(arr) ((Header*)(arr) - 1)->capacity
#endif // DA_H_
