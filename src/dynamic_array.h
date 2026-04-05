#pragma once

#ifndef DYNAMIC_ARRAY_H__
#define DYNAMIC_ARRAY_H__

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define IS_MSVC() (defined(_MSC_VER) && !defined(__llvm__) && !defined(__INTEL_COMPILER))

#if defined(__GNUC__) || defined(__clang__)
    #define DA_ALWAYS_INLINE __attribute__((always_inline)) static inline
#elif IS_MSVC()
    #define DA_ALWAYS_INLINE __forceinline
#else
    #define DA_ALWAYS_INLINE static inline
#endif

#ifndef DA_INITIAL_CAPACITY
    #define DA_INITIAL_CAPACITY 64
#endif

typedef struct {
    size_t count;
    size_t cap;
    size_t typesz;
} DA_MetaData;

#define da_metadata(da) ((DA_MetaData*)((uint8_t*)(da) - sizeof(DA_MetaData))) // Subtracting the size of DA_MetaData from the pointer to get it's address
#define da_data(md)     ((void*)       ((uint8_t*)(md) + sizeof(DA_MetaData))) // Doing the opposite to get the address of the data
#define da_count(da)    (da_metadata(da))->count
#define da_cap(da)      (da_metadata(da))->cap
#define da_typesz(da)   (da_metadata(da))->typesz

#define da_alloc(da, capacity)                                                                   \
    do {                                                                                         \
        DA_MetaData *m = (DA_MetaData*)malloc(sizeof(DA_MetaData) + sizeof(*(da)) * (capacity)); \
        if (m == NULL) {                                                                         \
            (da) = NULL;                                                                         \
            break;                                                                               \
        }                                                                                        \
        m->count = 0;                                                                            \
        m->cap = (capacity);                                                                     \
        m->typesz = sizeof(*(da));                                                               \
        (da) = da_data(m);                                                                       \
    } while (0)

