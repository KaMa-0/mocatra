#ifndef _MOCATRA_H
#define _MOCATRA_H

#include <stdio.h>
#include <math.h>

#include "ray.h"
#include "vec3.h"
#include "image.h"

/* constants */
static const float INF = INFINITY;
static const float PI  = 3.14159;

static inline double
degrees_to_radians(double degrees)
{
    return degrees * PI / 180.0;
}

#endif /* _MOCATRA_H */
