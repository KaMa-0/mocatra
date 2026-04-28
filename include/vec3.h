/**
 *
 *  \file   vec3.h
 *  \brief  Utilities for mathematical operations on vectors in 3d space.
 *
 */

/*
 *  Author:  KaMa (https://github.com/KaMa-0/
 *                 https://gitlab.com/KaMa-0/)
 *
 */


#ifndef _VEC3_H
#define _VEC3_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct vec3 {
        float x, y, z;
} vec3_t;


/* helper functions for derivation of length needed for unit vector calc */

static inline float
vec3_len_squared(vec3_t u)
{
        return u.x * u.x + u.y * u.y + u.z * u.z;
}

static inline float
vec3_length(vec3_t u)
{
        return sqrt(vec3_len_squared(u));
}

/* --------------------------------------------------------------------- */


vec3_t vec3_add(vec3_t u, vec3_t v);
vec3_t vec3_sub(vec3_t u, vec3_t v);

vec3_t vec3_scal(vec3_t u, float t);
vec3_t vec3_neg(vec3_t u);

float vec3_dot(vec3_t u, vec3_t v);
vec3_t vec3_cross(vec3_t u, vec3_t v);

vec3_t vec3_cmpnt_mult(vec3_t u, vec3_t v);

vec3_t vec3_unit(vec3_t u);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _VEC3_H */
