/**
 *
 *  \file   hittable.h
 *  \brief  Abstract hittabl class for any object which a ray can "hit".
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
#include "ray.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct hit_record {
    vec3_t p;
    vec3_t normal;
    float  t;
    int8_t front_face;
} hit_record_t;

typedef struct hittable hittable_t;

typedef struct hittable_vtable {
    uint8_t (*hit) (const hittable_t* self, const ray_t r, 
                    float t_min, float t_max, hit_record_t* rec);
    void (*destroy)(hittable_t* self);
} hittable_vtable_t;

struct hittable {
    const hittable_vtable_t* vtable;
};

static inline void
hit_record_set_face_normal(hit_record_t* rec, const ray_t r, 
                           const vec3_t outward_normal)
{
    rec->front_face = vec3_dot(r.dir, outward_normal) < 0;
    if (rec->front_face)
        rec->normal = outward_normal;
    else
        rec->normal = vec3_neg(outward_normal);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HITTABLE_H */
