/**
 *
 *  \file   image.h
 *  \brief  Utility for writing PPM Images
 *
 */

/*
 *  Author:  KaMa (https://github.com/KaMa-0/
 *                 https://gitlab.com/KaMa-0/)
 *
 */


#ifndef _IMAGE_H
#define _IMAGE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct pixel {
        uint8_t  r, g, b;
} pixel_t;


typedef struct image {
        pixel_t* buffer;
        uint32_t width, height;
} image_t;


image_t* image_create(uint32_t width, uint32_t height);
void     image_free(image_t* img);

pixel_t* image_px_get(image_t* img, uint32_t x, uint32_t y);
int      image_px_set(image_t* img, 
                      uint32_t x, uint32_t y, 
                      pixel_t px);

size_t image_size(const image_t* img);
size_t image_bufsize(const image_t* img);

int image_write_ppm(image_t* img, const char* path);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _IMAGE_H */
