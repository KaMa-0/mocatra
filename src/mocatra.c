#include "mocatra.h"

#include "config.h"

#include "core/ray.h"
#include "core/vec3.h"

#include "scene/camera.h"
#include "scene/world.h"

#include "render/renderer.h"

#include "io/image.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int
mocatra_main(void)
{
        image_t* img; /* image containing image buffer */

        hittable_list_t* world; /* world containing list of objects */

        render_settings_t settings;

        camera_t cam;

        char* img_path; /* path to destination for final image render */

        float aspect_ratio; /* aspect ratio of the final image render */

        int img_width, img_height; /* image width and height (min. 1) */

        /* ============= */
        /* configuration */

        /* values taken over from the central config file */
#ifndef MOCATRA_REGRESSION
        img_path       = output_image_path;
        aspect_ratio   = image_aspect_ratio;
        img_width      = image_width;
        //samples_per_px = samples_per_pixel;
        //max_depth      = maximum_ray_depth;
#else
        img_path       = REGRESSION_PATH;
        aspect_ratio   = REGRESSION_RATIO;
        img_width      = REGRESSION_WIDTH;
        settings = (render_settings_t) {
                samples_per_px = REGRESSION_SPP,
                max_depth      = REGRESSION_DEPTH,
        }
#endif

        /* ============= */

        /* Random generator seeding */

        const char* s = getenv("MOCATRA_SEED");
        srand(s ? (unsigned int)strtoul(s, NULL, 10)
                : (unsigned int)time(NULL));

        /* ------------------------ */

        /* Image */

        img_height = (int)(img_width / aspect_ratio);
        img_height = (img_height < 1) ? 1 : img_height;

        img = image_create(img_width, img_height);
        if (img == NULL) {
                printf("[ERROR] Failed to create image.\n");
                return MOCATRA_ERROR;
        }

        /* ----- */

        /* Camera */

        cam = camera_init(img_width, img_height);

        /* ------ */

        /* World */

        world = create_world(18);
        if (world == NULL) {
                printf("[ERROR] Failed to create world scene.\n");
                return MOCATRA_ERROR;
        }
        if (init_world_cornell_box(world) != MOCATRA_OK) {
                printf("[ERROR] Failed to initialize world scene.\n");
                return MOCATRA_ERROR;
        }

        /* ----- */

        /* Render */

        settings = (render_settings_t){
                .sample_per_px = 64,
                .max_depth     = 5,
        };
        render(img, &cam, world, &settings);

        /* ------ */

        printf("\n\nRender Complete!\nSaving image ... \n");

        if (image_write_ppm(img, img_path) != 0) {
                fprintf(stderr, "Failed to write image\n");
                image_destroy(img);
                return 1;
        }

        /* Cleanup */

        image_destroy(img);

        destroy_world(world);

        /* ------- */

        return 0;
}
