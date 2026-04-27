#include "viewport.h"

vec3_t 
get_viewport_origin(vec3_t cam, vec3_t u, vec3_t v, float focal_len)
{
        vec3_t focal_offset;
        vec3_t origin;

        focal_offset = (vec3_t){ 0, 0, focal_len };
    
        origin = vec3_sub(cam, focal_offset);
        origin = vec3_sub(origin, vec3_scal(u, 0.5f));
        origin = vec3_sub(origin, vec3_scal(v, 0.5f));
    
        return origin;
}


vec3_t 
get_pixel_origin(vec3_t vp, vec3_t px_u, vec3_t px_v)
{
        vec3_t origin;
        vec3_t px;

        px = vec3_add(px_u, px_v);
        px = vec3_scal(px, 0.5);

        origin = vec3_add(vp, px);

        return origin;
}

vec3_t 
get_pixel_center(vec3_t px_origin, vec3_t px_u, vec3_t px_v, int x, int y)
{
        vec3_t center;
        vec3_t full_px_u, full_px_v;


        full_px_u = vec3_scal(px_u, x);
        full_px_v = vec3_scal(px_v, y);

        center = vec3_add(full_px_u, full_px_v);
        center = vec3_add(px_origin, center);

        return center;
}

