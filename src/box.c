/*
 * \file        box.c
 * */

#include "box.h"

#include "hittable_list.h"
#include "mocatra.h"
#include "quad.h"

void
hittable_list_add_box(hittable_list_t* list, vec3_t p0, vec3_t p1,
                      float angle_degrees, vec3_t translation, material_t mat)
{
        vec3_t          min_pt, max_pt;
        vec3_t          dx, dy, dz;
        quad_face_def_t local_faces[6];
        float           radians, sin_theta, cos_theta;

        if (list == NULL) {
                return;
        }

        min_pt = (vec3_t){
                .x = fminf(p0.x, p1.x),
                .y = fminf(p0.y, p1.y),
                .z = fminf(p0.z, p1.z),
        };

        max_pt = (vec3_t){
                .x = fmaxf(p0.x, p1.x),
                .y = fmaxf(p0.y, p1.y),
                .z = fmaxf(p0.z, p1.z),
        };

        dx = (vec3_t){.x = max_pt.x - min_pt.x, .y = 0.0f, .z = 0.0f};
        dy = (vec3_t){.x = 0.0f, .y = max_pt.y - min_pt.y, .z = 0.0f};
        dz = (vec3_t){.x = 0.0f, .y = 0.0f, .z = max_pt.z - min_pt.z};

        /* Front */
        local_faces[0] = (quad_face_def_t){
                .origin = {min_pt.x, min_pt.y, max_pt.z}, .u = dx, .v = dy};
        /* Right */
        local_faces[1] = (quad_face_def_t){.origin = {max_pt.x, min_pt.y,
                                                      max_pt.z},
                                           .u      = vec3_scal(dz, -1.0f),
                                           .v      = dy};
        /* Back */
        local_faces[2] = (quad_face_def_t){.origin = {max_pt.x, min_pt.y,
                                                      min_pt.z},
                                           .u      = vec3_scal(dx, -1.0f),
                                           .v      = dy};
        /* Left */
        local_faces[3] = (quad_face_def_t){
                .origin = {min_pt.x, min_pt.y, min_pt.z}, .u = dz, .v = dy};
        /* Top */
        local_faces[4] = (quad_face_def_t){.origin = {min_pt.x, max_pt.y,
                                                      max_pt.z},
                                           .u      = dx,
                                           .v      = vec3_scal(dz, -1.0f)};
        /* Bottom */
        local_faces[5] = (quad_face_def_t){
                .origin = {min_pt.x, min_pt.y, min_pt.z}, .u = dx, .v = dz};

        radians   = angle_degrees * PI / 180.0f;
        sin_theta = sinf(radians);
        cos_theta = cosf(radians);

        for (size_t idx = 0; idx < 6; ++idx) {
                quad_face_def_t f = local_faces[idx];
                vec3_t          w_q, w_u, w_v;
                quad_t*         q;

                /* Y-Axis Rotation + Translation */
                w_q = (vec3_t){
                        .x = (cos_theta * f.origin.x + sin_theta * f.origin.z)
                             + translation.x,
                        .y = f.origin.y + translation.y,
                        .z = (-sin_theta * f.origin.x + cos_theta * f.origin.z)
                             + translation.z,
                };

                w_u = (vec3_t){
                        .x = cos_theta * f.u.x + sin_theta * f.u.z,
                        .y = f.u.y,
                        .z = -sin_theta * f.u.x + cos_theta * f.u.z,
                };

                w_v = (vec3_t){
                        .x = cos_theta * f.v.x + sin_theta * f.v.z,
                        .y = f.v.y,
                        .z = -sin_theta * f.v.x + cos_theta * f.v.z,
                };

                q = quad_create();
                if (q != NULL) {
                        quad_init(q, w_q, w_u, w_v, mat);
                        hittable_list_add(list, (hittable_t*)q);
                }
        }
}
