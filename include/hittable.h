/**
 *
 *  \file   hittable.h
 *  \brief  Abstract hittable class for any object which a ray can "hit".
 *
 */

/*
 *  Author:  KaMa (https://github.com/KaMa-0/
 *                 https://gitlab.com/KaMa-0/)
 *
 */


#ifndef _HITTABLE_H
#define _HITTABLE_H

#include <stdint.h>

#include "vec3.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct hit_record {
    vec3_t p;
    vec3_t normal;
    float  t;
} hit_record_t;

typedef struct hittable hittable_t;

typedef struct hittable_vtable {
    uint8_t (*hit)(const hittable_t* self, const ray_t* r, 
                   float t_min, float t_max, hit_record_t* rec);
} hittable_vtable_t;

struct hittable {
    const hittable_vtable;
};


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HITTABLE_H */
