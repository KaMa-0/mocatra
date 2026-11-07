#ifndef _MOCATRA_H
#define _MOCATRA_H

#include "core/vec3.h"
#include "mocatra.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
        MOCATRA_OK = 0,        /**< function/program exited properly */
        MOCATRA_INVARG,        /**< invalid arguments given to function */
        MOCATRA_INVIMG,        /**< invalid values in image structure */
        MOCATRA_INVSETTINGS,   /**< invalid render settings */
        MOCATRA_INIT_FAILED,   /**< mocatra init error */
        MOCATRA_NOMEM,         /**< memory allocation failed (out of memory) */
        MOCATRA_FILE_ERROR,    /**< file i/o operation failed */
        MOCATRA_ERROR_UNKNOWN, /**< unknown mocatra error */
        MOCATRA_ERROR,         /**< generic mocatra error */
} mocatra_error_t;

/* global constants */
static const float INF = INFINITY;
static const float PI  = 3.14159f;

int mocatra_main(void);

static inline double
degrees_to_radians(double degrees)
{
        return degrees * PI / 180.0f;
}

static inline double
random_float(float min, float max)
{
        return min + (max - min) * (rand() / (RAND_MAX + 1.0f));
}

static inline vec3_t
random_vec3_t(float min, float max)
{
        return (vec3_t){
                .x = random_float(min, max),
                .y = random_float(min, max),
                .z = random_float(min, max),
        };
}

static inline vec3_t
random_unit_vector(void)
{
        vec3_t p;
        float  lensq;
        while (1) {
                p     = random_vec3_t(-1, 1);
                lensq = vec3_len_squared(p);
                if (1e-30f < lensq && lensq <= 1.0f) {
                        return vec3_scal(p, 1.0f / sqrt(lensq));
                }
        }
}

static inline vec3_t
random_on_hemisphere(const vec3_t normal)
{
        vec3_t on_unit_sphere = random_unit_vector();

        if (vec3_dot(on_unit_sphere, normal) > 0.0f) {
                return on_unit_sphere;
        } else {
                return (vec3_t){
                        .x = -on_unit_sphere.x,
                        .y = -on_unit_sphere.y,
                        .z = -on_unit_sphere.z,
                };
        }
}

static inline vec3_t
sample_square(void)
{
        return (vec3_t){
                .x = random_float(-0.5f, 0.5f),
                .y = random_float(-0.5f, 0.5f),
                .z = 0.0f,
        };
}

static inline float
linear_to_gamma(float linear_component)
{
        if (linear_component > 0) {
                return sqrt(linear_component);
        }
        return 0;
}

#endif /* _MOCATRA_H */
