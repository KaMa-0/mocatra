/**
 *
 *  \file   hittable_list.h
 *  \brief  List of hittable objects.
 *
 */

#ifndef _HITTABLE_LIST_H
#define _HITTABLE_LIST_H

#include "hittable.h"

#include "mocatra_error.h"

typedef struct hittable_list {
        hittable_t   base;
        hittable_t** objects;
        int          size;
        int          capacity;
} hittable_list_t;

hittable_list_t* hittable_list_create(void);
void             hittable_list_destroy(hittable_list_t* list);
mocatra_error_t  hittable_list_init(hittable_list_t* list, int initial_capacity);
mocatra_error_t  hittable_list_add(hittable_list_t* list, hittable_t* object);

#endif /* _HITTABLE_LIST_H */
