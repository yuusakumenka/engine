/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#ifdef __INTELLISENSE__
#include "engine_types.h"
#include "engine_module_gpu.h"
#endif __INTELLISENSE__

//Gpu_Texture jam_texture(char* path);
typedef struct Jam_Vertex {
    V3  position; float unused0;
} Jam_Vertex;

typedef struct Jam_Mesh Jam_Mesh;
typedef struct Jam_Mesh {
    Jam_Mesh *next;
    U32 flags;
    S8 path;
    Gpu_Buffer buffer;
    Gpu_Buffer_Range vertex_buffer;
    Gpu_Buffer_Range index_buffer;
    Gpu_Buffer_Range static_constant_buffer;
} Jam_Mesh;

typedef struct Jam {
    void*      cpu_buffer;
    void*      cpu_staging_buffer;
    Gpu_Buffer gpu_staging_buffer;
    Gpu_Buffer gpu_buffer;
    Gpu_Buffer coherent_buffer_size_in_bytes;

    Jam_Mesh *meshes_first;
    Jam_Mesh *meshes_last;

    /* Scene & Graphics */
    Bool render_grid : 1;
} Jam;
Jam* jam;

typedef struct Jam_Desc {
    U64      cpu_buffer_size_in_bytes;
    U64  staging_buffer_size_in_bytes;
    U64      gpu_buffer_size_in_bytes;
    U64 coherent_buffer_size_in_bytes;
} Jam_Desc;

typedef struct Gpu_Create_Coherent_Buffer_Command {
    Gpu_U64      num_bytes;
    void       **mapped_data;
    Gpu_Buffer **gpu_buffer;
} Gpu_Create_Coherent_Buffer_Command;

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

    /* gpu_create_coherent_buffer_command */
}

void jam_frame_begin() {
    jam->render_grid = 1;
}

