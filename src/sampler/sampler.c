/**
 * \file        sampler.c
 */

#include "sampler/sampler.h"

#include <math.h>
#include <stdlib.h>

static float
sampler_random_float(float min, float max)
{
        return min + (max - min) * (rand() / (RAND_MAX + 1.0f));
}

static vec3_t
sampler_random_vec3(float min, float max)
{
        return (vec3_t){
                .x = sampler_random_float(min, max),
                .y = sampler_random_float(min, max),
                .z = sampler_random_float(min, max),
        };
}

static vec3_t
sampler_random_unit_vector(void* state)
{
        vec3_t p;
        float  lensq;

        (void)state;
        while (1) {
                p     = sampler_random_vec3(-1, 1);
                lensq = vec3_len_squared(p);
                if (1e-30f < lensq && lensq <= 1.0f) {
                        return vec3_scal(p, 1.0f / sqrt(lensq));
                }
        }
}

static vec3_t
sampler_random_pixel_offset(void* state)
{
        (void)state;
        return (vec3_t){
                .x = sampler_random_float(-0.5f, 0.5f),
                .y = sampler_random_float(-0.5f, 0.5f),
                .z = 0.0f,
        };
}

const sampler_t*
sampler_default(void)
{
        static const sampler_t default_sampler = {
                .state               = NULL,
                .sample_pixel_offset = sampler_random_pixel_offset,
                .sample_unit_vector  = sampler_random_unit_vector,
        };

        return &default_sampler;
}

vec3_t
sampler_sample_pixel_offset(const sampler_t* sampler)
{
        return sampler->sample_pixel_offset(sampler->state);
}

vec3_t
sampler_sample_unit_vector(const sampler_t* sampler)
{
        return sampler->sample_unit_vector(sampler->state);
}