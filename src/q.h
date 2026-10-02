#ifndef Q_H_
#define Q_H_

#include "da.h"

#define q_empty(q) (da_empty(q))

#define q_enqueue(q, val_)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((q)->head + (q)->count >= (q)->capacity)                                                                   \
        {                                                                                                              \
            if ((q)->capacity == 0) (q)->capacity = DA_INIT_CAPACITY;                                                  \
            while ((q)->head + (q)->count >= (q)->capacity) (q)->capacity *= DA_CAPACITY_FACTOR;                       \
            (q)->items = realloc((q)->items, (q)->capacity * sizeof(*(q)->items));                                     \
        }                                                                                                              \
        (q)->items[(q)->head + (q)->count++] = (val_);                                                                 \
    } while (0)

#define q_dequeue(q)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!q_empty((q)))                                                                                             \
        {                                                                                                              \
            (q)->head++;                                                                                               \
            (q)->count--;                                                                                              \
        }                                                                                                              \
    } while (0)

#define q_peek(q) ((q)->items[(q)->head])

#define q_free(q) da_free((q))

#endif // Q_H_
