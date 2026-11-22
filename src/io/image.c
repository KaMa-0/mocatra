/**
 * \file        image.c
 */

#include "io/image.h"

#include <stdbool.h>
#include <stdint.h>

static inline bool
image_is_valid(const image_t* img)
{
        return img != NULL && img->buffer != NULL && img->height != 0
               && img->width != 0;
}

static inline bool
image_in_bounds(const image_t* img, uint32_t x, uint32_t y)
{
        return x < img->width && y < img->height;
}

static inline size_t
image_size(const image_t* img)
{
        return image_is_valid(img) ? img->width * img->height : 0;
}

static inline size_t
image_bufsize(const image_t* img)
{
        return image_size(img) * sizeof(pixel_t);
}

static inline uint8_t
channel_to_byte(float c)
{
        if (c < 0.0f) {
                c = 0.0f;
        } else if (c > 1.0f) {
                c = 1.0f;
        }
        return (uint8_t)(255.999f * c);
}

image_t*
image_create(uint32_t width, uint32_t height)
{
        image_t* img;

        if (width == 0 || height == 0) {
                return NULL;
        }

        img = malloc(sizeof(*img));
        if (img == NULL) {
                return NULL;
        }

        img->width  = width;
        img->height = height;
        img->buffer = calloc((size_t)img->width * img->height, sizeof(pixel_t));

        if (img->buffer == NULL) {
                free(img);
                return NULL;
        }

        return img;
}

void
image_destroy(image_t* img)
{
        if (img == NULL) {
                return;
        }

        free(img->buffer);
        free(img);
}

pixel_t*
image_px_get(image_t* img, uint32_t x, uint32_t y)
{
        if (!image_is_valid(img) || !image_in_bounds(img, x, y)) {
                return NULL;
        }

        return &img->buffer[(size_t)y * img->width + x];
}

mocatra_error_t
image_px_set(image_t* img, uint32_t x, uint32_t y, pixel_t px)
{
        pixel_t* dst;

        dst = image_px_get(img, x, y);
        if (dst == NULL) {
                return MOCATRA_INVARG;
        }

        *dst = px;
        return MOCATRA_OK;
}

mocatra_error_t
image_write_ppm(const image_t* img, const char* path)
{
        FILE*           fp;
        uint8_t*        bytes;
        size_t          count, nbytes;
        mocatra_error_t err;

        err = MOCATRA_OK;

        if (!image_is_valid(img) || path == NULL) {
                return MOCATRA_INVARG;
        }

        count  = image_size(img);
        nbytes = count * 3;

        bytes = malloc(nbytes);
        if (bytes == NULL) {
                return MOCATRA_NOMEM;
        }

        for (size_t i = 0; i < count; i++) {
                uint8_t* out;
                out    = &bytes[3 * i];
                out[0] = channel_to_byte(img->buffer[i].r);
                out[1] = channel_to_byte(img->buffer[i].g);
                out[2] = channel_to_byte(img->buffer[i].b);
        }

        fp = fopen(path, "wb");
        if (fp == NULL) {
                free(bytes);
                return MOCATRA_FILE_ERROR;
        }

        if (fprintf(fp, "P6\n%u %u\n255\n", img->width, img->height) < 0
            || fwrite(bytes, 1, nbytes, fp) != nbytes) {
                err = MOCATRA_FILE_ERROR;
        }

        if (fclose(fp) != 0) {
                err = MOCATRA_FILE_ERROR;
        }

        free(bytes);
        return err;
}
