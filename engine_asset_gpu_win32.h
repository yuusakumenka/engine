/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#define WUFFS_IMPLEMENTATION
#include "include\wuffs-v0.3.c"

// Gpu_Texture* asset_load_texture_to_gpu(
// 	      Arena *temporary_arena,
// 	const char  *path,
// ) {
//     Gpu_Texture *output      = 0;
//     Arena_Scope  arena_scope = arena_scope_begin(temporary_arena);
//     HANDLE file = CreateFileA(
// 		path,
//         GENERIC_READ,
//         FILE_SHARE_READ,
//         0,
//         OPEN_EXISTING,
//         0,
//         0
//     );
//     if (file == INVALID_HANDLE_VALUE) {
//         //debug_log("Couldn't find file \"%s\".\n", file_path);
//         goto CLEANUP_THEN_RETRUN;
//     }
//     LARGE_INTEGER file_size;
//     if (!GetFileSizeEx(file, &file_size)) {
//         //debug_log("Failed to get file size \"%s\".\n", file_path);
//         goto CLEANUP_THEN_RETRUN;
//     }
// 	void *data = arena_insert(dst_arena, file_size.QuadPart);
//     DWORD read_byte_count;
//     if (!ReadFile(
//         file,
// 		data,
//         file_size.QuadPart,
//         &read_byte_count,
//         0
//     )) {
//         //debug_log("Failed to read file \"%s\". [Windows Error Code: %d]\n", file_path, GetLastError());
//         goto CLEANUP_THEN_RETRUN;
//     }
// 	CloseHandle(file);
// 	if(size) *size = file_size.QuadPart;
// 	return data;
//     output = (Gpu_Texture*)data;

//     CLEANUP_THEN_RETRUN:;
//     arena_scope_end(arena_scope);
//     return output;
// }

Gpu_Texture asset_load_texture_to_gpu(
    // since the texture might need to get decompressed,
    // it will first be read from disk to a temporary buffer
    // then decompressed from it...
    Arena      *temporary_arena, 
    void       *cpu_staging_buffer,
    U64         offset,
    char       *path
) {
    Arena_Scope arena_scope = arena_scope_begin(temporary_arena);

    U64   raw_size;
    void *raw_data = asset_read(temporary_arena, path, &raw_size);

    wuffs_base__io_buffer src = wuffs_base__ptr_u8__reader(
        raw_data,
        raw_size,
        0
    );
    wuffs_png__decoder decoder;
    wuffs_png__decoder__initialize(
        &decoder, 
        sizeof(decoder), 
        WUFFS_VERSION, 
        0
    );
    wuffs_base__image_config image_config;
    wuffs_png__decoder__decode_image_config(&decoder, &image_config, &src);

    U32 size_x = wuffs_base__pixel_config__width(&image_config.pixcfg);
    U32 size_y = wuffs_base__pixel_config__height(&image_config.pixcfg);
    // wuffs_gif__decoder__initialize might fail

    U64 dst_size = size_x * size_y * sizeof(U32);
    //void* dst = arena_insert(
    //    temporary_arena, 
    //    dst_size
    //);
    void* dst = cpu_staging_buffer;

    wuffs_base__pixel_buffer pixel_buffer;
    wuffs_base__pixel_buffer__set_from_slice(
        &pixel_buffer,
        &image_config.pixcfg,
        wuffs_base__make_slice_u8(dst, dst_size)
    );
    
    // alloc work buffer
    U64 work_buffer_size = wuffs_png__decoder__workbuf_len(&decoder).max_incl;
    void* work_buffer = arena_insert(
        temporary_arena, 
        work_buffer_size
    );

    // read frame config
    wuffs_base__frame_config frame_config;
    for(;;) {
        wuffs_base__status status;
        status = wuffs_png__decoder__decode_frame_config(&decoder, &frame_config, &src);
        if (status.repr == 0) 
            break;
        if (status.repr != wuffs_base__suspension__short_read)
            break;
    }

    // read frame
    for(;;) {
        wuffs_base__status status;
        status = wuffs_png__decoder__decode_frame(
            &decoder, 
            &pixel_buffer, 
            &src,
            WUFFS_BASE__PIXEL_BLEND__SRC,
            wuffs_base__make_slice_u8(work_buffer, work_buffer_size),
            0
        );
        if (status.repr == 0) 
            break;
        if (status.repr == wuffs_base__note__end_of_data)
            break;
        if (status.repr != wuffs_base__suspension__short_read)
            break;
        if(!wuffs_base__status__is_ok(&status)) 
            break;
    }

    // transfer
    // gpu_cmd_copy_buffer(
    //     engine->gpu_command_list, // will need dedicated transfer cmd list

    // );

    GLuint texture_name;
    {
        glGenTextures(1, &texture_name);
        glBindTexture(GL_TEXTURE_2D, texture_name);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            size_x,
            size_y,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            dst
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    
    EXIT:;
    arena_scope_end(arena_scope);

    return texture_name;
}