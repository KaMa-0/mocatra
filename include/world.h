/**
 * @file world.h
 * @brief Utility for handling "world" list of objects which interact with ray.
 */

#ifndef _WORLD_H
#define _WORLD_H


#include "hittable_list.h"
#include "mocatra_error.h"


hittable_list_t* create_world(size_t max_obj);
void             destroy_world(hittable_list_t* world);

mocatra_error_t  init_world_cornell_box(hittable_list_t* world);
mocatra_error_t  init_world_sphere_playground(hittable_list_t* world);


#endif /* _WORLD_H */

