/**
 * \file        camera.c
 */

#include "scene/camera.h"

#include "mocatra.h"

camera_config_t
camera_config_default(void)
{
        return (camera_config_t){
                .lookfrom     = v3(278.0f, 278.0f, -800.0f),
                .lookat       = v3(278.0f, 278.0f, 0.0f),
                .vup          = v3(0.0f, 1.0f, 0.0f),
                .vfov_degrees = 40.0f,
        };
}

camera_t
camera_init(const camera_config_t* cfg, int img_width, int img_height)
{
        camera_t cam;
        vec3_t   w, u, v, vp_u, vp_v, vp_upper_left, half_pixel;
        float    h, focal_length, vp_width, vp_height;

        h            = tanf(cfg->vfov_degrees * (float)PI / 180.0f / 2.0f);
        focal_length = vec3_length(vec3_sub(cfg->lookfrom, cfg->lookat));
        vp_height    = 2.0f * h * focal_length;
        vp_width     = vp_height * ((float)(img_width) / (float)img_height);

        w = vec3_unit(vec3_sub(cfg->lookfrom, cfg->lookat));
        u = vec3_unit(vec3_cross(cfg->vup, w));
        v = vec3_cross(w, u);

        vp_u = vec3_scal(u, vp_width);
        vp_v = vec3_scal(v, -vp_height);

        cam.center     = cfg->lookfrom;
        cam.px_delta_u = vec3_scal(vp_u, (1.0f / (float)img_width));
        cam.px_delta_v = vec3_scal(vp_v, (1.0f / (float)img_height));

        vp_upper_left = vec3_sub(
                vec3_sub(vec3_sub(cam.center, vec3_scal(w, focal_length)),
                         vec3_scal(vp_u, 0.5f)),
                vec3_scal(vp_v, 0.5f));

        half_pixel = vec3_scal(vec3_add(cam.px_delta_u, cam.px_delta_v), 0.5f);
        cam.px_origin = vec3_add(vp_upper_left, half_pixel);

        return cam;
}

ray_t
camera_get_ray(const camera_t* cam, uint32_t x, uint32_t y)
{
        vec3_t offset, sample;

        offset = sample_square();
        sample = vec3_add(
                cam->px_origin,
                vec3_add(vec3_scal(cam->px_delta_u, (float)x + offset.x),
                         vec3_scal(cam->px_delta_v, (float)y + offset.y)));

        return (ray_t){
                .orig = cam->center,
                .dir  = vec3_sub(sample, cam->center),
        };
}
