/*
 * \file        quad.c
 * */

#include "geometry/quad.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define PARALLEL_EPSILON   1e-8f
#define DEGENERATE_EPSILON 1e-12f

static bool
quad_hit(const hittable_t* self, const ray_t r, float t_min, float t_max,
         hit_record_t* rec)
{
        const quad_t* quad;
        vec3_t        hit_point, planar;
        float         denom, t, alpha, beta;

        quad = (const quad_t*)self;

        denom = vec3_dot(quad->normal, r.dir);
        if (fabsf(denom) < PARALLEL_EPSILON) {
                return false;
        }

        t = (quad->d - vec3_dot(quad->normal, r.orig)) / denom;
        if (t < t_min || t > t_max) {
                return false;
        }

        hit_point = vec3_add(r.orig, vec3_scal(r.dir, t));
        planar    = vec3_sub(hit_point, quad->q);

        alpha = vec3_dot(quad->w, vec3_cross(planar, quad->v));
        beta  = vec3_dot(quad->w, vec3_cross(quad->u, planar));

        if (alpha < 0.0f || alpha > 1.0f || beta < 0.0f || beta > 1.0f) {
                return false;
        }

        rec->t   = t;
        rec->p   = hit_point;
        rec->mat = quad->mat;
        hit_record_set_face_normal(rec, r, quad->normal);

        return true;
}

static void
quad_destroy(hittable_t* self)
{
        free(self);
}

static const hittable_vtable_t quad_vtable = {
        .hit     = quad_hit,
        .destroy = quad_destroy,
};

quad_t*
quad_create(vec3_t q, vec3_t u, vec3_t v, material_t mat)
{
        quad_t* quad;

        quad = malloc(sizeof(*quad));
        if (quad == NULL) {
                fprintf(stderr, "[ERROR] Memory Allocation failed: quad_t\n");
                return NULL;
        }

        if (quad_init(quad, q, u, v, mat) != MOCATRA_OK) {
                fprintf(stderr, "[ERROR] Quad creation failed (quad_init)\n");
                free(quad);
                return NULL;
        }

        return quad;
}

mocatra_error_t
quad_init(quad_t* quad, vec3_t q, vec3_t u, vec3_t v, material_t mat)
{
        vec3_t n;

        if (quad == NULL) {
                return MOCATRA_INVARG;
        }

        n = vec3_cross(u, v);
        if (vec3_len_squared(n) < DEGENERATE_EPSILON) {
                return MOCATRA_INVARG;
        }

        quad->base.vtable = &quad_vtable;
        quad->q           = q;
        quad->u           = u;
        quad->v           = v;
        quad->mat         = mat;
        quad->normal      = vec3_unit(n);
        quad->d           = vec3_dot(quad->normal, q);
        quad->w           = vec3_scal(n, 1.0f / vec3_len_squared(n));

        return MOCATRA_OK;
}

mocatra_error_t
hittable_list_add_quad(hittable_list_t* list, vec3_t q, vec3_t u, vec3_t v,
                       material_t mat)
{
        mocatra_error_t err;
        quad_t*         quad;

        quad = quad_create(q, u, v, mat);
        if (quad == NULL) {
                return MOCATRA_ERROR;
        }

        err = hittable_list_add(list, (hittable_t*)quad);
        if (err != MOCATRA_OK) {
                free(quad);
        }

        return err;
}
