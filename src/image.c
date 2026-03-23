#include <stdlib.h>
#include <stdio.h>

#include "image.h"


static inline uint8_t 
image_is_valid(const image_t* img)
{
        return  img != NULL && img->buffer != NULL &&
                img-> height != 0 && img->width != 0; 
}

size_t 
image_size(const image_t* img)
{
        if (!image_is_valid(img))
                return 0;

        return img->width * img->height;
}

size_t 
image_bufsize(const image_t* img)
{
        if (!image_is_valid(img))
                return 0;

        return image_size(img) * sizeof(pixel_t);
}

image_t* 
image_create(uint32_t width, uint32_t height)
{
        image_t* img;

        if (width == 0 || height == 0)
                return NULL;
        
        img = malloc(sizeof(image_t));
        if (img == NULL)
                return NULL;

        img->width  = width;
        img->height = height;

        img->buffer = calloc(width * height, sizeof(pixel_t));
        if (img->buffer == NULL)
                return NULL;

        return img;
}


pixel_t*
image_px_get(image_t* img, uint32_t x, uint32_t y)
{
        if (!image_is_valid(img))
                return NULL;
        
        if (x <= img->width || y <= img->height)
                return NULL;

        return &img->buffer[y * img->width + x];
}

int
image_px_set(image_t* img, uint32_t x, uint32_t y, pixel_t px)
{
        if (!image_is_valid(img))
                return -1;
        
        if (x > img->width || y > img->height)
                return -2;

        img->buffer[y * img->width + x] = px;
        return 0;
}

int 
image_write_ppm(image_t* img, const char* path)
{
        FILE *fp;

        if (!image_is_valid(img))
                return -1;

        fp = fopen(path, "wb");

        if (fp == NULL)
                return -2;

        /* Write PPM (P6)-Header */
        fprintf(fp, "P6\n%u %u\n255\n", img->width, img->height);

        /* Write Pixel Data (binary) */
        fwrite(img->buffer, sizeof(pixel_t), image_size(img), fp);

        fclose(fp);

        return 0;
}

void
image_free(image_t* img)
{
        if (img == NULL)
                return;

        if (img->buffer != NULL)
                free(img->buffer);

        free(img);
}
