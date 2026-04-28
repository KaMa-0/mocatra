/**
 *
 *  \file   ray.h
 *  \brief  Utilities for a Ray.
 *
 */

/*
 *  Author:  KaMa (https://github.com/KaMa-0/
 *                 https://gitlab.com/KaMa-0/)
 *
 */


#ifndef _RAY_H
#define _RAY_H


#include "vec3.h"
#include "image.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct ray {
        vec3_t orig;
        vec3_t dir;
} ray_t;


const vec3_t ray_at(const ray_t* r, float t);

pixel_t ray_color(const ray_t t);

float hit_sphere(const vec3_t center, float radius, const ray_t r);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RAY_H */
