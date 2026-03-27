#pragma once

#ifndef DYNAMIC_ARRAY_H__
#define DYNAMIC_ARRAY_H__

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef DA_INITIAL_CAPACITY
    #define DA_INITIAL_CAPACITY 64
#endif

typedef struct {
    size_t count;
    size_t cap;
} DA_MetaData;

#define da_metadata(da) ((DA_MetaData*)((uint8_t*)(da) - sizeof(DA_MetaData))) // Subtracting the size of DA_MetaData from the pointer to get it's address
#define da_data(md)     ((void*)       ((uint8_t*)(md) + sizeof(DA_MetaData))) // Doing the opposite to get the address of the data
#define da_count(da)    (da_metadata(da))->count
#define da_cap(da)      (da_metadata(da))->cap

#define da_alloc(da, capacity)                                                     \
    do {                                                                           \
        DA_MetaData* m = malloc(sizeof(DA_MetaData) + sizeof(*(da)) * (capacity)); \
        m->count = 0;                                                              \
        m->cap = (capacity);                                                       \
        (da) = da_data(m);                                                         \
    } while (0)

#define da_resize(da, factor)                                           \
    do {                                                                \
        DA_MetaData* m = da_metadata(da);                               \
        m->cap *= (factor);                                             \
        m = realloc(m, sizeof(DA_MetaData) + sizeof(*(da)) * (m->cap)); \
        (da) = da_data(m);                                              \
    } while (0)

#define da_free(da)            \
    do {                       \
        free(da_metadata(da)); \
        (da) = NULL;           \
    } while (0)

#define da_push(da, item)                                                 \
    do {                                                                  \
        if ((da) == NULL) da_alloc(da, DA_INITIAL_CAPACITY);              \
        else if (da_count(da) >= da_cap(da)) da_resize(da, 2);            \
        (da)[da_count(da)] = item;                                        \
        da_count(da) += 1;                                                \
    } while (0)


#endif // DYNAMIC_ARRAY_H__
