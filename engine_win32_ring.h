#ifdef __INTELLISENSE__
#include <memoryapi.h>
#include <engine_ring.h>
#endif // __INTELLISENSE__

Ring *ring_alloc(Ring_U64 size_in_bytes)) {
	void *placeholder1 = VirtualAlloc2 (
		NULL,
		NULL,
        size_in_bytes << 1,
		MEM_RESERVE | MEM_RESERVE_PLACEHOLDER, 
        PAGE_NOACCESS,
		NULL,
		0
	);
	// VirtualAlloc2 might fail

    BOOL result = VirtualFree (
        placeholder1,
        size_in_bytes,
        MEM_RELEASE | MEM_PRESERVE_PLACEHOLDER
    );

	// VirtualFree might fail	

    void *placeholder2 = (void*)((ULONG_PTR)placeholder1 + size_in_bytes);

	// Create a pagefile-backed section for the buffer.
    HANDLE file_mapping_handle = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        size_in_bytes, 
		NULL
    );

	// CreateFileMapping might fail

    // Map the section into the first placeholder region.
    void* view1 = MapViewOfFile3 (
        file_mapping_handle,
        NULL,
        placeholder1,
        0,
        size_in_bytes,
        MEM_REPLACE_PLACEHOLDER,
        PAGE_READWRITE,
        NULL, 
		0
    );

	// MapViewOfFile3 might fail

    // Ownership transferred, don’t free this on exit.
    placeholder1 = NULL;
	
    // Map the section into the second placeholder region.
    void* view2 = MapViewOfFile3 (
        file_mapping_handle,
        NULL,
        placeholder2,
        0,
        size_in_bytes,
        MEM_REPLACE_PLACEHOLDER,
        PAGE_READWRITE,
        NULL, 
		0
    );

	// MapViewOfFile3 might fail

	Ring ring;
	ring.data = view1;
	ring.tail = 0;
	ring.head = 0;

	return ring;
}
// void arena_free(Arena *arena) {
// 	VirtualFree(
// 		arena,
// 		0ULL,
// 		MEM_RELEASE
// 	);
// }
// void *arena_insert(Arena *arena, Arena_U64 size_in_bytes) {
// 	void *data = ((char*)arena) + sizeof(*arena) + arena->head;
// 	arena->head += size_in_bytes;
// 	if (sizeof(*arena) + arena->head <= arena->commited) return data;
// 	Arena_U64 commit_size = (((sizeof(*arena) + arena->head - arena->commited) + (4 << 10) - 1) / (4 << 10)) * (4 << 10);
// 	VirtualAlloc(
// 		((char*)arena) + arena->commited,
// 		commit_size,
// 		MEM_COMMIT,
// 		PAGE_READWRITE
// 	);
// 	return data;
// }