/**
 *
 *  \file   viewport.h
 *  \brief  Viewport for 3d space to pixel mapping.
 *
 */

/*
 *  Author:  KaMa (https://github.com/KaMa-0/
 *                 https://gitlab.com/KaMa-0/)
 *
 */


#ifndef _VIEWPORT_H
#define _VIEWPORT_H

#include <stdint.h>

#include "vec3.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


vec3_t get_viewport_origin(vec3_t cam, vec3_t u, vec3_t v, float focal_len);

vec3_t get_pixel_origin(vec3_t vp, vec3_t px_u, vec3_t px_v);
vec3_t get_pixel_center(vec3_t px_origin, 
                        vec3_t px_u, vec3_t px_v, 
                        int x, int y);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _VIEWPORT_H */
