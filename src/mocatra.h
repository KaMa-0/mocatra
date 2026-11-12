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
        MOCATRA_INVCONFIG,     /**< invalid mocatra user configuration */
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

static inline float
linear_to_gamma(float linear_component)
{
        if (linear_component > 0) {
                return sqrt(linear_component);
        }
        return 0;
}

#endif /* _MOCATRA_H */
