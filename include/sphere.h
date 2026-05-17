/**
 *
 *  \file   sphere.h
 *  \brief  Sphere class, which is a hittable object in the 3d scene.
 *
 */

/*
 *  Author:  KaMa (https://github.com/KaMa-0/
 *                 https://gitlab.com/KaMa-0/)
 *
 */


#ifndef _SPHERE_H
#define _SPHERE_H

#include <stdint.h>

#include "hittable.h"
#include "vec3.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct sphere {
    hittable_t  base;
    vec3_t      center; 
    float       radius;
} sphere_t;


sphere_t* sphere_create(void);

void sphere_init(sphere_t* s, vec3_t center, float radius);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPHERE_H */
