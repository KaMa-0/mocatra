#include <math.h>


#include "mocatra.h"

#include "ray.h"
#include "hittable.h"


const vec3_t 
ray_at(const ray_t r, float t)
{
        return vec3_add(r.orig, vec3_scal(r.dir, t));
}

pixel_t 
ray_color(ray_t r, const hittable_t* world)
{
        hit_record_t    rec;
        vec3_t          unit_direction;
        float           gradient;

        if (world->vtable->hit(world, r, 0.001f, INF, &rec)) {
                return (pixel_t){
                        .r = 0.5 * (rec.normal.x + 1),
                        .g = 0.5 * (rec.normal.y + 1),
                        .b = 0.5 * (rec.normal.z + 1),
                };
        }
        

        unit_direction = vec3_unit(r.dir);
        gradient = 0.5 * (unit_direction.y + 1.0);

        return (pixel_t){ 
                .r = (1.0 - gradient) + gradient * 0.5, 
                .g = (1.0 - gradient) + gradient * 0.7, 
                .b = (1.0 - gradient) + gradient * 1.0,
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
