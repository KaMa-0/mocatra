/**
 * \file        quad.h
 * \brief       Implementation of planar "quad"-rilateral hittable primitive.
 */

#ifndef _QUAD_H
#define _QUAD_H

#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "vec3.h"

typedef struct quad {
        hittable_t base;
        vec3_t     q;
        vec3_t     u;
        vec3_t     v;
        material_t mat;
        vec3_t     normal;
        vec3_t     w;
        float      d;
} quad_t;

quad_t* quad_create(void);

void quad_init(quad_t* quad, vec3_t q, vec3_t u, vec3_t v, material_t mat);

#endif /* _QUAD_H */
