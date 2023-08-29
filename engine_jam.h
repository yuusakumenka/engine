#ifdef __INTELLISENSE__
#include "engine_types.h"
#include "engine_module_gpu.h"
#endif __INTELLISENSE__

//Gpu_Texture jam_texture(char* path);

typedef struct Jam {
    void*      cpu_buffer;
    void*      cpu_staging_buffer;
    Gpu_Buffer gpu_staging_buffer;
    Gpu_Buffer gpu_buffer;
    Gpu_Buffer coherent_buffer_size_in_bytes;
} Jam;
Jam* jam;

typedef struct Jam_Desc {
    U64      cpu_buffer_size_in_bytes;
    U64  staging_buffer_size_in_bytes;
    U64      gpu_buffer_size_in_bytes;
    U64 coherent_buffer_size_in_bytes;
} Jam_Desc;
void jam_init(Jam_Desc* desc) {
    jam = ARENA_INSERT_STRUCT(engine->app_arena, Jam);
    U64      cpu_buffer_size_in_bytes;
    U64  staging_buffer_size_in_bytes = MiB(512);
    U64      gpu_buffer_size_in_bytes;
    U64 coherent_buffer_size_in_bytes;
    jam->gpu_staging_buffer = gpu_make_buffer_coherent(
        staging_buffer_size_in_bytes,
        &jam->cpu_staging_buffer
    );
}

// implementation
Gpu_Texture jam_texture(char* path) {
    return asset_load_texture_to_gpu(
        engine->frame_arena,
        jam->cpu_staging_buffer,
        0,
        path
    );
}