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
                      material_t mat)
{
        vec3_t min_pt;
        vec3_t max_pt;
        quad_t* f_front;
        quad_t* f_back;
        quad_t* f_right;
        quad_t* f_left;
        quad_t* f_top;
        quad_t* f_bottom;

        min_pt = (vec3_t){.x = fminf(p0.x, p1.x), .y = fminf(p0.y, p1.y), .z = fminf(p0.z, p1.z)};
        max_pt = (vec3_t){.x = fmaxf(p0.x, p1.x), .y = fmaxf(p0.y, p1.y), .z = fmaxf(p0.z, p1.z)};

        f_front  = quad_create();
        f_back   = quad_create();
        f_right  = quad_create();
        f_left   = quad_create();
        f_top    = quad_create();
        f_bottom = quad_create();

        /* Front face */
        quad_init(f_front, 
                  (vec3_t){min_pt.x, min_pt.y, max_pt.z}, 
                  (vec3_t){max_pt.x - min_pt.x, 0, 0}, 
                  (vec3_t){0, max_pt.y - min_pt.y, 0}, mat);

        /* Right face */
        quad_init(f_right, 
                  (vec3_t){max_pt.x, min_pt.y, max_pt.z}, 
                  (vec3_t){0, 0, min_pt.z - max_pt.z}, 
                  (vec3_t){0, max_pt.y - min_pt.y, 0}, mat);

        /* Back face */
        quad_init(f_back, 
                  (vec3_t){max_pt.x, min_pt.y, min_pt.z}, 
                  (vec3_t){min_pt.x - max_pt.x, 0, 0}, 
                  (vec3_t){0, max_pt.y - min_pt.y, 0}, mat);

        /* Left face */
        quad_init(f_left, 
                  (vec3_t){min_pt.x, min_pt.y, min_pt.z}, 
                  (vec3_t){0, 0, max_pt.z - min_pt.z}, 
                  (vec3_t){0, max_pt.y - min_pt.y, 0}, mat);

        /* Top face */
        quad_init(f_top, 
                  (vec3_t){min_pt.x, max_pt.y, max_pt.z}, 
                  (vec3_t){max_pt.x - min_pt.x, 0, 0}, 
                  (vec3_t){0, 0, min_pt.z - max_pt.z}, mat);

        /* Bottom face */
        quad_init(f_bottom, 
                  (vec3_t){min_pt.x, min_pt.y, min_pt.z}, 
                  (vec3_t){max_pt.x - min_pt.x, 0, 0}, 
                  (vec3_t){0, 0, max_pt.z - min_pt.z}, mat);

        hittable_list_add(list, (hittable_t*)f_front);
        hittable_list_add(list, (hittable_t*)f_right);
        hittable_list_add(list, (hittable_t*)f_back);
        hittable_list_add(list, (hittable_t*)f_left);
        hittable_list_add(list, (hittable_t*)f_top);
        hittable_list_add(list, (hittable_t*)f_bottom);
}
