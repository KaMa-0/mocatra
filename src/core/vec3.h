/**
 * \file       vec3.h
 * \brief      Utilities for mathematical operations on vectors in 3d space.
 */

#ifndef _VEC3_H
#define _VEC3_H

#include <math.h>

typedef struct vec3 {
        float x, y, z;
} vec3_t;

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

static inline vec3_t
v3(float x, float y, float z)
{
        return (vec3_t){x, y, z};
}

static inline vec3_t
vec3_add(const vec3_t u, const vec3_t v)
{
        return (vec3_t){u.x + v.x, u.y + v.y, u.z + v.z};
}

static inline vec3_t
vec3_sub(const vec3_t u, const vec3_t v)
{
        return (vec3_t){u.x - v.x, u.y - v.y, u.z - v.z};
}

static inline vec3_t
vec3_scal(const vec3_t u, float t)
{
        return (vec3_t){u.x * t, u.y * t, u.z * t};
}

static inline vec3_t
vec3_neg(const vec3_t u)
{
        return (vec3_t){-u.x, -u.y, -u.z};
}

static inline float
vec3_dot(const vec3_t u, const vec3_t v)
{
        return u.x * v.x + u.y * v.y + u.z * v.z;
}

static inline vec3_t
vec3_cross(const vec3_t u, const vec3_t v)
{
        return (vec3_t){u.y * v.z - v.y * u.z, u.z * v.x - v.z * u.x,
                        u.x * v.y - v.x * u.y};
}

static inline vec3_t
vec3_cmpnt_mult(const vec3_t u, const vec3_t v)
{
        return (vec3_t){u.x * v.x, u.y * v.y, u.z * v.z};
}

static inline vec3_t
vec3_unit(const vec3_t u)
{
        return vec3_scal(u, (float)(1.0 / vec3_length(u)));
}

static inline vec3_t
vec3_reflect(const vec3_t v, const vec3_t n)
{
        return vec3_sub(v, vec3_scal(n, 2.0f * vec3_dot(v, n)));
}

#endif /* _VEC3_H */
