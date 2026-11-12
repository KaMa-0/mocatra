/**
 * \file        material.h
 * \brief       Material properties including scattering and attenuation.
 */

#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "core/vec3.h"
#include "sampler/sampler.h"

#include <stdbool.h>

typedef enum material_type {
        MAT_LAMBERTIAN,
        MAT_METAL,
        MAT_DIFFUSE_LIGHT,
} material_type_t;

typedef struct material {
        material_type_t type;
        vec3_t          albedo;
        vec3_t          emission;
} material_t;

material_t material_lambertian(vec3_t albedo);
material_t material_metal(vec3_t albedo);
material_t material_light(vec3_t emission);

bool material_scatter(material_t mat, vec3_t in_dir, vec3_t normal,
                      vec3_t* out_dir, vec3_t* attenuation,
                      const sampler_t* sampler);

float material_scattering_pdf(material_t mat, vec3_t normal,
                              vec3_t scatter_dir);

#endif /* _MATERIAL_H */
