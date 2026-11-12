/**
 *  \file       ray.h
 *  \brief      Utilities for a Ray.
 */

#ifndef _RAY_H
#define _RAY_H

#include "core/ray.h"
#include "core/vec3.h"

typedef struct ray {
        vec3_t orig;
        vec3_t dir;
} ray_t;

vec3_t ray_at(const ray_t r, float t);

#endif /* _RAY_H */
