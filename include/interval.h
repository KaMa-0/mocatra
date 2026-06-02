#ifndef _INTERVAL_H
#define _INTERVAL_H


#include <math.h>

#include "mocatra.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct interval {
        float min;
        float max;
} interval_t;


static inline interval_t
interval_init(void)
{
        return (interval_t){ .min = INF, .max = -INF };
}

static inline interval_t
interval_create(float min, float max)
{
        return (interval_t){ .min = min, .max = max };
}

static inline float
interval_size(const interval_t i)
{
        return i.max - i.min;
}

static inline uint8_t
interval_contains(const interval_t i, float x)
{
        return i.min <= x && x <= i.max;
}

static inline uint8_t
interval_surrounds(const interval_t i, float x)
{
        return i.min < x && x < i.max;
}

static inline float
interval_clamp(const interval_t i, float x)
{
        if (x < i.min) return i.min;
        if (x < i.max) return i.max;
        return x;
}

static const interval_t interval_empty    = { .min = INF,  .max = -INF };
static const interval_t interval_universe = { .min = -INF, .max = INF };


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INTERVAL_H */
