/**
 * \file        camera.c
 */

#include "camera.h"


#include "mocatra.h"


camera_t 
camera_init(int img_width, int img_height)
{
        camera_t cam;
        vec3_t   w, u, v, vp_u, vp_v, vp_upper_left;
        float    theta, h, focal_length, vp_width, vp_height;

        const vec3_t lookfrom = {278.0f, 278.0f, -800.0f};
        const vec3_t lookat   = {278.0f, 278.0f, 0.0f};
        const vec3_t vup      = {0.0f, 1.0f, 0.0f};

        const float vfov = 40.0f;

        theta = vfov * (float)PI / 180.0f;
        h     = tanf(theta / 2.0f);
        
        focal_length = vec3_length(vec3_sub(lookfrom, lookat)); 
        vp_height    = 2.0f * h * focal_length;
        vp_width     = vp_height * ((float)(img_width) / img_height);

        
        w = vec3_unit(vec3_sub(lookfrom, lookat)); 
        u = vec3_unit(vec3_cross(vup, w));         
        v = vec3_cross(w, u);                      

        cam.center = lookfrom;

        vp_u = vec3_scal(u, vp_width);
        vp_v = vec3_scal(v, -vp_height); 

        cam.px_delta_u = vec3_scal(vp_u, (1.0f / (float)img_width));
        cam.px_delta_v = vec3_scal(vp_v, (1.0f / (float)img_height));

        vp_upper_left = vec3_sub(
                vec3_sub(
                        vec3_sub(cam.center, vec3_scal(w, focal_length)), 
                        vec3_scal(vp_u, 0.5f)
                ), 
                vec3_scal(vp_v, 0.5f)
        );

        cam.px_origin = vec3_add(
                vp_upper_left, 
                vec3_scal(vec3_add(cam.px_delta_u, cam.px_delta_v), 0.5f)
        );

        return cam;
}
