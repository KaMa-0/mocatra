#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mocatra.h"
#include "config.h"

#include "image.h"
#include "vec3.h"
#include "ray.h"

#include "camera.h"
#include "world.h"


int
main(void) 
{
        image_t* img; /* image containing image buffer */
        pixel_t  px;  /* pixel containing r,g,b values */

        hittable_list_t* world;          /* world containing list of objects */

        material_t mat_ground;
        material_t mat_main;

        ray_t    ray;           /* ray                          */
        vec3_t   ray_direction; /* direction of ray in 3d space */

        camera_t cam;

        vec3_t   accumulated_color; /* contains sub pixel color accumulation */
        vec3_t   offset;            /* random offset within sample square    */
        vec3_t   sample_pixel_loc;  /* sample coordinate on viewport grid    */
        vec3_t   sample_col;        /* color of single sample                */

        char*    img_path; /* path to destination for final image render */

        float    aspect_ratio; /* aspect ratio of the final image render */

        float    vp_width,  vp_height;  /* viewport width and height       */
        int      img_width, img_height; /* image width and height (min. 1) */

        float    pixel_samples_scale;   /* scale factor maps sum to 0.0-1.0 */
        int      samples_per_px;        /* number of samples per pixel      */

        int      max_depth;


        printf("MOCATRA - Monte Carlo Tracer!\n");

        /* ============= */
        /* configuration */

        /* values taken over from the central config file */
        img_path        = output_image_path;
        aspect_ratio    = image_aspect_ratio;
        img_width       = image_width;
        samples_per_px  = samples_per_pixel;
        max_depth       = maximum_ray_depth;

        /* ============= */

        /* Random generator seeding */

        srand((unsigned int)time(NULL));

        /* ------------------------ */
        
        /* Image */

        img_height = (int)(img_width / aspect_ratio);
        img_height = (img_height < 1) ? 1 : img_height;

        img = image_create(img_width, img_height);
        if (img == NULL) {
                printf("[ERROR] Failed to create image.\n");
        }

        /* ----- */

        /* Camera */

        cam = camera_init(img_width, img_height);

        /* ------ */


        /* World */

        world = create_world(18);
        init_world_cornell_box(world);

        /* ----- */

        
        /* Render */

        pixel_samples_scale = 1.0f / (float)samples_per_px;

        for (uint32_t y = 0; y < img->height; y++) {
                for (uint32_t x = 0; x < img->width; x++) {
                        accumulated_color = (vec3_t){ 
                                .x = 0.0f,
                                .y = 0.0f,
                                .z = 0.0f,
                        };

                        for (int s = 0; s < samples_per_px; s++) {
                                offset = sample_square();
                                sample_pixel_loc = vec3_add(cam.px_origin,
                                        vec3_add(vec3_scal(cam.px_delta_u,
                                                           (float)x + offset.x),
                                                 vec3_scal(cam.px_delta_v,
                                                           (float)y + offset.y))
                                );
                                ray_direction = vec3_sub(sample_pixel_loc,
                                                         cam.center);
                                ray = (ray_t){
                                        .orig = cam.center,
                                        .dir = ray_direction,
                                };

                                sample_col = ray_color(ray, max_depth,
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

                        image_px_set(img, x, y, px);
                }

                /* progress indicator */
                printf("\rRendering: ["); // Added \r back to fix line spam
                for (int i = 0; i < 50; i++) {
                        if (i < (int)(50 * ((float)(y + 1) / img->height))) {
                                printf("=");
                        } else if (i == (int)(50 * ((float)(y + 1) / img->height))) {
                                printf(">");
                        } else {
                                printf(" ");
                        }
                }
                printf("] %3d%%", (int)(((float)(y + 1) / img->height) * 100));
                fflush(stdout);
        }

        printf("\n\nRender Complete!\nSaving image ... \n");


        if (image_write_ppm(img, img_path) != 0) {
                fprintf(stderr, "Failed to write image\n");
                image_free(img);
                return 1;
        }

        /* ------ */


        /* Cleanup */

        image_free(img);

        destroy_world(world);

        /* ------- */

        return 0;
}
