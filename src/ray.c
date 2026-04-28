#include <math.h>


#include "ray.h"


const vec3_t 
at(ray_t* r, float t)
{
        return vec3_add(r->orig, vec3_scal(r->dir, t));
}

pixel_t 
ray_color(const ray_t r)
{
        vec3_t unit_direction;
        vec3_t sphere_center;
        float gradient;

        sphere_center = (vec3_t){ 
                .x =  0, 
                .y =  0, 
                .z = -1,
        };
        if (hit_sphere(sphere_center, 0.5, r)) {
                return (pixel_t){
                        .r = 1,
                        .g = 0,
                        .b = 0,
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

uint8_t 
hit_sphere(const vec3_t center, float radius, const ray_t r)
{
        vec3_t oc;
        float a, b, c, discriminant;

        oc = vec3_sub(center, r.orig);

        a = vec3_dot(r.dir, r.dir);
        b = -2.0 * vec3_dot(r.dir, oc);
        c = vec3_dot(oc, oc) - (radius * radius);

        discriminant = b * b - 4 * a * c;

        return (discriminant >= 0);
}
