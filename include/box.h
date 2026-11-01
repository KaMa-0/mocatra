/**
 * \file        box.h
 * \brief       Box primitive construct from 6 quads (6-sided box).
 */

#ifndef _BOX_H
#define _BOX_H

#include "hittable_list.h"
#include "vec3.h"

typedef struct quad_face_def {
        vec3_t origin;
        vec3_t u;
        vec3_t v;
} quad_face_def_t;

void hittable_list_add_box(hittable_list_t* list, vec3_t p0, vec3_t p1,
                           float angle_degrees, vec3_t transformation,
                           material_t mat);

#endif /* _BOX_H */
