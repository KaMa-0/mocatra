/**
 *
 *  \file   image.h
 *  \brief  Utility for writing PPM Images.
 *
 */

#ifndef _IMAGE_H
#define _IMAGE_H

#include "mocatra_error.h"

#include <stddef.h>
#include <stdint.h>

typedef struct pixel {
        float r;
        float g;
        float b;
} pixel_t;

typedef struct image {
        pixel_t* buffer;
        uint32_t width;
        uint32_t height;
} image_t;

image_t* image_create(uint32_t width, uint32_t height);
void     image_destroy(image_t* img);

pixel_t*        image_px_get(image_t* img, uint32_t x, uint32_t y);
mocatra_error_t image_px_set(image_t* img, uint32_t x, uint32_t y, pixel_t px);

size_t image_size(const image_t* img);
size_t image_bufsize(const image_t* img);

mocatra_error_t image_write_ppm(image_t* img, const char* path);

#endif /* _IMAGE_H */
