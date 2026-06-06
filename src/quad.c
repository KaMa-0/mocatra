#include <stdlib.h>
#include <math.h>

#include "mocatra.h"
#include "quad.h"

static uint8_t 
quad_hit(const hittable_t* self, const ray_t r, float t_min, float t_max, 
         hit_record_t* rec)
{
        const quad_t* quad;
        vec3_t        intersection_point;
        vec3_t        planar_hitpoint_vector;
        float         denom;
        float         t;
        float         alpha;
        float         beta;

        quad = (const quad_t*)self;

        denom = vec3_dot(quad->normal, r.dir);

        if (fabs(denom) < 1e-8f) {
                return 0;
        }

        t = (quad->d - vec3_dot(quad->normal, r.orig)) / denom;
        if (t < t_min || t > t_max) {
                return 0;
        }

        intersection_point     = vec3_add(r.orig, vec3_scal(r.dir, t));
        planar_hitpoint_vector = vec3_sub(intersection_point, quad->q);
        
        alpha = vec3_dot(quad->w, vec3_cross(planar_hitpoint_vector, quad->v));
        beta  = vec3_dot(quad->w, vec3_cross(quad->u, planar_hitpoint_vector));

        if (alpha < 0.0f || alpha > 1.0f || beta < 0.0f || beta > 1.0f) {
                return 0;
        }

        rec->t   = t;
        rec->p   = intersection_point;
        rec->mat = quad->mat;
        
        /* Sets rec->front_face and adjusts rec->normal to point against the ray */
        hit_record_set_face_normal(rec, r, quad->normal);

        return 1;
}

static void
quad_destroy(hittable_t* self)
{
        free(self);
}

static const hittable_vtable_t quad_vtable = { 
        .hit     = quad_hit,
        .destroy = quad_destroy,
};

quad_t*
quad_create(void)
{
        quad_t* quad;
        quad = malloc(sizeof(quad_t));
        if (quad == NULL) {
                printf("[ERROR] Memory Allocation failed for quad_t\n");
                return NULL;
        }
        return quad;
}

void
quad_init(quad_t* quad, vec3_t q, vec3_t u, vec3_t v, material_t mat)
{
        vec3_t n;
        quad->base.vtable = &quad_vtable;
        quad->q   = q;
        quad->u   = u;
        quad->v   = v;
        quad->mat = mat;

        n = vec3_cross(u, v);
        quad->normal = vec3_unit(n);
        quad->d      = vec3_dot(quad->normal, q);
        
        quad->w = vec3_scal(n, 1.0f / vec3_len_squared(n));
}

