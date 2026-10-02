#ifndef ECS_H_
#define ECS_H_

#include <stdint.h>

#include "da.h"
#include "ht.h"
#include "q.h"

#define ENTITY_INDEX_BITS 24
#define ENTITY_INDEX_MASK ((1 << ENTITY_INDEX_BITS) - 1)

#define ENTITY_GENERATION_BITS 8
#define ENTITY_GENERATION_MASK ((1 << ENTITY_GENERATION_BITS) - 1)

typedef uint32_t EntityID;

uint32_t entity_generation(EntityID id);
uint32_t entity_index(EntityID id);
EntityID entity_make(uint32_t index, uint32_t generation);

#define ECS_MINIMUM_FREE_INDICES 256

typedef struct
{
    uint32_t* items;
    size_t count;
    size_t capacity;
} Generations;

typedef struct
{
    uint32_t* items;
    size_t count;
    size_t capacity;
    size_t head;
} FreeIndices;

typedef struct
{
    EntityID* items;
    size_t count;
    size_t capacity;
} Entities;

typedef struct
{
    float x, y;
} Vec2;

typedef struct
{
    Vec2 position;
    Vec2 scale;
    float rotation;
} Transform;

typedef struct Sparse
{
    char* key;
    size_t value;
    struct Sparse* next;
} Sparse;

// typedef void (*TransformSystem)(Transform* transform, void* user_data);

#define ComponentArray(C)                                                                                              \
    struct                                                                                                             \
    {                                                                                                                  \
        C* items;                                                                                                      \
        size_t count;                                                                                                  \
        size_t capacity;                                                                                               \
    }

#define ComponentStore(C)                                                                                              \
    struct                                                                                                             \
    {                                                                                                                  \
        ComponentArray(C) components;                                                                                  \
        Sparse** sparse_index;                                                                                         \
        Entities entities;                                                                                             \
    }

typedef struct ComponentStores
{
    char* key;
    void* value;
    struct ComponentStores* next;
} ComponentStores;

typedef struct
{
    Generations generations;
    FreeIndices free_indices;

    ComponentStore(Transform) transform_store;

    ComponentStores** component_stores;

    void* user_data;
} ECS;

int ecs_init(ECS* ecs);
void ecs_shutdown(ECS* ecs);

void ecs_update(ECS* ecs);

void ecs_set_user_data(ECS* ecs, void* user_data);
void* ecs_get_user_data(ECS* ecs);

EntityID ecs_create_entity(ECS* ecs);
int ecs_is_valid_entity(ECS* ecs, EntityID id);
void ecs_destroy_entity(ECS* ecs, EntityID id);

#define ecs_add_component(ecs, id, C, c)                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        ComponentStore(C) * store;                                                                                     \
        void** store_ptr = NULL;                                                                                       \
        ht_get(ecs.component_stores, #C, store_ptr);                                                                   \
        if (store_ptr == NULL)                                                                                         \
        {                                                                                                              \
            ComponentStore(C)* store = malloc(sizeof(*store));                                                         \
            ht_init(store->sparse_index);                                                                              \
            ht_insert(ecs.component_stores, #C, (void*)store);                                                         \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            store = (ComponentStore(C)*)(*store_ptr);                                                                  \
        }                                                                                                              \
        size_t dense_index = store->components.count;                                                                  \
        da_append(&store->components, c);                                                                              \
        da_append(&store->entities, id);                                                                               \
        char key[32];                                                                                                  \
        snprintf(key, sizeof(key), "%d", entity_index(id));                                                            \
        ht_insert(store->sparse_index, key, dense_index);                                                              \
    } while (0)

void ecs_add_transform(ECS* ecs, EntityID id, Transform transform);
void ecs_remove_transform(ECS* ecs, EntityID id);

// void ecs_set_transform_system(ECS* ecs, TransformSystem system);

#endif // ECS_H_
