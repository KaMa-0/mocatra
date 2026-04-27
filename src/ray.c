#include <math.h>


#include "ray.h"


const vec3_t 
at(ray_t* r, float t)
{
        return vec3_add(r->orig, vec3_scal(r->dir, t));
}

pixel_t 
ray_color(const ray_t t)
{
        vec3_t unit_direction;
        float gradient;

        unit_direction = vec3_unit(t.dir);
        gradient = 0.5 * (unit_direction.y + 1.0);

        return (pixel_t){ 
                .r = (1.0 - gradient) + gradient * 0.5, 
                .g = (1.0 - gradient) + gradient * 0.7, 
                .b = (1.0 - gradient) + gradient * 1.0,
        };
}

