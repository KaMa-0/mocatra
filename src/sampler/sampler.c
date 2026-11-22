/**
 * \file        sampler.c
 */

#include "sampler/sampler.h"

#include <math.h>

static uint64_t
sampler_rng_next(sampler_rng_t* state)
{
        uint64_t value;

        /*************************************************************
        next() function taken from https://prng.di.unimi.it/splitmix.c
        written in 2015 by Sebastiano Vigna (vigna@acm.org)
        *************************************************************/
        value = (state->state += UINT64_C(0x9e3779b97f4a7c15));
        value = (value ^ (value >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        value = (value ^ (value >> 27)) * UINT64_C(0x94d049bb133111eb);
        return value ^ (value >> 31);
}

static float
sampler_random_float(sampler_rng_t* state, float min, float max)
{
        float sample;

        sample = (float)(sampler_rng_next(state) >> 40) * (1.0f / 16777216.0f);
        return min + (max - min) * sample;
}

static vec3_t
sampler_random_vec3(sampler_rng_t* state, float min, float max)
{
        return (vec3_t){
                .x = sampler_random_float(state, min, max),
                .y = sampler_random_float(state, min, max),
                .z = sampler_random_float(state, min, max),
        };
}

static vec3_t
sampler_random_unit_vector(void* state)
{
        sampler_rng_t* rng;
        vec3_t         p;
        float          lensq;

        rng = state;
        while (1) {
                p     = sampler_random_vec3(rng, -1, 1);
                lensq = vec3_len_squared(p);
                if (1e-30f < lensq && lensq <= 1.0f) {
                        return vec3_scal(p, 1.0f / sqrt(lensq));
                }
        }
}

static vec3_t
sampler_random_pixel_offset(void* state, uint32_t pixel_x, uint32_t pixel_y,
                            uint32_t sample_index, uint32_t sample_count)
{
        sampler_rng_t* rng;

        (void)pixel_x;
        (void)pixel_y;
        (void)sample_index;
        (void)sample_count;
        rng = state;
        return (vec3_t){
                .x = sampler_random_float(rng, -0.5f, 0.5f),
                .y = sampler_random_float(rng, -0.5f, 0.5f),
                .z = 0.0f,
        };
}

void
sampler_init_default(sampler_t* sampler, sampler_rng_t* state, uint64_t seed)
{
        *state = (sampler_rng_t){
                .state = seed,
        };

        *sampler = (sampler_t){
                .state               = state,
                .sample_pixel_offset = sampler_random_pixel_offset,
                .sample_unit_vector  = sampler_random_unit_vector,
        };
}

vec3_t
sampler_sample_pixel_offset(const sampler_t* sampler, uint32_t pixel_x,
                            uint32_t pixel_y, uint32_t sample_index,
                            uint32_t sample_count)
{
        return sampler->sample_pixel_offset(sampler->state, pixel_x, pixel_y,
                                            sample_index, sample_count);
}

vec3_t
sampler_sample_unit_vector(const sampler_t* sampler)
{
        return sampler->sample_unit_vector(sampler->state);
}