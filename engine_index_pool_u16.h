/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#include <intrin.h>
#ifndef INDEX_POOL_U16_COUNT_TRAILING_ZEROES64
#ifdef _MSC_VER
#ifndef _WIN64
#error x86 is not supported. Please compile targeting x64.
#endif
#define INDEX_POOL_U16_COUNT_TRAILING_ZEROES64 _tzcnt_u64
#elif __GNUC__ /* GCC */
#define INDEX_POOL_U16_COUNT_TRAILING_ZEROES64 __builtin_ctzll
#else /* Unknown */
#error Unsuported compiler.
#endif /* Compiler */
#endif /* !INDEX_POOL_U16_COUNT_TRAILING_ZEROES64 */

#ifndef INDEX_POOL_U16_COUNT_LEADING_ZEROES64
#ifdef _MSC_VER
#define INDEX_POOL_U16_COUNT_LEADING_ZEROES64  _lzcnt_u64
#elif __GNUC__ /* GCC */
#define INDEX_POOL_U16_COUNT_LEADING_ZEROES64  __builtin_clzll
#else /* Unknown */
#error Unsuported compiler.
#endif /* Compiler */
#endif /* !INDEX_POOL_U16_COUNT_LEADING_ZEROES64 */

/* Structure */

/* A pool of unused U16 indicies. 
 * First remove from the pool to aquire usage. 
 * Later insert back to the pool to release usage. */
typedef struct Index_Pool_U16 {
    unsigned short     tail; /* All indicies from 0x0000 to tail are in use. */
    unsigned short     head; /* All indicies from head to 0xFFFF are not used. */
    unsigned long long in_use_mask[1024];
} Index_Pool_U16;

/* API */
void index_pool_u16_set_in_use(Index_Pool_U16 *pool, unsigned short index_range_start, unsigned short index_range_end);
void index_pool_u16_remove    (Index_Pool_U16 *pool, unsigned short num_indices, unsigned short *indicies);
void index_pool_u16_insert    (Index_Pool_U16 *pool, unsigned short num_indices, unsigned short *indicies);
void index_pool_u16_clear     (Index_Pool_U16 *pool);

/* Implementation */
#ifndef INDEX_POOL_U16_DISABLE_IMPLEMENTATION
void index_pool_u16_set_in_use(Index_Pool_U16 *pool, unsigned short index_range_start, unsigned short index_range_end) {
    unsigned short     start_mask_index = index_range_start >> 6;
    unsigned short     end_mask_index   = index_range_end   >> 6;
    unsigned short     start_shift      = index_range_start & 0x3F;
    unsigned short     end_shift        = index_range_end   & 0x3F;
    unsigned long long start_mask       = ~((1ULL << start_shift) - 1ULL);
    unsigned long long end_mask         = ((1ULL << end_shift)) - 1ULL;
    if(start_mask_index == end_mask_index) {
        unsigned long long mask = start_mask & end_mask;
        pool->in_use_mask[start_mask_index] |= mask;
    } else {
        pool->in_use_mask[start_mask_index] |= start_mask;
        for(U16 mask_index = start_mask_index; mask_index != end_mask_index; ++mask_index)
            pool->in_use_mask[mask_index] = 0xFFFFFFFFFFFFFFFFULL;
        pool->in_use_mask[end_mask_index] |= end_mask;
    }
}

void index_pool_u16_remove(Index_Pool_U16 *pool, unsigned short num_indices, unsigned short *indicies) {
    unsigned short head = pool->head;
    unsigned short tail = pool->tail;
    unsigned short index_index = 0;
    /* First try restoring the tail/head balance if necesary. */
    for (; index_index != num_indices && tail != head; ++index_index) {
        /* we know the tail points to a index not in use */
        indicies[index_index] = tail;

        /* mark as used */
        unsigned short     mask_index = tail >> 6;
        unsigned short     shift      = tail & 0x3F;
        unsigned long long mask       = 1ULL << shift;
        unsigned long long new_mask   = pool->in_use_mask[mask_index] | mask;
        pool->in_use_mask[mask_index] = new_mask;

        /* relocate the tail to the next index not in use */
        unsigned long long new_local_tail;
        for(;;) {
            new_local_tail = INDEX_POOL_U16_COUNT_TRAILING_ZEROES64(pool->in_use_mask[mask_index] + 1);
            if(new_local_tail != 64) 
                break;
            ++mask_index;
        }
        tail = ((unsigned long long)mask_index) << 6 | new_local_tail;
    }
    if(index_index != num_indices) {
        for (; index_index != num_indices; ++index_index) {
            indicies[index_index] = head;
            ++head;
        }
        index_pool_u16_set_in_use(pool, tail, head);
        tail = head;
    }
    pool->head = head;
    pool->tail = tail;
}

void index_pool_u16_insert(Index_Pool_U16 *pool, unsigned short num_indices, unsigned short *indicies) {
    unsigned short head = pool->head;
    unsigned short tail = pool->tail;
    for (unsigned short index_index = 0; index_index != num_indices; ++index_index) {
        unsigned short index = indicies[index_index];
        unsigned short     mask_index = index >> 6;
        unsigned short     shift      = index & 0x3F;
        unsigned long long mask       = 1ULL << shift;
        pool->in_use_mask[mask_index] &= ~mask;
        if(index == (head - 1)) {
            /* Relocate the head to the next index not in use */
            unsigned long long new_local_head;
            for(;;) {
                new_local_head = 64 - INDEX_POOL_U16_COUNT_LEADING_ZEROES64(pool->in_use_mask[mask_index]);
                if(new_local_head != 0 || mask_index == 0) 
                    break;
                --mask_index;
            }
            unsigned short new_global_head = (mask_index << 6) + new_local_head;
            head = new_global_head;
        }
        if(index < tail)
            tail = index;
    }
    pool->head = head;
    pool->tail = tail;
}

void index_pool_u16_clear(Index_Pool_U16 *pool) {
    memset(pool, 0x00000000, ((Index_Pool_U16*)0)->in_use_mask[(pool->head >> 6) + 1]);
}

int index_pool_u16_is_in_use(Index_Pool_U16 *pool, unsigned short index) {
    if(index < pool->tail) 
        return 1;
    unsigned short     mask_index = index >> 6;
    unsigned short     shift      = index & 0x3F;
    unsigned long long index_mask = 1ULL << shift;
    unsigned long long mask       = pool->in_use_mask[mask_index];
    return (mask & index_mask) != 0;
}
#endif /* !INDEX_POOL_U16_DISABLE_IMPLEMENTATION */
void index_pool_u16_remove_at (Index_Pool_U16 *pool, unsigned short num_indices, unsigned short *indicies);