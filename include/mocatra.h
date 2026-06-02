#ifndef _MOCATRA_H
#define _MOCATRA_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "ray.h"
#include "vec3.h"
#include "image.h"

/* constants */
static const float INF = INFINITY;
static const float PI  = 3.14159f;

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

#endif /* _MOCATRA_H */
