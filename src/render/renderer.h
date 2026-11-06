/**
 * \file        renderer.h
 * \brief       Rendering capabilities.
 */

#ifndef _RENDERER_H
#define _RENDERER_H

#include "io/image.h"
#include "mocatra.h"
#include "scene/camera.h"
#include "scene/world.h"

typedef struct render_settings {
        int sample_per_px;
        int max_depth;
} render_settings_t;

render_settings_t render_settings_default(void);
mocatra_error_t   render(image_t* img, const camera_t* cam,
                         const hittable_list_t*   world,
                         const render_settings_t* settings);

#endif /* _RENDERER_H */
