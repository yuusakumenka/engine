/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

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