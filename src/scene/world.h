/**
 * \file        world.h
 * \brief       Utility for handling list of objects which interact with ray.
 *              This list of objects is defined as a "world" (or scene).
 */

#ifndef _WORLD_H
#define _WORLD_H

#include "geometry/hittable_list.h"
#include "mocatra.h"

#define WORLD_CORNELL_WALL_COUNT   6
#define WORLD_CORNELL_OBJECT_COUNT (WORLD_CORNELL_WALL_COUNT + 2 * 6)

hittable_list_t* world_create(size_t max_obj);
void             world_destroy(hittable_list_t* world);

mocatra_error_t init_world_cornell_box(hittable_list_t* world);

#endif /* _WORLD_H */
