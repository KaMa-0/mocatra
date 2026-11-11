/**
 * \file        world.c
 */

#include "scene/world.h"

#include "geometry/box.h"
#include "geometry/quad.h"
#include "material/material.h"

#define ROOM 555.0f /* edge length of the Cornell box */

typedef struct {
        hittable_list_t* list;
        mocatra_error_t  err;
} builder_t;

static mocatra_error_t
add_quad(builder_t* b, vec3_t q, vec3_t u, vec3_t v, material_t mat)
{
        if (b->err == MOCATRA_OK) {
                b->err = hittable_list_add_quad(b->list, q, u, v, mat);
        }

        return b->err;
}

/*
 * Axis-aligned rectangles. The name gives the two axes the edges run along
 * (first u, then v); arguments are always: the fixed coordinate, the start on
 * the u axis, the start on the v axis, the size along u, the size along v.
 */
static void
add_rect_xz(builder_t* b, float y, float x, float z, float dx, float dz,
            material_t mat)
{
        add_quad(b, v3(x, y, z), v3(dx, 0.0f, 0.0f), v3(0.0f, 0.0f, dz), mat);
}

static void
add_rect_zy(builder_t* b, float x, float z, float y, float dz, float dy,
            material_t mat)
{
        add_quad(b, v3(x, y, z), v3(0.0f, 0.0f, dz), v3(0.0f, dy, 0.0f), mat);
}

static void
add_rect_xy(builder_t* b, float z, float x, float y, float dx, float dy,
            material_t mat)
{
        add_quad(b, v3(x, y, z), v3(dx, 0.0f, 0.0f), v3(0.0f, dy, 0.0f), mat);
}

static void
add_box(builder_t* b, vec3_t p0, vec3_t p1, float degrees, vec3_t translation,
        material_t mat)
{
        if (b->err == MOCATRA_OK) {
                b->err = hittable_list_add_box(b->list, p0, p1, degrees,
                                               translation, mat);
        }
}

static mocatra_error_t
cornell_populate(hittable_list_t* world)
{
        builder_t b = {.list = world, .err = MOCATRA_OK};

        const material_t light = material_light(v3(15.0f, 15.0f, 15.0f));
        const material_t red   = material_lambertian(v3(0.65f, 0.05f, 0.05f));
        const material_t green = material_lambertian(v3(0.12f, 0.45f, 0.15f));
        const material_t white = material_lambertian(v3(0.73f, 0.73f, 0.73f));

        add_rect_xz(&b, ROOM - 1.0f, 213.0f, 227.0f, 130.0f, 105.0f, light);

        add_rect_xz(&b, 0.0f, 0.0f, 0.0f, ROOM, ROOM, white);
        add_rect_xz(&b, ROOM, 0.0f, 0.0f, ROOM, ROOM, white);
        add_rect_xy(&b, ROOM, 0.0f, 0.0f, ROOM, ROOM, white);
        add_rect_zy(&b, ROOM, 0.0f, 0.0f, ROOM, ROOM, green);
        add_rect_zy(&b, 0.0f, 0.0f, 0.0f, ROOM, ROOM, red);

        add_box(&b, v3(0, 0, 0), v3(165, 165, 165), -18.0f, v3(130, 0, 65),
                white);
        add_box(&b, v3(0, 0, 0), v3(165, 330, 165), 15.0f, v3(265, 0, 295),
                white);

        return b.err;
}

hittable_list_t*
world_create(size_t max_obj)
{
        hittable_list_t* world;

        world = hittable_list_create();

        if (world == NULL) {
                return NULL;
        }

        if (hittable_list_init(world, max_obj) != MOCATRA_OK) {
                return NULL;
        }

        return world;
}

void
world_destroy(hittable_list_t* world)
{
        if (world == NULL) {
                return;
        }

        hittable_list_destroy(world);
        free(world);
}

mocatra_error_t
init_world_cornell_box(hittable_list_t* world)
{
        if (world == NULL) {
                return MOCATRA_INVARG;
        }

        return cornell_populate(world);
}
