/**
 * \file        sampler.h
 * \brief       Random sample generation strategies.
 */

#ifndef _SAMPLER_H
#define _SAMPLER_H

#include "core/vec3.h"

#include <stdint.h>

typedef struct sampler_rng {
        uint64_t state;
} sampler_rng_t;

typedef struct sampler {
        void* state;
        vec3_t (*sample_pixel_offset)(void* state, uint32_t pixel_x,
                                      uint32_t pixel_y, uint32_t sample_index,
                                      uint32_t sample_count);
        vec3_t (*sample_unit_vector)(void* state);
} sampler_t;

void   sampler_init_default(sampler_t* sampler, sampler_rng_t* state,
                            uint64_t seed);
vec3_t sampler_sample_pixel_offset(const sampler_t* sampler, uint32_t pixel_x,
                                   uint32_t pixel_y, uint32_t sample_index,
                                   uint32_t sample_count);
vec3_t sampler_sample_unit_vector(const sampler_t* sampler);

#endif /* _SAMPLER_H */