/*
 * \file        quad.c
 * */

#include "quad.h"

#include <math.h>
#include <stdlib.h>

#include "mocatra.h"

static uint8_t
quad_hit(const hittable_t* self, const ray_t r, float t_min, float t_max,
         hit_record_t* rec)
{
        const quad_t* quad;
        vec3_t intersection_point, planar_hitpoint_vector;
        float denom, t, alpha, beta;

        quad = (const quad_t*)self;

        denom = vec3_dot(quad->normal, r.dir);

        if (fabsf(denom) < 1e-8f) {
                return 0;
        }

        t = (quad->d - vec3_dot(quad->normal, r.orig)) / denom;
        if (t < t_min || t > t_max) {
                return 0;
        }

        intersection_point = vec3_add(r.orig, vec3_scal(r.dir, t));
        planar_hitpoint_vector = vec3_sub(intersection_point, quad->q);

        alpha = vec3_dot(quad->w, vec3_cross(planar_hitpoint_vector, quad->v));
        beta = vec3_dot(quad->w, vec3_cross(quad->u, planar_hitpoint_vector));

        if (alpha < 0.0f || alpha > 1.0f || beta < 0.0f || beta > 1.0f) {
                return 0;
        }

        rec->t = t;
        rec->p = intersection_point;
        rec->mat = quad->mat;

        hit_record_set_face_normal(rec, r, quad->normal);

        return 1;
}

static void
quad_destroy(hittable_t* self)
{
        free(self);
        self = NULL;
}

static const hittable_vtable_t quad_vtable = {
        .hit = quad_hit,
        .destroy = quad_destroy,
};

quad_t*
quad_create(void)
{
        quad_t* quad;

        quad = malloc(sizeof(quad_t));
        if (quad == NULL) {
                printf("[ERROR] Memory Allocation failed for quad_t\n");
                return NULL;
        }

        return quad;
}

void
quad_init(quad_t* quad, vec3_t q, vec3_t u, vec3_t v, material_t mat)
{
        vec3_t n;

        if (quad == NULL) {
                return;
        }

        quad->base.vtable = &quad_vtable;
        quad->q = q;
        quad->u = u;
        quad->v = v;
        quad->mat = mat;

        n = vec3_cross(u, v);
        quad->normal = vec3_unit(n);
        quad->d = vec3_dot(quad->normal, q);

        quad->w = vec3_scal(n, 1.0f / vec3_len_squared(n));
}
