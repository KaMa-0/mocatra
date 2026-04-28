#include <math.h>


#include "ray.h"


const vec3_t 
ray_at(const ray_t* r, float t)
{
        return vec3_add(r->orig, vec3_scal(r->dir, t));
}

pixel_t 
ray_color(ray_t r)
{
        vec3_t N;
        vec3_t unit_direction;
        vec3_t sphere_center;
        float  gradient;
        float  t;

        sphere_center = (vec3_t){ 
                .x =  0, 
                .y =  0, 
                .z = -1,
        };

        t = hit_sphere(sphere_center, 0.5, r);
        if (t > 0.0) {
                N = vec3_unit(vec3_sub(ray_at(&r, t), sphere_center));
                return (pixel_t){
                        .r = 0.5 * (N.x + 1),
                        .g = 0.5 * (N.y + 1),
                        .b = 0.5 * (N.z + 1),
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