void
hittable_list_add_box(hittable_list_t* list, vec3_t p0, vec3_t p1, 
                      float angle_degrees, vec3_t translation, material_t mat)
{
        vec3_t min_pt;
        vec3_t max_pt;
        float radians;
        float sin_theta;
        float cos_theta;
        
        vec3_t local_front_origin;
        vec3_t local_right_origin;
        vec3_t local_back_origin;
        vec3_t local_left_origin;
        vec3_t local_top_origin;
        vec3_t local_bottom_origin;

        vec3_t local_front_u,  local_front_v;
        vec3_t local_right_u,  local_right_v;
        vec3_t local_back_u,   local_back_v;
        vec3_t local_left_u,   local_left_v;
        vec3_t local_top_u,    local_top_v;
        vec3_t local_bottom_u, local_bottom_v;

        vec3_t w_front_q,  w_front_u,  w_front_v;
        vec3_t w_right_q,  w_right_u,  w_right_v;
        vec3_t w_back_q,   w_back_u,   w_back_v;
        vec3_t w_left_q,   w_left_u,   w_left_v;
        vec3_t w_top_q,    w_top_u,    w_top_v;
        vec3_t w_bottom_q, w_bottom_u, w_bottom_v;

        quad_t* f_front;
        quad_t* f_back;
        quad_t* f_right;
        quad_t* f_left;
        quad_t* f_top;
        quad_t* f_bottom;

        min_pt = (vec3_t){
                .x = fminf(p0.x, p1.x), 
                .y = fminf(p0.y, p1.y), 
                .z = fminf(p0.z, p1.z)
        };
        max_pt = (vec3_t){
                .x = fmaxf(p0.x, p1.x), 
                .y = fmaxf(p0.y, p1.y), 
                .z = fmaxf(p0.z, p1.z)
        };

        radians   = angle_degrees * 3.1415926535f / 180.0f;
        sin_theta = sinf(radians);
        cos_theta = cosf(radians);

        #define TRANSFORM_POINT(p) (vec3_t){ \
                .x = (cos_theta * (p).x + sin_theta * (p).z) + translation.x, \
                .y = (p).y + translation.y, \
                .z = (-sin_theta * (p).x + cos_theta * (p).z) + translation.z \
        }

        #define TRANSFORM_VECTOR(v) (vec3_t){ \
                .x = cos_theta * (v).x + sin_theta * (v).z, \
                .y = (v).y, \
                .z = -sin_theta * (v).x + cos_theta * (v).z \
        }

        local_front_origin  = (vec3_t){min_pt.x, min_pt.y, max_pt.z};
        local_front_u       = (vec3_t){max_pt.x - min_pt.x, 0, 0};
        local_front_v       = (vec3_t){0, max_pt.y - min_pt.y, 0};

        local_right_origin  = (vec3_t){max_pt.x, min_pt.y, max_pt.z};
        local_right_u       = (vec3_t){0, 0, min_pt.z - max_pt.z};
        local_right_v       = (vec3_t){0, max_pt.y - min_pt.y, 0};

        local_back_origin   = (vec3_t){max_pt.x, min_pt.y, min_pt.z};
        local_back_u        = (vec3_t){min_pt.x - max_pt.x, 0, 0};
        local_back_v        = (vec3_t){0, max_pt.y - min_pt.y, 0};

        local_left_origin   = (vec3_t){min_pt.x, min_pt.y, min_pt.z};
        local_left_u        = (vec3_t){0, 0, max_pt.z - min_pt.z};
        local_left_v        = (vec3_t){0, max_pt.y - min_pt.y, 0};

        local_top_origin    = (vec3_t){min_pt.x, max_pt.y, max_pt.z};
        local_top_u         = (vec3_t){max_pt.x - min_pt.x, 0, 0};
        local_top_v         = (vec3_t){0, 0, min_pt.z - max_pt.z};

        local_bottom_origin = (vec3_t){min_pt.x, min_pt.y, min_pt.z};
        local_bottom_u      = (vec3_t){max_pt.x - min_pt.x, 0, 0};
        local_bottom_v      = (vec3_t){0, 0, max_pt.z - min_pt.z};

        w_front_q  = TRANSFORM_POINT(local_front_origin);
        w_front_u  = TRANSFORM_VECTOR(local_front_u);
        w_front_v  = TRANSFORM_VECTOR(local_front_v);

        w_right_q  = TRANSFORM_POINT(local_right_origin);
        w_right_u  = TRANSFORM_VECTOR(local_right_u);
        w_right_v  = TRANSFORM_VECTOR(local_right_v);

        w_back_q   = TRANSFORM_POINT(local_back_origin);
        w_back_u   = TRANSFORM_VECTOR(local_back_u);
        w_back_v   = TRANSFORM_VECTOR(local_back_v);

        w_left_q   = TRANSFORM_POINT(local_left_origin);
        w_left_u   = TRANSFORM_VECTOR(local_left_u);
        w_left_v   = TRANSFORM_VECTOR(local_left_v);

        w_top_q    = TRANSFORM_POINT(local_top_origin);
        w_top_u    = TRANSFORM_VECTOR(local_top_u);
        w_top_v    = TRANSFORM_VECTOR(local_top_v);

        w_bottom_q = TRANSFORM_POINT(local_bottom_origin);
        w_bottom_u = TRANSFORM_VECTOR(local_bottom_u);
        w_bottom_v = TRANSFORM_VECTOR(local_bottom_v);

        #undef TRANSFORM_POINT
        #undef TRANSFORM_VECTOR

        f_front  = quad_create();
        f_back   = quad_create();
        f_right  = quad_create();
        f_left   = quad_create();
        f_top    = quad_create();
        f_bottom = quad_create();

        quad_init(f_front,  w_front_q,  w_front_u,  w_front_v,  mat);
        quad_init(f_right,  w_right_q,  w_right_u,  w_right_v,  mat);
        quad_init(f_back,   w_back_q,   w_back_u,   w_back_v,   mat);
        quad_init(f_left,   w_left_q,   w_left_u,   w_left_v,   mat);
        quad_init(f_top,    w_top_q,    w_top_u,    w_top_v,    mat);
        quad_init(f_bottom, w_bottom_q, w_bottom_u, w_bottom_v, mat);

        hittable_list_add(list, (hittable_t*)f_front);
        hittable_list_add(list, (hittable_t*)f_right);
        hittable_list_add(list, (hittable_t*)f_back);
        hittable_list_add(list, (hittable_t*)f_left);
        hittable_list_add(list, (hittable_t*)f_top);
        hittable_list_add(list, (hittable_t*)f_bottom);
}
