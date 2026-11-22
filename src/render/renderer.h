/**
 * \file        renderer.h
 * \brief       Rendering capabilities.
 */

#ifndef _RENDERER_H
#define _RENDERER_H

#include "integrator/integrator.h"
#include "io/image.h"
#include "mocatra.h"
#include "sampler/sampler.h"
#include "scene/camera.h"
#include "scene/world.h"

typedef struct render_settings {
        int                 sample_per_px;
        int                 max_depth;
        const sampler_t*    sampler;
        const integrator_t* integrator;
} render_settings_t;

mocatra_error_t render(image_t* img, const camera_t* cam,
                       const hittable_list_t*   world,
                       const render_settings_t* settings);

#endif /* _RENDERER_H */
