/**
 * @file camera.h
 * @brief Camera configuration and viewport ray calculation utilites.
 */

#ifndef _CAMERA_H
#define _CAMERA_H


#include "mocatra.h"

typedef struct camera {
        vec3_t center;
        vec3_t px_origin;
        vec3_t px_delta_u;
        vec3_t px_delta_v;
} camera_t;


camera_t camera_init(int img_width, int img_height);


#endif /* _CAMERA_H */
