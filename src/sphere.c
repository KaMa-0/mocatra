#include <math.h>
#include <stdlib.h>

#include "sphere.h"

static uint8_t
sphere_hit(const hittable_t* self, const ray_t r, 
           float t_min, float t_max, hit_record_t* rec)
{
        const sphere_t* s;
        vec3_t          oc, outward_normal;
        float           a, h, c, discriminant, sqrtd, root;

        s = (const sphere_t*)self;
        oc = vec3_sub(s->center, r.orig);

        a = vec3_len_squared(r.dir);
        h = vec3_dot(r.dir, oc);
        c = vec3_len_squared(oc) - s->radius * s->radius;

        discriminant = h * h - a * c;

        if (discriminant < 0)
                return 0; /* false */

        sqrtd = sqrt(discriminant);

        root = (h - sqrtd) / a;
        if (root <= t_min || t_max <= root) {
                root = (h + sqrtd) / a;
                if (root <= t_min || t_max <= root)
                        return 0; /* false */
        }

        rec->t         = root;
        rec->p         = ray_at(r, root);
        outward_normal = vec3_scal(vec3_sub(rec->p, s->center),
                                   (1.0f / s->radius));
        hit_record_set_face_normal(rec, r, outward_normal);

        return 1; /* true */
}

static void
sphere_destroy(hittable_t* self)
{
        free(self);
}

static const hittable_vtable_t sphere_vtable = { 
        .hit     = sphere_hit,
        .destroy = sphere_destroy,
};

void
sphere_init(sphere_t* s, vec3_t center, float radius)
{
        s->base.vtable = &sphere_vtable;
        s->center = center;
        s->radius = (radius > 0) ? radius : 0;
}
