/**
 * \file        integrator.h
 * \brief       Ray integration strategies.
 */

#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H

#include "core/ray.h"
#include "core/vec3.h"
#include "sampler/sampler.h"

typedef struct hittable hittable_t;

typedef struct integrator {
        vec3_t (*integrate)(ray_t ray, int depth, const hittable_t* world,
                            const sampler_t* sampler);
} integrator_t;

const integrator_t* integrator_path_tracing(void);

#endif /* _INTEGRATOR_H */