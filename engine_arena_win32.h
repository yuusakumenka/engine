/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

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
	size_t initial_commit_size = (4ULL << 10); // 4kB (common page size)
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
void *arena_insert(Arena *arena, size_t size_in_bytes) {
	void *data = ((char*)arena) + sizeof(*arena) + arena->head;
	arena->head += size_in_bytes;
	if (sizeof(*arena) + arena->head > arena->commited) {
		size_t commit_size = (((sizeof(*arena) + arena->head - arena->commited) + (4 << 10) - 1) / (4 << 10)) * (4 << 10);
		VirtualAlloc(
			((char*)arena) + arena->commited,
			commit_size,
			MEM_COMMIT,
			PAGE_READWRITE
		);
		arena->commited += commit_size;
	}
	memset(data, 0, size_in_bytes);
	return data;
}
void *arena_insert_nozeroed(Arena *arena, size_t size_in_bytes) {
	void *data = ((char*)arena) + sizeof(*arena) + arena->head;
	arena->head += size_in_bytes;
	if (sizeof(*arena) + arena->head <= arena->commited) return data;
	size_t commit_size = (((sizeof(*arena) + arena->head - arena->commited) + (4 << 10) - 1) / (4 << 10)) * (4 << 10);
	VirtualAlloc(
		((char*)arena) + arena->commited,
		commit_size,
		MEM_COMMIT,
		PAGE_READWRITE
	);
	arena->commited += commit_size;
	return data;
}
void *arena_insert_aligned(Arena *arena, size_t size_in_bytes, size_t alignment_in_bytes) {
	size_t alignment_mask = alignment_in_bytes - 1;
	size_t aligned_head   = arena->head + alignment_mask & ~alignment_mask;
	void *data = ((char*)arena) + sizeof(*arena) + aligned_head;
	arena->head += aligned_head + size_in_bytes;
	if (sizeof(*arena) + arena->head <= arena->commited) return data;
	size_t commit_size = (((sizeof(*arena) + arena->head - arena->commited) + (4 << 10) - 1) / (4 << 10)) * (4 << 10);
	VirtualAlloc(
		((char*)arena) + arena->commited,
		commit_size,
		MEM_COMMIT,
		PAGE_READWRITE
	);
	arena->commited += commit_size;
	return data;
}