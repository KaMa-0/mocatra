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
        hit_record_t    rec;
        ray_t           scattered_ray;
        vec3_t          direction;
        vec3_t          unit_direction;
        vec3_t          incoming_col;
        float           gradient;

        if (depth <= 0)
                return (vec3_t){
                        .x = 0.0f,
                        .y = 0.0f,
                        .z = 0.0f,
                };

        if (world->vtable->hit(world, r, 0.001f, INF, &rec)) {
                direction = vec3_add(rec.normal, random_unit_vector());
                scattered_ray = (ray_t){
                        .orig = rec.p,
                        .dir  = direction,
                };
                incoming_col = ray_color(scattered_ray, depth - 1, world);
                return vec3_scal(incoming_col, 0.5f);
        }
        

        unit_direction = vec3_unit(r.dir);
        gradient = 0.5f * (unit_direction.y + 1.0f);

        return (vec3_t){ 
                .x = (1.0f - gradient) + gradient * 0.5f, 
                .y = (1.0f - gradient) + gradient * 0.7f, 
                .z = (1.0f - gradient) + gradient * 1.0f,
        };
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