void jam_frame_end() {
    
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

typedef struct Jam_Load_Mesh_Job {
    Jam_Mesh *mesh;
    char *path;                                  
} Jam_Load_Mesh_Job;

void jam_load_mesh_job(Jam_Load_Mesh_Job *job) {
    U64   size;
    void *data = asset_read(engine->frame_arena, job->path, &size); /* move to stack */
    ASSERT(data, "Failed to load %s.", job->path);
    
    U64 work_buffer_size  = collada_deserializer_num_work_buffer_bytes(8, 8, 1, 1, 1 + 256);
    void *work_buffer = arena_insert(engine->frame_arena, work_buffer_size);
    Collada_Deserializer _deserializer;
    _deserializer.work_buffer      = work_buffer;
    _deserializer.work_buffer_head = work_buffer;
    _deserializer.work_buffer_size = work_buffer_size;

    
    M4 transform_3d_software_to_engine = m4(
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 1
    );
    
    Collada_Deserializer_Mesh _mesh = {0};
    Collada_Deserializer      *deserializer = &_deserializer;
    Collada_Deserializer_Mesh *mesh         = &_mesh;
    Collada_Deserializer_Error error        = collada_deserializer_mesh(
        deserializer, 
        size, 
        data, 
        transform_3d_software_to_engine,
        m4(
            -1,  0,  0, 0,
             0,  0, -1, 0,
             0, -1,  0, 0,
             0,  0,  0, 1
        ),
        mesh
    );
    ASSERT(!error, "avatar_renderer", "Failed to parse avatar model: %s.", COLLADA_READER_ERROR_STRINGS[error]);
    
    U16 num_vertices = collada_deserializer_mesh_get_num_vertices(mesh);
    U64 num_indices  = collada_deserializer_mesh_get_num_triangles(mesh) * 3;
    U64 num_joints   = collada_deserializer_mesh_get_num_joints(mesh);

    size_t          vertex_buffer_offset = 0;
    size_t          vertex_buffer_size   = sizeof(Jam_Vertex) * num_vertices;
    size_t           index_buffer_offset = vertex_buffer_size;
    size_t           index_buffer_size   = sizeof(U16) * num_indices;
    size_t static_constant_buffer_offset = gpu_align_constant_buffer_offset(vertex_buffer_size + index_buffer_size);
    size_t static_constant_buffer_size   = sizeof(M4) * num_joints;
    
    Uint mesh_buffer_size = 
        static_constant_buffer_offset +
        static_constant_buffer_size;   
        
    char *buffer;
    Gpu_Buffer gpu_buffer = gpu_make_buffer_coherent(mesh_buffer_size, &buffer);
    
    job->mesh->buffer = gpu_buffer;
    
    job->mesh->         vertex_buffer.buffer = gpu_buffer;
    job->mesh->         vertex_buffer.size   = vertex_buffer_size;
    job->mesh->         vertex_buffer.offset = vertex_buffer_offset;
    job->mesh->          index_buffer.buffer = gpu_buffer;
    job->mesh->          index_buffer.size   = index_buffer_size;
    job->mesh->          index_buffer.offset = index_buffer_offset;
    job->mesh->static_constant_buffer.buffer = gpu_buffer;
    job->mesh->static_constant_buffer.size   = static_constant_buffer_size;
    job->mesh->static_constant_buffer.offset = static_constant_buffer_offset;

    
    Jam_Vertex *         vertex_buffer = buffer + vertex_buffer_offset;
    U16        *          index_buffer = buffer + index_buffer_offset;
    M4         *static_constant_buffer = buffer + static_constant_buffer_offset;

    F32 *position_float_array;
    U64 num_float = collada_deserializer_mesh_get_num_vertex_position_floats(mesh);
    ASSERT(
        num_float,
        "Missing vertex positions.", "No vertex positions in your model.\nDid you select it before clicking export to Collada?"
    );
    position_float_array = (F32*)alloca(num_float * sizeof(F32));
    collada_deserializer_mesh_parse_vertex_positions_float_array(mesh, position_float_array);

    U16 *trianges_index_array;
    U64 num_triangle_indices = collada_deserializer_mesh_get_num_triangles_indices(mesh);
    trianges_index_array = (U16*)alloca(num_triangle_indices * sizeof(U16));
    collada_deserializer_mesh_parse_triangle_index_array(mesh, trianges_index_array);

    collada_deserializer_mesh_vertex_positions_v3(
        mesh,
        position_float_array,
        trianges_index_array,
        transform_3d_software_to_engine,
        &vertex_buffer->position,
        sizeof(Jam_Vertex)
    );

    U16 *vcount_array;
    U64 num_vcount = collada_deserializer_mesh_get_num_vertex_weights_vcount(mesh);
    vcount_array = (U16*)alloca(num_vcount * sizeof(U16));
    collada_deserializer_mesh_parse_vertex_weights_vcount_array(mesh, vcount_array);

    U16 *summed_vcount_array = (U16*)alloca(num_vcount * sizeof(U16));
    collada_deserializer_mesh_sum_vertex_weights_vcount_array(mesh, vcount_array, summed_vcount_array);

    U16 *v_array;
    U64 num_vs = collada_deserializer_mesh_get_num_vertex_weights_vs(mesh);
    v_array = (U16*)alloca(num_vs * sizeof(U16));
    collada_deserializer_mesh_parse_vertex_weights_v_array(mesh, v_array);

    F32 *weight_float_array;
    U64 num_weight_float = collada_deserializer_mesh_get_num_vertex_weights_floats(mesh);
    weight_float_array = (F32*)alloca(num_weight_float * sizeof(F32));
    collada_deserializer_mesh_parse_vertex_weights_float_array(mesh, weight_float_array);

    collada_deserializer_mesh_triangle_indices(
        mesh,
        index_buffer
    );
    
    job->mesh->flags |= 0x1; /* Done */
}

void jam_mesh(char *path) {
    S8 path_s8 = s8_cstr(path);
    Jam_Mesh *mesh = 0;
    /* try find mesh in cache */
    for(Jam_Mesh *maybe_mesh = jam->meshes_first; maybe_mesh; maybe_mesh->next) {
        if(s8_equals(path_s8, maybe_mesh->path)) {
            mesh = maybe_mesh;
            break;
        }
    }
    if(!mesh) {
        /* load mesh */
        mesh = ARENA_INSERT_STRUCT(engine->app_arena, Jam_Mesh);
        if(jam->meshes_first) {
            jam->meshes_last->next = mesh;
        } else {
            jam->meshes_first = mesh;
        }
        jam->meshes_last = mesh;
        mesh->path = path_s8;
        
        Jam_Load_Mesh_Job job = {0};
        job.mesh = mesh;
        job.path = path;
        jam_load_mesh_job(&job);        
    }
    /* draw mesh*/
    if(mesh->flags & 1) { /* Rdy */

    }
}

void jam_flycam() {

}

void jam_3d_grid() {
    jam->render_grid = 1;
}

void jam_button(char button) {
    
}

void jam_update(void *data) {

    engine->update();

    job_ring64_insert_one(&engine->job_ring, data);

    //job_end(&engine->job_ring, (Job*)data);
}

void _jam_startup (Engine_Startup_Desc *startup_desc) {
    
    // Job *jobs = ARENA_INSERT_ARRAY(engine->app_arena, Job, 1);
    
    // jobs[0].proc          = jam_update;
    // jobs[0].data          = jobs;
    // jobs[0].num_dependee  = 0;
    // jobs[0].num_dependers = 0;
    // jobs[0].dependers     = 0;

    // job_ring_insert_one(&engine->job_ring, jobs);

}