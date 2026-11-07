/**
 * \file        camera.h
 * \brief       Creation and configuration of 3D camera.
 */

#ifndef _CAMERA_H
#define _CAMERA_H

#include "core/ray.h"
#include "core/vec3.h"

#include <stdint.h>

typedef struct camera_config {
        vec3_t lookfrom;
        vec3_t lookat;
        vec3_t vup;
        float  vfov_degrees;
} camera_config_t;

typedef struct camera {
        vec3_t center;
        vec3_t px_origin;  /* center of pixel (0,0) */
        vec3_t px_delta_u; /* step to the next pixel */
        vec3_t px_delta_v; /* step to the next row */
} camera_t;

camera_config_t camera_config_default(void);
camera_t camera_init(const camera_config_t* cfg, int img_width, int img_height);

ray_t camera_get_ray(const camera_t* cam, uint32_t x, uint32_t y);

#endif /* _CAMERA_H */
