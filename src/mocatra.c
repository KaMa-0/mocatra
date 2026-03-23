#include <stdio.h>

#include "image.h"


int
main(void) 
{
        image_t* img;
        printf("MOCATRA - Monte Carlo Tracer!\n");
        
        img = image_create(1920, 1080);
        if (img == NULL) {
                printf("[ERROR] Failed to create image.\n");
        }

        for (uint32_t y = 0; y < img->height; y++) {
                for (uint32_t x = 0; x < img->width; x++) {
                        pixel_t px;
                        px.r = (uint8_t)(x * 255 / (img->width  - 1));
                        px.g = (uint8_t)(y * 255 / (img->height - 1));
                        px.b = 128;
                        image_px_set(img, x, y, px);
                }
        }

        if (image_write_ppm(img, "test/gradient.ppm") != 0) {
                fprintf(stderr, "Failed to write image\n");
                image_free(img);
                return 1;
        }

        image_free(img);

        return 0;
}
