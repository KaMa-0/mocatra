/**
 * \file        path_tracer.c
 */

#include "integrator/integrator.h"

#include "geometry/hittable.h"
#include "material/material.h"
#include "mocatra.h"

#define RAY_T_MIN 0.001f

static vec3_t
path_tracer_integrate(ray_t ray, int depth, const hittable_t* world,
                      const sampler_t* sampler)
{
        hit_record_t rec;
        ray_t        scattered;
        vec3_t       attenuation, scatter_dir, incoming;

        if (depth <= 0) {
                return v3(0.0f, 0.0f, 0.0f);
        }

        if (!world->vtable->hit(world, ray, RAY_T_MIN, INF, &rec)) {
                return v3(0.0f, 0.0f, 0.0f);
        }

        if (!material_scatter(rec.mat, ray.dir, rec.normal, &scatter_dir,
                              &attenuation, sampler)) {
                return rec.mat.emission;
        }

        scattered = (ray_t){.orig = rec.p, .dir = scatter_dir};
        incoming  = path_tracer_integrate(scattered, depth - 1, world, sampler);

        return vec3_add(rec.mat.emission,
                        vec3_cmpnt_mult(attenuation, incoming));
}

const integrator_t*
integrator_path_tracing(void)
{
        static const integrator_t path_tracing = {
                .integrate = path_tracer_integrate,
        };

        return &path_tracing;
}