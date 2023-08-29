// Gpu_Texture asset_load_texture_to_gpu(
// 	      Arena *temporary_arena,
// 	const char  *path
// );

Gpu_Texture asset_load_texture_to_gpu(
    Arena      *temporary_arena, 
    Gpu_Buffer *staging_buffer,
    U64         offset,
    char       *path
);