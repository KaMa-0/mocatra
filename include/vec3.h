/**
 *
 *  \file   vec3.h
 *  \brief  Utility for writing PPM Images
 *
 */

/*
 *  Author:  KaMa (https://github.com/KaMa-0/
 *                 https://gitlab.com/KaMa-0/)
 *
 */


#ifndef _VEC3_H
#define _VEC3_H


typedef struct vec3 {
        double x, y, z;
} vec3_t;


static inline vec3_t vec3_add(vec3_t u, vec3_t v);
static inline vec3_t vec3_sub(vec3_t u, vec3_t v);

static inline vec3_t vec3_scal(vec3_t u, double t);
static inline vec3_t vec3_neg(vec3_t u);

static inline double vec3_dot(vec3_t u, vec3_t v);
static inline vec3_t vec3_cross(vec3_t u, vec3_t v);

static inline vec3_t vec3_cmpnt_mult(vec3_t u, vec3_t v);

#endif /* _VEC3_H */
