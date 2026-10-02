#include "ecs.h"

#include "da.h"
#include "ht.h"
#include "q.h"

uint32_t entity_generation(EntityID id) { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }
uint32_t entity_index(EntityID id) { return id & ENTITY_INDEX_MASK; }
EntityID entity_make(uint32_t index, uint32_t generation) { return (generation << ENTITY_INDEX_BITS) | index; }

int ecs_init(ECS* ecs)
{
    ht_init(ecs->transform_store.sparse_index);
    ht_init(ecs->component_stores);

    ecs->user_data = NULL;
    return 1;
}

void ecs_shutdown(ECS* ecs)
{
    da_free(&ecs->generations);
    q_free(&ecs->free_indices);
}

void ecs_update(ECS* ecs)
{
    // if (ecs->transform_store.system)
    // {
    //     for (size_t i = 0; i < ecs->transform_store.components.count; i++)
    //         ecs->transform_store.system(&ecs->transform_store.components.items[i], ecs->user_data);
    // }
}

void ecs_set_user_data(ECS* ecs, void* user_data) { ecs->user_data = user_data; }
void* ecs_get_user_data(ECS* ecs) { return ecs->user_data; }

EntityID ecs_create_entity(ECS* ecs)
{
    uint32_t index;
    if (ecs->free_indices.count > ECS_MINIMUM_FREE_INDICES)
    {
        index = q_peek(&ecs->free_indices);
        q_dequeue(&ecs->free_indices);
    }
    else
    {
        da_append(&ecs->generations, 0);
        index = ecs->generations.count - 1;
    }

    return entity_make(index, ecs->generations.items[index]);
}

int ecs_is_valid_entity(ECS* ecs, EntityID id)
{
    return (ecs->generations.items[entity_index(id)] == entity_generation(id));
}

void ecs_destroy_entity(ECS* ecs, EntityID id)
{
    if (!ecs_is_valid_entity(ecs, id)) return;

    ecs->generations.items[entity_index(id)]++;
    q_enqueue(&ecs->free_indices, entity_index(id));
}

void ecs_add_transform(ECS* ecs, EntityID id, Transform transform)
{
    size_t dense_index = ecs->transform_store.components.count;

    da_append(&ecs->transform_store.components, transform);
    da_append(&ecs->transform_store.entities, id);

    char key[32];
    snprintf(key, sizeof(key), "%d", entity_index(id));

    ht_insert(ecs->transform_store.sparse_index, key, dense_index);
}

void ecs_remove_transform(ECS* ecs, EntityID id)
{
    char key[32];
    snprintf(key, sizeof(key), "%d", entity_index(id));

    size_t* array_index;
    ht_get(ecs->transform_store.sparse_index, key, array_index);

    char moved_key[32];

    if (*array_index != ecs->transform_store.components.count - 1)
    {
        ecs->transform_store.components.items[*array_index] =
            ecs->transform_store.components.items[ecs->transform_store.components.count - 1];
        EntityID last = ecs->transform_store.entities.items[ecs->transform_store.entities.count - 1];
        ecs->transform_store.entities.items[*array_index] = last;

        snprintf(moved_key, sizeof(moved_key), "%d", last);

        ht_insert(ecs->transform_store.sparse_index, moved_key, *array_index);
    }

    da_pop(&ecs->transform_store.components);
    da_pop(&ecs->transform_store.entities);
    ht_delete(ecs->transform_store.sparse_index, key);
}

// void ecs_set_transform_system(ECS* ecs, TransformSystem system) { ecs->transform_store.system = system; }