#define da_resize(da, factor)                                                   \
    do {                                                                        \
        assert((da) != NULL && "dynamic array can't be null");                  \
        DA_MetaData *m = da_metadata(da);                                       \
        size_t old_cap = m->cap;                                                \
        m->cap *= (factor);                                                     \
        if (m->cap < DA_INITIAL_CAPACITY) m->cap = DA_INITIAL_CAPACITY;         \
        DA_MetaData *m_old = m;                                                 \
        m = (DA_MetaData*)realloc(m, sizeof(DA_MetaData) + m->typesz * m->cap); \
        if (m == NULL) {                                                        \
            m = m_old;                                                          \
            m->cap = old_cap;                                                   \
            fprintf(stderr, "Reallocing %s failed\n", #da);                     \
            break;                                                              \
        }                                                                       \
        (da) = da_data(m);                                                      \
    } while (0)

#define da_free(da)              \
    do {                         \
        if ((da) == NULL) break; \
        free(da_metadata(da));   \
        (da) = NULL;             \
    } while (0)

#define da_push(da, item)                                                         \
    do {                                                                          \
        if ((da) == NULL) da_alloc(da, DA_INITIAL_CAPACITY);                      \
        else if (da_count(da) >= da_cap(da)) da_resize(da, 2);                    \
        assert((da) != NULL && "Allocating the dynamic array failed");            \
        assert(da_count(da) < da_cap(da) && "Resizing the dynamic array failed"); \
        (da)[da_count(da)] = (item);                                              \
        da_count(da) += 1;                                                        \
    } while (0)

#define da_insert(da, item, index)                                           \
    do {                                                                     \
        if ((da) == NULL || (index) >= da_count(da)) {                       \
            da_push(da, item);                                               \
            break;                                                           \
        }                                                                    \
        if (da_count(da) >= da_cap(da)) da_resize(da, 2);                    \
        size_t move_count = da_count(da) - (index);                          \
        memmove(&(da)[(index)+1], &(da)[index], move_count * sizeof(*(da))); \
        (da)[index] = (item);                                                \
        da_count(da) += 1;                                                   \
    } while (0)

#define DA_OFFSET(da, i) ((uint8_t*)(da) + (i) * da_typesz(da))

DA_ALWAYS_INLINE void* dynamic_array_remove(void *da, size_t index, bool shift)
{
    if (da == NULL) {
        fprintf(stderr, "Expected dynamic array, got null pointer\n");
        return NULL;
    }

    DA_MetaData *md = da_metadata(da);

    if (md->count == 0) return da;
    if (index >= md->count) index = md->count - 1;

    if (md->count < md->cap / 3) {
        da_resize(da, 0.5);
    }

    size_t move_count = md->count - (index + 1);
    md->count -= 1;

    if (shift && move_count > 0) {
        uint8_t temp[md->typesz];
        memcpy(temp, DA_OFFSET(da, index), md->typesz);
        memmove(DA_OFFSET(da, index), DA_OFFSET(da, index + 1), md->typesz * move_count);
        memcpy(DA_OFFSET(da, md->count), temp, md->typesz);
    }
    else if (move_count > 0) {
        uint8_t temp[md->typesz];
        memcpy(temp, DA_OFFSET(da, index), md->typesz);
        memcpy(DA_OFFSET(da, index), DA_OFFSET(da, md->count), md->typesz);
        memcpy(DA_OFFSET(da, md->count), temp, md->typesz);
    }

    return DA_OFFSET(da, md->count);
}

#if __STDC_VERSION__ >= 202311L
    #define DA_TYPE_OF typeof
#elif defined(__GNUC__) || defined(__clang__) || IS_MSVC()
    #define DA_TYPE_OF __typeof__
#endif

#ifdef DA_TYPE_OF
    #define da_pop(da)               (*(DA_TYPE_OF(da))dynamic_array_remove(da, da_count(da) - 1, false))
    #define da_remove(da, i)         (*(DA_TYPE_OF(da))dynamic_array_remove(da, i, true))
    /* No shifting, replaces the last element with the element to be removed */
    #define da_fast_remove(da, i)    (*(DA_TYPE_OF(da))dynamic_array_remove(da, i, false))
    #define da_foreach(da, in) \
        for (DA_TYPE_OF(da) in = (da); in < &(da)[da_count(da)]; in++)
#else
    #define da_pop(type, da)               (*(type*)dynamic_array_remove(da, da_count(da) - 1, false))
    #define da_remove(type, da, i)         (*(type*)dynamic_array_remove(da, i, true))
    #define da_fast_remove(type, da, i)    (*(type*)dynamic_array_remove(da, i, false))
    #define da_foreach(type, da, in) \
        for (type *in = (da); in < &(da)[da_count(da)]; in++)
#endif

#define da_clear(da) do { da_count(da) = 0; } while (0)

/* Both have to be dynamic arrays from this library */
#define da_copy(dest, src, n)                                   \
    do {                                                        \
        if ((dest) == NULL || (src) == NULL) break;             \
        for (size_t i = 0; i < (n) && i < da_count(src); i++) { \
            if (i < da_count(dest)) (dest)[i] = (src)[i];       \
            else da_push(dest, (src)[i]);                       \
        }                                                       \
    } while (0)

DA_ALWAYS_INLINE void* dynamic_array_from_regular_array(void *arr, size_t count, size_t typesz)
{
    if (arr == NULL || count == 0) return NULL;

    size_t total_bytes = count * typesz;
    DA_MetaData *md = (DA_MetaData*)malloc(sizeof(DA_MetaData) + total_bytes);
    assert(md != NULL && "Allocating dynamic array failed in dynamic_array_from_regular_array");
    md->cap = count;
    md->count = count;
    md->typesz = typesz;

    memcpy(da_data(md), arr, total_bytes);

    return da_data(md);
}

#define da_from_arr(arr, arr_count) dynamic_array_from_regular_array(arr, arr_count, sizeof(*(arr)));

#define da_print(da, fmt)                                                                                                               \
    do {                                                                                                                                \
        printf("<address: %p, count: %zu, capacity: %zu, type size: %zu>\n",                                                            \
            (void*)da_metadata(da), da_count(da), da_cap(da), da_typesz(da));                                                           \
        printf("[ ");                                                                                                                   \
        da_foreach(da, item) {                                                                                                          \
            printf(fmt" ", *item);                                                                                                      \
        }                                                                                                                               \
        printf("]\n");                                                                                                                  \
    } while (0)

#undef DA_ALWAYS_INLINE

#endif // DYNAMIC_ARRAY_H__
