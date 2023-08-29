// types
typedef unsigned char      Ring_U8;
typedef unsigned long long Ring_U64;

// data structure
typedef struct Ring {
	Ring_U64 head;
	Ring_U64 tail;
	void*    data;
} Ring;

// api
Ring        ring_alloc    (Ring_U64 size_in_bytes);
void        arena_free    (Arena *arena);
void       *arena_insert  (Arena *arena, Arena_U64 size_in_bytes);
void        arena_remove  (Arena *arena, Arena_U64 size_in_bytes);
void        arena_clear   (Arena *arena);
void        arena_set_head(Arena *arena, Arena_U64 head);
Arena_Scope arena_scope_begin(Arena* arena);
void        arena_scope_end(Arena_Scope scope);
#define ARENA_INSERT_ARRAY(arena, type, count) (type *)arena_insert((arena), sizeof(type)*(count))
#define ARENA_INSERT_STRUCT(arena, type) ARENA_INSERT_ARRAY(arena, type, 1)

// backend agnostic implementations
void arena_remove(Arena *arena, Arena_U64 size_in_bytes) { arena->head -= size_in_bytes; }
void arena_clear(Arena *arena) { arena->head  = 0; }
void arena_set_head(Arena *arena, Arena_U64 head) { arena->head  = head; }
Arena_Scope arena_scope_begin(Arena* arena) { return ((Arena_Scope){ arena, arena->head }); }
void arena_scope_end (Arena_Scope scope) { scope.arena->head = scope.head; }