/**
 * \file        ray.c
 */

#include "core/ray.h"

vec3_t
ray_at(const ray_t r, float t)
{
        return vec3_add(r.orig, vec3_scal(r.dir, t));
}
