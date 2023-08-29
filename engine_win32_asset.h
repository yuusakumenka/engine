void* asset_read(
	Arena              *dst_arena,
	const char         *path,
	unsigned long long *size
) {
    HANDLE file = CreateFileA(
		path,
        GENERIC_READ,
        FILE_SHARE_READ,
        0,
        OPEN_EXISTING,
        0,
        0
    );
    if (file == INVALID_HANDLE_VALUE) {
        //debug_log("Couldn't find file \"%s\".\n", file_path);
        return 0;
    }
    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(file, &file_size)) {
        //debug_log("Failed to get file size \"%s\".\n", file_path);
        return 0;
    }
	void *data = arena_insert(dst_arena, file_size.QuadPart);
    DWORD read_byte_count;
    if (!ReadFile(
        file,
		data,
        file_size.QuadPart,
        &read_byte_count,
        0
    )) {
        //debug_log("Failed to read file \"%s\". [Windows Error Code: %d]\n", file_path, GetLastError());
        return 0;
    }
	CloseHandle(file);
	if(size) *size = file_size.QuadPart;
	return data;
}