/**
 * \file        renderer.c
 */

#include "render/renderer.h"

#include <stdbool.h>

#define PROGRESS_BAR_WIDTH 50

static void
render_print_progress(uint32_t rows_done, uint32_t rows_total)
{
        float frac;
        int   filled;

        frac   = (float)rows_done / (float)rows_total;
        filled = (int)(PROGRESS_BAR_WIDTH * frac);

        printf("\rRendering: [");
        for (int i = 0; i < PROGRESS_BAR_WIDTH; i++) {
                if (i < filled) {
                        printf("=");
                } else if (i == filled) {
                        printf(">");
                } else {
                        printf(" ");
                }
        }
        printf("] %3d%%", (int)(frac * 100));
        fflush(stdout);
}

static bool
render_settings_valid(const render_settings_t* settings)
{
        return (settings->sample_per_px > 0 && settings->max_depth > 0);
}

mocatra_error_t
render(image_t* img, const camera_t* cam, const hittable_list_t* world,
       const render_settings_t* settings)
{
        float pixel_samples_scale;

        if (img == NULL || cam == NULL || world == NULL) {
                return MOCATRA_INVARG;
        }

        if (!render_settings_valid(settings)) {
                return MOCATRA_INVSETTINGS;
        }

        pixel_samples_scale = 1.0f / (float)settings->sample_per_px;

        for (uint32_t y = 0; y < img->height; y++) {
                for (uint32_t x = 0; x < img->width; x++) {
                        vec3_t  accumulated_color;
                        pixel_t px;

                        accumulated_color = (vec3_t){
                                .x = 0.0f,
                                .y = 0.0f,
                                .z = 0.0f,
                        };

                        for (int s = 0; s < settings->sample_per_px; s++) {
                                vec3_t offset;
                                vec3_t sample_col;
                                vec3_t sample_pixel_loc;
                                ray_t  ray;

                                offset           = sample_square();
                                sample_pixel_loc = vec3_add(
                                        cam->px_origin,
                                        vec3_add(vec3_scal(cam->px_delta_u,
                                                           (float)x + offset.x),
                                                 vec3_scal(
                                                         cam->px_delta_v,
                                                         (float)y + offset.y)));
                                ray = (ray_t)
                                {
                                        .orig = cam->center,
                                        .dir  = vec3_sub(sample_pixel_loc,
                                                         cam->center),
                                };

                                sample_col = ray_color(ray, settings->max_depth,
                                                       (hittable_t*)world);
                                accumulated_color = vec3_add(accumulated_color,
                                                             sample_col);
                        }

                        accumulated_color = vec3_scal(accumulated_color,
                                                      pixel_samples_scale);

                        px = (pixel_t){
                                .r = linear_to_gamma(accumulated_color.x),
                                .g = linear_to_gamma(accumulated_color.y),
                                .b = linear_to_gamma(accumulated_color.z),
                        };

                        if (image_px_set(img, x, y, px) != MOCATRA_OK) {
                                printf("[ERROR] Failed setting pixel value.\n");
                                return MOCATRA_ERROR;
                        }
                }

                /* progress indicator */
                render_print_progress(y + 1, img->height);
        }

        return MOCATRA_OK;
}
