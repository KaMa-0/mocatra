#include <stdlib.h>

#include "vec3.h"

vec3_t 
vec3_add(vec3_t u, vec3_t v)
{
        return (vec3_t){ u.x + v.x, u.y + u.y, u.z + u.z };
}

vec3_t 
vec3_sub(vec3_t u, vec3_t v)
{
        return (vec3_t){ u.x - v.x, u.y - u.y, u.z - u.z };
}

vec3_t 
vec3_scal(vec3_t u, float t)
{
        return (vec3_t){ u.x * t, u.y * t, u.z * t };
}

vec3_t 
vec3_neg(vec3_t u)
{
        return (vec3_t){ -u.x, -u.y, -u.z };
}

float 
vec3_dot(vec3_t u, vec3_t v)
{
        return u.x * v.x + u.y * v.y + u.z * v.z;
}

vec3_t 
vec3_cross(vec3_t u, vec3_t v)
{
        return (vec3_t){ u.y * v.z - v.y * u.z, 
                         u.z * v.x - v.z * u.x,
                         u.x * v.y - v.x * u.y };
}

vec3_t 
vec3_cmpnt_mult(vec3_t u, vec3_t v)
{
        return (vec3_t){ u.x * v.x, u.y * u.y, u.z * u.z };
}

