/**
 * \file        material.c
 */

#include "material/material.h"

#include "mocatra.h"

#define NEAR_ZERO_SQ 1e-6f

material_t
material_lambertian(vec3_t albedo)
{
        return (material_t){
                .type     = MAT_LAMBERTIAN,
                .albedo   = albedo,
                .emission = v3(0.0f, 0.0f, 0.0f),
        };
}

material_t
material_metal(vec3_t albedo)
{
        return (material_t){
                .type     = MAT_METAL,
                .albedo   = albedo,
                .emission = v3(0.0f, 0.0f, 0.0f),
        };
}

material_t
material_light(vec3_t emission)
{
        return (material_t){.type = MAT_DIFFUSE_LIGHT, .emission = emission};
}

bool
material_scatter(material_t mat, vec3_t in_dir, vec3_t normal, vec3_t* out_dir,
                 vec3_t* attenuation)
{
        vec3_t dir;

        switch (mat.type) {
                case MAT_METAL:
                        dir = vec3_reflect(vec3_unit(in_dir), normal);
                        if (vec3_dot(dir, normal) <= 0.0f) {
                                return false;
                        }
                        break;
                case MAT_LAMBERTIAN:
                        dir = vec3_add(normal, random_unit_vector());
                        if (vec3_len_squared(dir) < NEAR_ZERO_SQ) {
                                dir = normal;
                        }
                        break;
                case MAT_DIFFUSE_LIGHT:
                default: return false;
        }

        *out_dir     = dir;
        *attenuation = mat.albedo;
        return true;
}

float
material_scattering_pdf(material_t mat, vec3_t normal, vec3_t scatter_dir)
{
        float cos_theta;

        if (mat.type == MAT_LAMBERTIAN) {
                cos_theta = vec3_dot(vec3_unit(scatter_dir), normal);
                return (cos_theta < 0.0f) ? 0.0f : cos_theta / (float)PI;
        }

        return 1.0f;
}
