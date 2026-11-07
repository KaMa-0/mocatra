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
        mocatra_conf_t    conf;

        camera_config_t cam_cfg;
        camera_t        cam;

        /* ============= */
        /* configuration */

        /* values taken over from the central config file */
        conf = load_config();
        config_validate(&conf);

        /* ============= */

        /* Random generator seeding */

        srand(conf.seed == 0u ? (unsigned int)time(NULL) : conf.seed);

        /* ------------------------ */

        /* Image */

        img = image_create(conf.img_width, conf.img_height);
        if (img == NULL) {
                printf("[ERROR] Failed to create image.\n");
                return MOCATRA_ERROR;
        }

        /* ----- */

        /* Camera */

        cam_cfg = camera_config_default();
        cam     = camera_init(&cam_cfg, conf.img_width, conf.img_height);

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
                .sample_per_px = conf.spp,
                .max_depth     = conf.ray_depth,
        };
        render(img, &cam, world, &settings);

        /* ------ */

        printf("\n\nRender Complete!\nSaving image ... \n");

        if (image_write_ppm(img, conf.img_path) != 0) {
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
