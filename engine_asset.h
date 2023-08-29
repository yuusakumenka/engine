typedef void* Asset;
typedef void* Asset_Id;

typedef struct Asset_Info {
	Asset_Id id;
	void*    data;
} Asset_Info;

void *asset_read(
	Arena              *dst_arena,
	const char         *path,
	unsigned long long *size
);