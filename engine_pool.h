/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#define POOL_ASSERT(test, ...)
#define POOL_INTERLOCKED_INCREMENT(addend) ((*(addend))++)
#define POOL_INTERLOCKED_DECREMENT(addend) ((*(addend))--)
#define POOL_INTERLOCKED_ADD(addend, value) ((*(addend)) += value)
#define POOL_INTERLOCKED_SUB(addend, value) ((*(addend)) -= value)

typedef struct Pool {
    size_t  capacity;
    size_t  head;
    size_t *data;
} Pool;

size_t pool_size_max(Pool *pool) {
    return pool->capacity * sizeof(size_t);
}

void pool_fill_ascending(Pool *pool) {
    pool->head = pool->capacity;
    for (size_t i = 0; i < pool->capacity; ++i) {
        pool->data[i] = i;
    }
}

size_t pool_remove_one_threadsafe(Pool *pool) {
    size_t  head = POOL_INTERLOCKED_DECREMENT(&pool->head);
    return pool->data[head];
}

void pool_insert_one_threadsafe(Pool *pool, size_t data) {
    size_t  head = POOL_INTERLOCKED_INCREMENT(&pool->head);
    pool->data[head] = data;
}

void pool_remove_threadsafe(size_t *dst, size_t num, Pool *src) {
    size_t  head = POOL_INTERLOCKED_SUB(&src->head, num);
    size_t *data = &src->data[head];
    for (size_t i = 0; i < num; ++i) {
        *dst = *data;
        ++dst;
        ++data;
    }
}

void pool_insert_threadsafe(Pool *dst, size_t num, size_t *src) {
    size_t  head = POOL_INTERLOCKED_ADD(&dst->head, num);
    size_t *data = &dst->data[head];
    for (size_t i = 0; i < num; ++i) {
        *data = *src;
        ++data;
        ++src;
    }
}