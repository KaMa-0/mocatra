/**
 * \file        ray.c
 */

#include "geometry/hittable.h"
#include "material/material.h"
#include "mocatra.h"

#define RAY_T_MIN 0.001f

vec3_t
ray_at(const ray_t r, float t)
{
        return vec3_add(r.orig, vec3_scal(r.dir, t));
}

vec3_t
ray_color(ray_t r, int depth, const hittable_t* world)
{
        hit_record_t rec;
        ray_t        scattered;
        vec3_t       attenuation, scatter_dir, incoming;

        if (depth <= 0) {
                return v3(0.0f, 0.0f, 0.0f);
        }

        if (!world->vtable->hit(world, r, RAY_T_MIN, INF, &rec)) {
                return v3(0.0f, 0.0f, 0.0f);
        }

        if (!material_scatter(rec.mat, r.dir, rec.normal, &scatter_dir,
                              &attenuation)) {
                return rec.mat.emission;
        }

        scattered = (ray_t){.orig = rec.p, .dir = scatter_dir};
        incoming  = ray_color(scattered, depth - 1, world);

        return vec3_add(rec.mat.emission,
                        vec3_cmpnt_mult(attenuation, incoming));
}
