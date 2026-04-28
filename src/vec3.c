#include <stdlib.h>
#include <math.h>

#include "vec3.h"

vec3_t 
vec3_add(const vec3_t u, const vec3_t v)
{
        return (vec3_t){ u.x + v.x, u.y + v.y, u.z + v.z };
}

vec3_t 
vec3_sub(const vec3_t u, const vec3_t v)
{
        return (vec3_t){ u.x - v.x, u.y - v.y, u.z - v.z };
}

vec3_t 
vec3_scal(const vec3_t u, float t)
{
        return (vec3_t){ u.x * t, u.y * t, u.z * t };
}

vec3_t 
vec3_neg(const vec3_t u)
{
        return (vec3_t){ -u.x, -u.y, -u.z };
}

float 
vec3_dot(const vec3_t u, const vec3_t v)
{
        return u.x * v.x + u.y * v.y + u.z * v.z;
}

vec3_t 
vec3_cross(const vec3_t u, const vec3_t v)
{
        return (vec3_t){ u.y * v.z - v.y * u.z, 
                         u.z * v.x - v.z * u.x,
                         u.x * v.y - v.x * u.y };
}

vec3_t 
vec3_cmpnt_mult(const vec3_t u, const vec3_t v)
{
        return (vec3_t){ u.x * v.x, u.y * v.y, u.z * v.z };
}

vec3_t
vec3_unit(const vec3_t u)
{
        return vec3_scal(u, (float)(1.0 / vec3_length(u)));
}

