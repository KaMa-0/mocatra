/**
 * @file camera.h
 * @brief Camera and viewport creation and configuration utility.
 */

#ifndef _CAMERA_H
#define _CAMERA_H


#include "vec3.h"


typedef struct camera {
        vec3_t center;
        vec3_t px_origin;
        vec3_t px_delta_u;
        vec3_t px_delta_v;
} camera_t;


camera_t camera_create(int img_width, int img_height);


#endif /* _CAMERA_H */

