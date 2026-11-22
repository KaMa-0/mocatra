/**
 * \file        quad.h
 * \brief       Implementation of planar "quad"-rilateral hittable primitive.
 */

#ifndef _QUAD_H
#define _QUAD_H

#include "core/vec3.h"
#include "geometry/hittable_list.h"
#include "material/material.h"

typedef struct quad {
        hittable_t base;
        vec3_t     q;
        vec3_t     u;
        vec3_t     v;
        vec3_t     w;
        vec3_t     normal;
        float      d;
        material_t mat;
} quad_t;

quad_t*         quad_create(vec3_t q, vec3_t u, vec3_t v, material_t mat);
mocatra_error_t quad_init(quad_t* quad, vec3_t q, vec3_t u, vec3_t v,
                          material_t mat);

/* for convenience, extending the functionality of hittable_list_add for quad */
mocatra_error_t hittable_list_add_quad(hittable_list_t* list, vec3_t q,
                                       vec3_t u, vec3_t v, material_t mat);

#endif /* _QUAD_H */
