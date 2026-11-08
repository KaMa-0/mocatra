/**
 * \file        ray.c
 */

#include "core/ray.h"

#include "geometry/hittable.h"
#include "mocatra.h"

#include <math.h>

vec3_t
ray_at(const ray_t r, float t)
{
        return vec3_add(r.orig, vec3_scal(r.dir, t));
}

vec3_t
ray_color(ray_t r, int depth, const hittable_t* world)
{
        hit_record_t rec;
        ray_t        scattered_ray;
        vec3_t       bounce_direction;
        vec3_t       emitted;
        vec3_t       incoming_col;
        vec3_t       attenuation;
        float        scattering_pdf;
        float        pdf;

        if (depth <= 0) {
                return (vec3_t){
                        .x = 0.0f,
                        .y = 0.0f,
                        .z = 0.0f,
                };
        }

        if (!world->vtable->hit(world, r, 0.001f, INF, &rec)) {
                /* blackness */
                return (vec3_t){
                        .x = 0.0f,
                        .y = 0.0f,
                        .z = 0.0f,
                };
        }

        /* light source */
        if (rec.mat.type == MAT_DIFFUSE_LIGHT) {
                return rec.mat.emission;
        }

        emitted = rec.mat.emission;

        if (rec.mat.type == MAT_METAL) {
                /* metal material */
                bounce_direction = vec3_reflect(vec3_unit(r.dir), rec.normal);
                if (vec3_dot(bounce_direction, rec.normal) <= 0.0f) {
                        return emitted;
                }

                scattered_ray = (ray_t){
                        .orig = rec.p,
                        .dir  = bounce_direction,
                };
                incoming_col = ray_color(scattered_ray, depth - 1, world);

                return vec3_add(emitted,
                                vec3_cmpnt_mult(rec.mat.albedo, incoming_col));

        } else {
                /* lambertian diffuse material */
                bounce_direction = vec3_add(rec.normal, random_unit_vector());
                if (vec3_len_squared(bounce_direction) < 1e-6f) {
                        bounce_direction = rec.normal;
                }

                scattered_ray = (ray_t){
                        .orig = rec.p,
                        .dir  = bounce_direction,
                };

                scattering_pdf = material_scattering_pdf(rec.mat, rec.normal,
                                                         scattered_ray.dir);
                pdf            = scattering_pdf;

                if (pdf <= 0.0f) {
                        return emitted;
                }

                incoming_col = ray_color(scattered_ray, depth - 1, world);

                attenuation = vec3_scal(rec.mat.albedo, scattering_pdf / pdf);

                return vec3_add(emitted,
                                vec3_cmpnt_mult(attenuation, incoming_col));
        }
}
