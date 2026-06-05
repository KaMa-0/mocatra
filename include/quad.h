/**
 *
 *  \file   quad.h
 *  \brief  tbd.
 *
 */

/*
 *  Author:  KaMa (https://github.com/KaMa-0/
 *                 https://gitlab.com/KaMa-0/)
 *
 */


#ifndef _QUAD_H
#define _QUAD_H

#include "vec3.h"
#include "hittable.h"
#include "material.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct quad {
    hittable_t  base;
    vec3_t      q;
    vec3_t      u;
    vec3_t      v;
    material_t  mat;
    vec3_t      normal;
    vec3_t      w;
    float       d;
} quad_t;


quad_t* quad_create(void);

void    quad_init(quad_t* quad, vec3_t q, vec3_t u, vec3_t v, material_t mat);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _QUAD_H */
