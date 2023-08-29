// types
typedef unsigned char      Arena_U8;
typedef unsigned long long Arena_U64;

// data structure
typedef struct Arena {
	Arena_U64 head;
    Arena_U64 commited;
} Arena;

typedef struct Arena_Scope {
	Arena*    arena;
	Arena_U64 head;
} Arena_Scope;

// api
Arena      *arena_alloc   (void);
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
Arena_Scope arena_scope_begin(Arena* arena) { Arena_Scope scope; scope.arena = arena; scope.head = arena->head; return scope; }
void arena_scope_end (Arena_Scope scope) { scope.arena->head = scope.head; }