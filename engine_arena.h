/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

// data structure
typedef struct Arena {
	size_t head;
    size_t commited;
} Arena;

typedef struct Arena_Scope {
	Arena *arena;
	size_t head;
} Arena_Scope;

// api
Arena      *arena_alloc   (void);
void        arena_free    (Arena *arena);
void       *arena_insert  (Arena *arena, size_t size_in_bytes);
void       *arena_insert_nozeroed  (Arena *arena, size_t size_in_bytes);
void       *arena_insert_threadsafe(Arena *arena, size_t size_in_bytes);
void       *arena_insert_aligned  (Arena *arena, size_t size_in_bytes, size_t alignment_in_bytes);
void        arena_remove  (Arena *arena, size_t size_in_bytes);
void        arena_clear   (Arena *arena);
void        arena_set_head(Arena *arena, size_t head);
Arena_Scope arena_scope_begin(Arena* arena);
void        arena_scope_end(Arena_Scope scope);
#define ARENA_INSERT_ARRAY(arena, type, count) (type *)arena_insert((arena), sizeof(type)*(count))
#define ARENA_INSERT_ARRAY_NOZEROED(arena, type, count) (type *)arena_insert_nozeroed((arena), sizeof(type)*(count))
#define ARENA_INSERT_ARRAY_ALIGNED(arena, type, count, alignment_in_bytes) (type *)arena_insert_aligned((arena), sizeof(type)*(count), alignment_in_bytes)
#define ARENA_INSERT_STRUCT(arena, type) ARENA_INSERT_ARRAY(arena, type, 1)
#define ARENA_INSERT_STRUCT_NOZEROED(arena, type) ARENA_INSERT_ARRAY_NOZEROED(arena, type, 1)
#define ARENA_INSERT_STRUCT_ALIGNED(arena, type, alignment_in_bytes) ARENA_INSERT_ARRAY_ALIGNED(arena, type, 1, alignment_in_bytes)
#define ARENA_SCOPE(arena) for(size_t _arena_scope_head_ = (arena->head); _arena_scope_head_ != (size_t)-1; _arena_scope_head_ = ((arena->head) = _arena_scope_head_, (size_t)-1))

// backend agnostic implementations
void arena_remove(Arena *arena, size_t size_in_bytes) { arena->head -= size_in_bytes; }
void arena_clear(Arena *arena) { arena->head  = 0; }
void arena_set_head(Arena *arena, size_t head) { arena->head  = head; }
Arena_Scope arena_scope_begin(Arena* arena) { Arena_Scope scope; scope.arena = arena; scope.head = arena->head; return scope; }
void arena_scope_end (Arena_Scope scope) { scope.arena->head = scope.head; }

#ifdef ARENA_ENABLE_INSERT_S8
void arena_insert_s8_copy(Arena *dst, S8 *src);
S16  arena_insert_s16_from_s8(Arena *dst, S8 *src);
#endif /* ARENA_ENABLE_INSERT_S8 */