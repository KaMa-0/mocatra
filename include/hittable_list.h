/**
 *
 *  \file   hittable_list.h
 *  \brief  List of hittable objects.
 *
 */

/*
 *  Author:  KaMa (https://github.com/KaMa-0/
 *                 https://gitlab.com/KaMa-0/)
 *
 */


#ifndef _HITTABLE_LIST_H
#define _HITTABLE_LIST_H

#include "hittable.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct hittable_list {
    hittable_t      base;
    hittable_t**    objects;
    int             size;
    int             capacity;
} hittable_list_t;

void hittable_list_init(hittable_list_t* list, int initial_capacity);
void hittable_list_add(hittable_list_t* list, hittable_t* object);
void hittable_list_cleanup(hittable_list_t* list);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HITTABLE_LIST_H */
