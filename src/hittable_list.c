#include <stdlib.h>

#include "hittable_list.h"

static uint8_t
hittable_list_hit(const hittable_t* self, const ray_t r, 
                  float t_min, float t_max, hit_record_t* rec)
{
        hittable_list_t* list;
        hit_record_t     temp_rec;
        uint8_t          hit_anything;
        float            closest_so_far;

        list            = (hittable_list_t*)self;
        hit_anything    = 0;
        closest_so_far  = t_max;

        for (int i = 0; i < list->size; i++) {
                if (list->objects[i]->vtable->hit(list->objects[i],
                                                  r, t_min, closest_so_far,
                                                  &temp_rec)) {
                        hit_anything    = 1;
                        closest_so_far  = temp_rec.t;
                        *rec            = temp_rec;
                }
        }

        return hit_anything;
}

static const hittable_vtable_t list_vtable = { .hit = hittable_list_hit };

void 
hittable_list_init(hittable_list_t* list, int initial_capacity)
{
        list->base.vtable = &list_vtable; 
        list->size        = 0;
        list->capacity    = initial_capacity;
        list->objects     = malloc(sizeof(hittable_t*) * initial_capacity);
}

void 
hittable_list_add(hittable_list_t* list, hittable_t* object)
{
        if (list->size == list->capacity) {
                list->capacity *= 2;
                list->objects  = realloc(list->objects, 
                                         sizeof(hittable_t*) * list->capacity);
        }
        list->objects[list->size++] = object;
}

void 
hittable_list_cleanup(hittable_list_t* list)
{
        if (list == NULL) return;

        for (int i = 0; i < list->size; ++i) {
                if (list->objects[i]->vtable->destroy) {
                        list->objects[i]->vtable->destroy(list->objects[i]);
                }
        }

        free(list->objects);
    
        list->objects = NULL;
        list->size = 0;
        list->capacity = 0;
}

