#include "world.h"


#include "quad.h"


hittable_list_t*
create_world(size_t max_obj)
{
        hittable_list_t* world;
        
        world = hittable_list_create();
        hittable_list_init(world, max_obj);

        return world;
}

void
destroy_world(hittable_list_t* world)
{
        if (world == NULL)
                return;

        hittable_list_cleanup(world);
        free(world);
}

mocatra_error_t
init_world_cornell_box(hittable_list_t* world)
{
        quad_t *light_top, *wall_floor, *wall_ceiling, 
               *wall_back, *wall_left,  *wall_right;

        vec3_t  temp_q, temp_u, temp_v;

        /* diffuse light material */
        const material_t mat_diff_light = {
                .type = MAT_DIFFUSE_LIGHT,
                .emission = (vec3_t){15.0f, 15.0f, 15.0f}
        };

        /* diffuse materials */
        const material_t mat_diff_red = {
                .type = MAT_LAMBERTIAN,
                .albedo = (vec3_t){0.65f, 0.05f, 0.05f}
        };
        const material_t mat_diff_green = {
                .type = MAT_LAMBERTIAN,
                .albedo = (vec3_t){0.12f, 0.45f, 0.15f}
        };
        const material_t mat_diff_white = {
                .type = MAT_LAMBERTIAN,
                .albedo = (vec3_t){0.73f, 0.73f, 0.73f}
        };

        if (world == NULL)
                return MOCATRA_INVARG;

        /* quad creation */
        light_top    = quad_create();
        wall_floor   = quad_create();
        wall_ceiling = quad_create();
        wall_back    = quad_create();
        wall_right   = quad_create();
        wall_left    = quad_create();

        /* init quads (rectangle shapes) */
        quad_init(light_top, 
                  v3(213, 554, 227), v3(130, 0, 0), v3(0, 0, 105), 
                  mat_diff_light);
        quad_init(wall_floor, 
                  v3(0, 0, 0), v3(555, 0, 0), v3(0, 0, 555), 
                  mat_diff_white);
        quad_init(wall_ceiling, 
                  v3(0, 555, 0), v3(555, 0, 0), v3(0, 0, 555), 
                  mat_diff_white);
        quad_init(wall_left, 
                  v3(555, 0, 0), v3(0, 0, 555), v3(0, 555, 0), 
                  mat_diff_green);
        quad_init(wall_right, 
                  v3(0, 0, 0), v3(0, 0, 555), v3(0, 555, 0), 
                  mat_diff_red);
        quad_init(wall_back,
                  v3(0, 0, 555), v3(555, 0, 0), v3(0, 555, 0), 
                  mat_diff_white);
       
        hittable_list_add(world, (hittable_t*)light_top);
        hittable_list_add(world, (hittable_t*)wall_floor);
        hittable_list_add(world, (hittable_t*)wall_ceiling);
        hittable_list_add(world, (hittable_t*)wall_back);
        hittable_list_add(world, (hittable_t*)wall_left);
        hittable_list_add(world, (hittable_t*)wall_right);

        hittable_list_add_box(world, 
                              v3(0.0f,   0.0f,   0.0f), 
                              v3(165.0f, 165.0f, 165.0f), 
                              -18.0f, 
                              v3(130.0f, 0.0f,   65.0f), 
                              mat_diff_white);
        hittable_list_add_box(world, 
                              v3(0.0f,   0.0f,   0.0f), 
                              v3(165.0f, 330.0f, 165.0f), 
                              15.0f, 
                              v3(265.0f, 0.0f,   295.0f), 
                              mat_diff_white);

        return MOCATRA_OK;
}

mocatra_error_t
init_world_sphere_playground(hittable_list_t* world)
{
        

        if (world == NULL)
                return MOCATRA_INVARG;

        return MOCATRA_OK;
}

