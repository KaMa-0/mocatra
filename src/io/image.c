/**
 * \file        image.c
 */

#include "image.h"

#include <stdio.h>
#include <stdlib.h>

static inline uint8_t
image_is_valid(const image_t* img)
{
        return img != NULL && img->buffer != NULL && img->height != 0
               && img->width != 0;
}

size_t
image_size(const image_t* img)
{
        if (!image_is_valid(img)) {
                return 0;
        }

        return img->width * img->height;
}

size_t
image_bufsize(const image_t* img)
{
        if (!image_is_valid(img)) {
                return 0;
        }

        return image_size(img) * sizeof(pixel_t);
}

image_t*
image_create(uint32_t width, uint32_t height)
{
        image_t* img;

        if (width == 0 || height == 0) {
                return NULL;
        }

        img = malloc(sizeof(image_t));
        if (img == NULL) {
                return NULL;
        }

        img->width  = width;
        img->height = height;

        img->buffer = calloc(width * height, sizeof(pixel_t));
        if (img->buffer == NULL) {
                return NULL;
        }

        return img;
}

pixel_t*
image_px_get(image_t* img, uint32_t x, uint32_t y)
{
        if (!image_is_valid(img)) {
                return NULL;
        }

        if (x >= img->width || y >= img->height) {
                return NULL;
        }

        return &img->buffer[y * img->width + x];
}

mocatra_error_t
image_px_set(image_t* img, uint32_t x, uint32_t y, pixel_t px)
{
        if (!image_is_valid(img)) {
                return MOCATRA_INVARG;
        }

        if (x >= img->width || y >= img->height) {
                return MOCATRA_INVARG;
        }

        img->buffer[y * img->width + x] = px;
        return MOCATRA_OK;
}

mocatra_error_t
image_write_ppm(image_t* img, const char* path)
{
        FILE* fp;

        if (!image_is_valid(img)) {
                return MOCATRA_INVARG;
        }

        fp = fopen(path, "wb");

        if (fp == NULL) {
                return MOCATRA_NOMEM;
        }

        /* Write PPM (P6)-Header */
        fprintf(fp, "P6\n%u %u\n255\n", img->width, img->height);

        /* Write Pixel Data (binary) */
        for (uint32_t i = 0; i < image_size(img); i++) {
                pixel_t p = img->buffer[i];

                unsigned char r = (unsigned char)(255.999f * p.r);
                unsigned char g = (unsigned char)(255.999f * p.g);
                unsigned char b = (unsigned char)(255.999f * p.b);

                fputc(r, fp);
                fputc(g, fp);
                fputc(b, fp);
        }

        fclose(fp);

        return MOCATRA_OK;
}

void
image_destroy(image_t* img)
{
        if (img == NULL) {
                return;
        }

        if (img->buffer != NULL) {
                free(img->buffer);
        }

        free(img);
}
