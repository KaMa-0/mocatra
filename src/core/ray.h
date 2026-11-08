/**
 *  \file       ray.h
 *  \brief      Utilities for a Ray.
 */

#ifndef _RAY_H
#define _RAY_H

#include "core/vec3.h"

typedef struct hittable hittable_t;

typedef struct ray {
        vec3_t orig;
        vec3_t dir;
} ray_t;

vec3_t ray_at(const ray_t r, float t);

vec3_t ray_color(ray_t r, int depth, const hittable_t* world);

#endif /* _RAY_H */
