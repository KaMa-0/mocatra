#ifndef _MATERIALS_H
#define _MATERIALS_H

#include "mocatra.h"


typedef enum mat_type {
    MAT_LAMBERTIAN,
    MAT_DIFFUSE_LIGHT,
    MAT_METAL
} mat_type_t;

typedef struct material {
    mat_type_t  type;
    vec3_t      albedo;
    vec3_t      emission;
} material_t;


#endif /* _MOCATRA_H */
