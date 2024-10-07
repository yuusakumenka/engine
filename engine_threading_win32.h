/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

inline I32       interlocked_increment32(volatile I32       *addend) { return InterlockedIncrement  ((volatile LONG  *)addend); }
inline I64       interlocked_increment64(volatile I64       *addend) { return InterlockedIncrement64((volatile LONG64*)addend); }
inline ptrdiff_t interlocked_increment  (volatile ptrdiff_t *addend) { return InterlockedIncrement64((volatile LONG64*)addend); }
inline I32       interlocked_decrement32(volatile I32       *addend) { return InterlockedDecrement  ((volatile LONG  *)addend); }
inline I64       interlocked_decrement64(volatile I64       *addend) { return InterlockedDecrement64((volatile LONG64*)addend); }
inline ptrdiff_t interlocked_decrement  (volatile ptrdiff_t *addend) { return InterlockedDecrement64((volatile LONG64*)addend); }
inline I32       interlocked_add32(volatile I32       *addend, I32       value) { return _InlineInterlockedAdd  ((volatile LONG  *)addend, value); }
inline I64       interlocked_add64(volatile I64       *addend, I64       value) { return _InlineInterlockedAdd64((volatile LONG64*)addend, value); }
inline ptrdiff_t interlocked_add  (volatile ptrdiff_t *addend, ptrdiff_t value) { return _InlineInterlockedAdd64((volatile LONG64*)addend, value); }
inline I32       interlocked_compare_exchange32(volatile I32       *destination, I32       exchange, I32       comparand) { return InterlockedCompareExchange  ((volatile LONG  *)destination, (volatile LONG  )exchange, (volatile LONG  )comparand); }
inline I64       interlocked_compare_exchange64(volatile I64       *destination, I64       exchange, I64       comparand) { return InterlockedCompareExchange64((volatile LONG64*)destination, (volatile LONG64)exchange, (volatile LONG64)comparand); }
inline ptrdiff_t interlocked_compare_exchange  (volatile ptrdiff_t *destination, ptrdiff_t exchange, ptrdiff_t comparand) { return InterlockedCompareExchange64((volatile LONG64*)destination, (volatile LONG64)exchange, (volatile LONG64)comparand); }
inline I32       interlocked_exchange32(volatile I32       *destination, I32       exchange) { return InterlockedExchange  ((volatile LONG  *)destination, (volatile LONG  )exchange); }
inline I64       interlocked_exchange64(volatile I64       *destination, I64       exchange) { return InterlockedExchange64((volatile LONG64*)destination, (volatile LONG64)exchange); }
inline ptrdiff_t interlocked_exchange  (volatile ptrdiff_t *destination, ptrdiff_t exchange) { return InterlockedExchange64((volatile LONG64*)destination, (volatile LONG64)exchange); }
inline I32       interlocked_exchange_add32(volatile I32       *addend, I32       value) { return _InterlockedExchangeAdd  ((volatile LONG  *)addend, value); }
inline I64       interlocked_exchange_add64(volatile I64       *addend, I64       value) { return _InterlockedExchangeAdd64((volatile LONG64*)addend, value); }
inline ptrdiff_t interlocked_exchange_add  (volatile ptrdiff_t *addend, ptrdiff_t value) { return _InterlockedExchangeAdd64((volatile LONG64*)addend, value); }