#ifndef H_H_
#define H_H_

#define H_VERSION_MAJOR 0
#define H_VERSION_MINOR 1
#define H_VERSION_PATCH 3
#define H_VERSION_STRING "0.1.3"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// ===== Dynamic Array =====

#define H_DA_INIT_CAPACITY 10
#define H_DA_CAPACITY_FACTOR 2

typedef struct
{
    size_t count;
    size_t capacity;
} H_DA_Header;

#define h_da_header(arr) ((H_DA_Header*)(arr) - 1)

#define h_da_reserve(arr, n)                                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        H_DA_Header* h = (H_DA_Header*)malloc(sizeof(H_DA_Header) + sizeof(*(arr)) * n);                               \
        if (!h)                                                                                                        \
        {                                                                                                              \
            perror("malloc failed");                                                                                   \
            exit(EXIT_FAILURE);                                                                                        \
        }                                                                                                              \
        h->capacity = n;                                                                                               \
        h->count = 0;                                                                                                  \
        (arr) = (void*)(h + 1);                                                                                        \
    } while (0)

#define h_da_append(arr, x)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((arr) == NULL)                                                                                             \
        {                                                                                                              \
            h_da_reserve(arr, H_DA_INIT_CAPACITY);                                                                     \
        }                                                                                                              \
        H_DA_Header* h = h_da_header((arr));                                                                           \
        if (h->count >= h->capacity)                                                                                   \
        {                                                                                                              \
            h->capacity *= H_DA_CAPACITY_FACTOR;                                                                       \
            h = realloc(h, sizeof(H_DA_Header) + h->capacity * sizeof(*(arr)));                                        \
            if (!h)                                                                                                    \
            {                                                                                                          \
                perror("realloc failed");                                                                              \
                exit(EXIT_FAILURE);                                                                                    \
            }                                                                                                          \
            (arr) = (void*)(h + 1);                                                                                    \
        }                                                                                                              \
        (arr)[h->count++] = (x);                                                                                       \
    } while (0)

#define h_da_size(arr) ((arr) != NULL ? h_da_header((arr))->count : 0)
#define h_da_capacity(arr) ((arr) != NULL ? h_da_header((arr))->capacity : 0)

#define h_da_free(arr)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        free(h_da_header((arr)));                                                                                      \
        (arr) = NULL;                                                                                                  \
    } while (0)

// ===== Linked List =====

#define H_Linked_List(T)                                                                                               \
    struct                                                                                                             \
    {                                                                                                                  \
        T value;                                                                                                       \
        void* next;                                                                                                    \
    }*

#define h_ll_push_front(ll, v)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        __typeof__(ll) node = malloc(sizeof(*(ll)));                                                                   \
        if (!node)                                                                                                     \
        {                                                                                                              \
            perror("malloc failed.");                                                                                  \
            exit(EXIT_FAILURE);                                                                                        \
        }                                                                                                              \
        node->value = v;                                                                                               \
        node->next = (ll);                                                                                             \
        (ll) = node;                                                                                                   \
    } while (0)

#define h_ll_push_back(ll, v)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        __typeof__(ll) node = malloc(sizeof(*(ll)));                                                                   \
        if (!node)                                                                                                     \
        {                                                                                                              \
            perror("malloc failed.");                                                                                  \
            exit(EXIT_FAILURE);                                                                                        \
        }                                                                                                              \
        node->value = v;                                                                                               \
        node->next = NULL;                                                                                             \
        if ((ll) == NULL) (ll) = node;                                                                                 \
        else                                                                                                           \
        {                                                                                                              \
            __typeof__(ll) cur = (ll);                                                                                 \
            while (cur->next) cur = cur->next;                                                                         \
            cur->next = node;                                                                                          \
        }                                                                                                              \
    } while (0)

#define h_ll_pop_front(ll)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((ll) != NULL)                                                                                              \
        {                                                                                                              \
            __typeof__(ll) dead = (ll);                                                                                \
            (ll) = (ll)->next;                                                                                         \
            free(dead);                                                                                                \
        }                                                                                                              \
    } while (0)

#define h_ll_pop_back(ll)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((ll))                                                                                                      \
        {                                                                                                              \
            __typeof__(ll) cur = (ll);                                                                                 \
            __typeof__(ll) prv;                                                                                        \
            while ((cur)->next)                                                                                        \
            {                                                                                                          \
                prv = cur;                                                                                             \
                cur = cur->next;                                                                                       \
            }                                                                                                          \
            free(cur);                                                                                                 \
            prv->next = NULL;                                                                                          \
        }                                                                                                              \
    } while (0)

