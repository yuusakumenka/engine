/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

inline I32       interlocked_increment32(volatile I32       *addend);
inline I64       interlocked_increment64(volatile I64       *addend);
inline ptrdiff_t interlocked_increment  (volatile ptrdiff_t *addend);
#define INTERLOCKED_INCREMENT(addend) interlocked_increment((ptrdiff_t *)(addend))
inline I32       interlocked_decrement32(volatile I32       *addend);
inline I64       interlocked_decrement64(volatile I64       *addend);
inline ptrdiff_t interlocked_decrement  (volatile ptrdiff_t *addend);
#define INTERLOCKED_DECREMENT(addend) interlocked_decrement((ptrdiff_t *)(addend))
inline I32       interlocked_add32(volatile I32       *addend, I32       value);
inline I64       interlocked_add64(volatile I64       *addend, I64       value);
inline ptrdiff_t interlocked_add  (volatile ptrdiff_t *addend, ptrdiff_t value);
#define INTERLOCKED_ADD(addend, value) interlocked_add((ptrdiff_t *)(addend), (value))
inline I32       interlocked_compare_exchange32(volatile I32       *destination, I32       exchange, I32       comparand);
inline I64       interlocked_compare_exchange64(volatile I64       *destination, I64       exchange, I64       comparand);
inline ptrdiff_t interlocked_compare_exchange  (volatile ptrdiff_t *destination, ptrdiff_t exchange, ptrdiff_t comparand);
#define INTERLOCKED_COMPARE_EXCHANGE(destination, exchange, comparand) interlocked_compare_exchange((ptrdiff_t *)(destination), (exchange), (comparand))
inline I32       interlocked_exchange32(volatile I32       *destination, I32       exchange);
inline I64       interlocked_exchange64(volatile I64       *destination, I64       exchange);
inline ptrdiff_t interlocked_exchange  (volatile ptrdiff_t *destination, ptrdiff_t exchange);
#define INTERLOCKED_EXCHANGE(destination, exchange) interlocked_exchange((ptrdiff_t *)(destination), (exchange))
inline I32       interlocked_exchange_add32(volatile I32       *destination, I32       value);
inline I64       interlocked_exchange_add64(volatile I64       *destination, I64       value);
inline ptrdiff_t interlocked_exchange_add  (volatile ptrdiff_t *destination, ptrdiff_t value);
#define INTERLOCKED_EXCHANGE_ADD(destination, value) interlocked_exchange_add((ptrdiff_t *)(destination), (value))