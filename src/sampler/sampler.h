/**
 * \file        sampler.h
 * \brief       Random sample generation strategies.
 */

#ifndef _SAMPLER_H
#define _SAMPLER_H

#include "core/vec3.h"

typedef struct sampler {
        void* state;
        vec3_t (*sample_pixel_offset)(void* state);
        vec3_t (*sample_unit_vector)(void* state);
} sampler_t;

const sampler_t* sampler_default(void);
vec3_t           sampler_sample_pixel_offset(const sampler_t* sampler);
vec3_t           sampler_sample_unit_vector(const sampler_t* sampler);

#endif /* _SAMPLER_H */