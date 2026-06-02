#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mocatra.h"

#include "viewport.h"
#include "image.h"
#include "vec3.h"
#include "ray.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"


int
main(void) 
{
        image_t* img; /* image containing image buffer */
        pixel_t  px;  /* pixel containing r,g,b values */

        hittable_list_t* world;          /* world containing list of objects */
        sphere_t*        ground;         /* big sphere serving as ground     */
        sphere_t*        sphere_main;    /* main sphere of scene             */
        vec3_t           sphere_center;  /* center vector for scene spheres  */

        ray_t    ray;           /* ray                          */
        vec3_t   ray_direction; /* direction of ray in 3d space */

        vec3_t   cam_center; /* 3d position of camera                       */
        vec3_t   vp_origin;  /* 3d position of viewport origin              */
        vec3_t   vp_u, vp_v; /* viewport vec from left to right edge (vp_u) */
                             /* viewport vec from top to bottom edge (vp_v) */

        vec3_t   px_origin; /* 3d position of pixel grid origin           */
        vec3_t   px_center; /* 3d position of the center-point of a pixel */
        vec3_t   px_delta_u, px_delta_v; /* pixel delta left to right (u) */
                                         /* pixel delta top to bottom (v) */

        vec3_t   accumulated_color; /* contains sub pixel color accumulation */
        vec3_t   offset;            /* random offset within sample square    */
        vec3_t   sample_pixel_loc;  /* sample coordinate on viewport grid    */
        vec3_t   sample_col;        /* color of single sample                */

        char*    img_path; /* path to destination for final image render */

        float    focal_length; /* camera focal length */
        float    aspect_ratio; /* aspect ratio of the final image render */

        float    vp_width,  vp_height;  /* viewport width and height       */
        int      img_width, img_height; /* image width and height (min. 1) */

        float    pixel_samples_scale;   /* scale factor maps sum to 0.0-1.0 */
        int      samples_per_px;        /* number of samples per pixel      */


        printf("MOCATRA - Monte Carlo Tracer!\n");

        /* ============= */
        /* configuration */

        img_path        = "test/circle_with_ground_with_anti_aliasing.ppm";
        aspect_ratio    = 16.0 / 9.0;
        img_width       = 1920;
        samples_per_px  = 1;

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

        focal_length = 1.0;
        cam_center   = (vec3_t){ 
                .x = 0, 
                .y = 0, 
                .z = 0,
        };

        vp_height    = 2.0;
        vp_width     = vp_height * ((double)(img_width) / img_height);

        vp_u = (vec3_t){ 
                .x = vp_width, 
                .y = 0, 
                .z = 0 
        };
        vp_v = (vec3_t){ 
                .x = 0,
                .y = -vp_height,
                .z = 0
        };

        px_delta_u = vec3_scal(vp_u, (1.0f / (float)img_width));
        px_delta_v = vec3_scal(vp_v, (1.0f / (float)img_height));

        vp_origin = get_viewport_origin(cam_center, vp_u, vp_v, focal_length);

        px_origin = get_pixel_origin(vp_origin, px_delta_u, px_delta_v);

        /* ------ */


        /* World */

        world           = hittable_list_create();
        ground          = sphere_create();
        sphere_main     = sphere_create();

        hittable_list_init(world, 3);
        sphere_center = (vec3_t){
                .x = 0,
                .y = -100.5,
                .z = -1,
        };
        sphere_init(ground, sphere_center, 100);
        sphere_center = (vec3_t){
                .x = 0,
                .y = 0,
                .z = -1,
        };
        sphere_init(sphere_main, sphere_center, 0.5);
        hittable_list_add(world, (hittable_t*)ground);
        hittable_list_add(world, (hittable_t*)sphere_main);

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
                                sample_pixel_loc = vec3_add(px_origin,
                                        vec3_add(vec3_scal(px_delta_u,
                                                           (float)x + offset.x),
                                                 vec3_scal(px_delta_v,
                                                           (float)y + offset.y))
                                );
                                ray_direction = vec3_sub(sample_pixel_loc,
                                                         cam_center);
                                ray = (ray_t){
                                        .orig = cam_center,
                                        .dir = ray_direction,
                                };

                                sample_col = ray_color(ray, (hittable_t*)world);
                                accumulated_color = vec3_add(accumulated_color,
                                                             sample_col);
                        }

                        accumulated_color = vec3_scal(accumulated_color,
                                                      pixel_samples_scale);

                        px = (pixel_t){
                                .r = accumulated_color.x,
                                .g = accumulated_color.y,
                                .b = accumulated_color.z,
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

        hittable_list_cleanup(world);
        free(world);

        /* ------- */

        return 0;
}
