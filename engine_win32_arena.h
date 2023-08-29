#ifdef __INTELLISENSE__
#include <memoryapi.h>
#endif // __INTELLISENSE__

Arena *arena_alloc(void) {
	void* virtual_address = VirtualAlloc(
		NULL,
		// 16       GB     of virtual address space
        (  16ULL << 30  ),
		MEM_RESERVE,
        PAGE_NOACCESS
	);
	Arena_U64 initial_commit_size = (4ULL << 10); // 4kB (common page size)
    virtual_address = VirtualAlloc(
		virtual_address,
		initial_commit_size,
		MEM_COMMIT,
        PAGE_READWRITE
	);
	Arena *arena = (Arena*)virtual_address;
	arena->head     = 0; 
	arena->commited = initial_commit_size;
	return arena;
}
void arena_free(Arena *arena) {
	VirtualFree(
		arena,
		0ULL,
		MEM_RELEASE
	);
}
void *arena_insert(Arena *arena, Arena_U64 size_in_bytes) {
	void *data = ((char*)arena) + sizeof(*arena) + arena->head;
	arena->head += size_in_bytes;
	if (sizeof(*arena) + arena->head <= arena->commited) return data;
	Arena_U64 commit_size = (((sizeof(*arena) + arena->head - arena->commited) + (4 << 10) - 1) / (4 << 10)) * (4 << 10);
	VirtualAlloc(
		((char*)arena) + arena->commited,
		commit_size,
		MEM_COMMIT,
		PAGE_READWRITE
	);
	return data;
}