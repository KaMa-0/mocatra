#include <math.h>


#include "mocatra.h"

#include "ray.h"
#include "hittable.h"


const vec3_t 
ray_at(const ray_t r, float t)
{
        return vec3_add(r.orig, vec3_scal(r.dir, t));
}

vec3_t
ray_color(ray_t r, int depth, const hittable_t* world)
{
        hit_record_t     rec;
        ray_t            scattered_ray;
        vec3_t           bounce_direction;
        vec3_t           emitted;
        vec3_t           incoming_col;
        vec3_t           attenuation;
        float            scattering_pdf;
        float            pdf;

        if (depth <= 0)
                return (vec3_t){
                        .x = 0.0f,
                        .y = 0.0f,
                        .z = 0.0f,
                };

        if (!world->vtable->hit(world, r, 0.001f, INF, &rec))
                /* blackness */
                return (vec3_t){
                        .x = 0.0f,
                        .y = 0.0f,
                        .z = 0.0f,
                };

        /* light source */
        if (rec.mat.type == MAT_DIFFUSE_LIGHT)
                return rec.mat.emission;

        emitted = material_emitted(rec.mat);

        if (rec.mat.type == MAT_METAL) {
                /* metal material */
                bounce_direction = vec3_reflect(vec3_unit(r.dir),
                                                rec.normal);
                if (vec3_dot(bounce_direction, rec.normal) <= 0.0f)
                        return emitted;

                scattered_ray = (ray_t){
                        .orig = rec.p,
                        .dir  = bounce_direction,
                };
                incoming_col = ray_color(scattered_ray, depth - 1, world);

                return vec3_add(emitted, vec3_cmpnt_mult(rec.mat.albedo,
                                                         incoming_col));

        } else {
                /* lambertian diffuse material */
                bounce_direction = vec3_add(rec.normal, 
                                            random_unit_vector());
                if (vec3_len_squared(bounce_direction) < 1e-6f) {
                        bounce_direction = rec.normal;
                }

                scattered_ray = (ray_t){
                        .orig = rec.p,
                        .dir  = bounce_direction,
                };

                scattering_pdf = material_scattering_pdf(rec.mat,
                                                         rec.normal,
                                                         scattered_ray.dir);
                pdf            = scattering_pdf;

                if (pdf <= 0.0f)
                        return emitted;

                incoming_col = ray_color(scattered_ray, depth - 1, world);

                attenuation = vec3_scal(rec.mat.albedo, scattering_pdf / pdf);

                return vec3_add(emitted, vec3_cmpnt_mult(attenuation,
                                                         incoming_col));
        }
}

float
hit_sphere(const vec3_t center, float radius, const ray_t r)
{
        vec3_t oc;
        float a, h, c, discriminant;

        oc = vec3_sub(center, r.orig);

        a = vec3_len_squared(r.dir);
        h = vec3_dot(r.dir, oc);
        c = vec3_len_squared(oc) - radius * radius;

        discriminant = h * h - a * c;

        if (discriminant < 0) {
                return -1.0;
        } else {
                return ((h - sqrt(discriminant)) / a);
        }
}

vec3_t 
material_emitted(material_t mat)
{
        if (mat.type == MAT_DIFFUSE_LIGHT) {
                return mat.emission;
        }
        return (vec3_t){
                .x = 0.0f,
                .y = 0.0f,
                .z = 0.0f,
        };
}

float
material_scattering_pdf(material_t mat, vec3_t normal, vec3_t scattered_dir)
{
        float cos_theta;

        if (mat.type == MAT_LAMBERTIAN) {
                cos_theta = vec3_dot(vec3_unit(scattered_dir), normal);
                return (cos_theta < 0.0f) ? 0.0f : cos_theta / (float)PI;
        }

        return 1.0f;
}
