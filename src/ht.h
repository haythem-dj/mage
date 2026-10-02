#ifndef HT_H_
#define HT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HT_HASH_TABLE_SIZE 64

#define ht_hash(key, hash)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        uint8_t* k = (uint8_t*)(key);                                                                                  \
        uint32_t h = 5381;                                                                                             \
        while (*k) h = h * 33 ^ (uint32_t)*k++;                                                                        \
        h = h % HT_HASH_TABLE_SIZE;                                                                                    \
        *hash = h;                                                                                                     \
    } while (0)

#define ht_init(ht)                                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        (ht) = calloc(HT_HASH_TABLE_SIZE, sizeof(*(ht)));                                                              \
        if (!(ht))                                                                                                     \
        {                                                                                                              \
            perror("calloc failed.");                                                                                  \
            exit(EXIT_FAILURE);                                                                                        \
        }                                                                                                              \
    } while (0)

#define ht_insert(ht, k, val)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        uint32_t index;                                                                                                \
        ht_hash((k), &index);                                                                                          \
        __typeof__(*(ht)) e = ht[index];                                                                               \
        while (e)                                                                                                      \
        {                                                                                                              \
            if (strcmp(e->key, (const char*)(k)) == 0)                                                                 \
            {                                                                                                          \
                e->value = val;                                                                                        \
                break;                                                                                                 \
            }                                                                                                          \
            e = e->next;                                                                                               \
        }                                                                                                              \
        if (!e)                                                                                                        \
        {                                                                                                              \
            __typeof__(*(ht)) entry = malloc(sizeof(**(ht)));                                                          \
            entry->key = strdup((const char*)(k));                                                                     \
            entry->value = val;                                                                                        \
            entry->next = (ht)[index];                                                                                 \
            (ht)[index] = entry;                                                                                       \
        }                                                                                                              \
    } while (0)

#define ht_get(ht, k, val)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        uint32_t index;                                                                                                \
        ht_hash((k), &index);                                                                                          \
        __typeof__(*(ht)) e = (ht)[index];                                                                             \
        while (e)                                                                                                      \
        {                                                                                                              \
            if (strcmp(e->key, (const char*)(k)) == 0)                                                                 \
            {                                                                                                          \
                val = &e->value;                                                                                       \
                break;                                                                                                 \
            }                                                                                                          \
            e = e->next;                                                                                               \
        }                                                                                                              \
    } while (0)

#define ht_delete(ht, k)                                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        uint32_t index;                                                                                                \
        ht_hash((k), &index);                                                                                          \
        __typeof__(*(ht)) e = (ht)[index];                                                                             \
        __typeof__(*(ht)) prv = NULL;                                                                                  \
        while (e)                                                                                                      \
        {                                                                                                              \
            if (strcmp(e->key, (const char*)(k)) == 0)                                                                 \
            {                                                                                                          \
                if (prv == NULL) (ht)[index] = e->next;                                                                \
                else                                                                                                   \
                    prv = e->next;                                                                                     \
                free(e->key);                                                                                          \
                free(e);                                                                                               \
                break;                                                                                                 \
            }                                                                                                          \
            prv = e;                                                                                                   \
            e = e->next;                                                                                               \
        }                                                                                                              \
    } while (0)

#endif // HT_H_