#define h_ll_free(ll)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        __typeof__(ll) cur = ll;                                                                                       \
        while (cur)                                                                                                    \
        {                                                                                                              \
            cur = ll->next;                                                                                            \
            free(ll);                                                                                                  \
            ll = cur;                                                                                                  \
        }                                                                                                              \
    } while (0)

// ===== Hash Table =====

#define H_HT_TABLE_SIZE 64

#define h_hash(k)                                                                                                      \
    ({                                                                                                                 \
        uint32_t h = 5381;                                                                                             \
        while (*(k)) h = h * 33 ^ (uint32_t)*(k)++;                                                                    \
        h % H_HT_TABLE_SIZE;                                                                                           \
    })

#define H_Hash_Table(T, name)                                                                                          \
    struct Entry_##name                                                                                                \
    {                                                                                                                  \
        char* key;                                                                                                     \
        T value;                                                                                                       \
        struct Entry_##name* next;                                                                                     \
    }**

#define h_ht_init(ht)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        (ht) = calloc(H_HT_TABLE_SIZE, sizeof(*(ht)));                                                                 \
        if (!(ht))                                                                                                     \
        {                                                                                                              \
            perror("calloc failed.");                                                                                  \
            exit(EXIT_FAILURE);                                                                                        \
        }                                                                                                              \
    } while (0)

#define h_ht_insert(ht, k, v)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((ht) == NULL) h_ht_init(ht);                                                                               \
        uint32_t index = h_hash(k);                                                                                    \
        __typeof__(*(ht)) e = ht[index];                                                                               \
        while (e)                                                                                                      \
        {                                                                                                              \
            if (strcmp(e->key, (k)) == 0)                                                                              \
            {                                                                                                          \
                e->value = v;                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
            e = e->next;                                                                                               \
        }                                                                                                              \
        if (!e)                                                                                                        \
        {                                                                                                              \
            __typeof__(*(ht)) entry = malloc(sizeof(*entry));                                                          \
            entry->key = strdup(k);                                                                                    \
            entry->value = (v);                                                                                        \
            entry->next = (ht)[index];                                                                                 \
            (ht)[index] = entry;                                                                                       \
        }                                                                                                              \
    } while (0)

#define h_ht_get(ht, k)                                                                                                \
    ({                                                                                                                 \
        uint32_t index = h_hash(k);                                                                                    \
        __typeof__(*(ht)) e = (ht)[index];                                                                             \
        while (e)                                                                                                      \
        {                                                                                                              \
            if (strcmp(e->key, (k)) == 0) break;                                                                       \
            e = e->next;                                                                                               \
        }                                                                                                              \
        e ? &e->value : NULL;                                                                                          \
    })

#define h_ht_delete(ht, k)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        uint32_t index = h_hash(k);                                                                                    \
        __typeof__(*(ht)) e = (ht)[index];                                                                             \
        __typeof__(*(ht)) prv = NULL;                                                                                  \
        while (e)                                                                                                      \
        {                                                                                                              \
            if (strcmp(e->key, (k)) == 0)                                                                              \
            {                                                                                                          \
                if (prv == NULL) (ht)[index] = e->next;                                                                \
                else                                                                                                   \
                    prv->next = e->next;                                                                               \
                free(e->key);                                                                                          \
                free(e);                                                                                               \
                break;                                                                                                 \
            }                                                                                                          \
            prv = e;                                                                                                   \
            e = e->next;                                                                                               \
        }                                                                                                              \
    } while (0)

#define h_ht_free(ht)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        for (size_t i = 0; i < H_HT_TABLE_SIZE; i++)                                                                   \
        {                                                                                                              \
            __typeof__(*(ht)) e = (ht)[i];                                                                             \
            while (e)                                                                                                  \
            {                                                                                                          \
                __typeof__(*(ht)) dead = e;                                                                            \
                e = e->next;                                                                                           \
                free(dead->key);                                                                                       \
                free(dead);                                                                                            \
            }                                                                                                          \
        }                                                                                                              \
        free(ht);                                                                                                      \
        (ht) = NULL;                                                                                                   \
    } while (0)

#endif
