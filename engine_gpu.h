/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* The only backend presently supported is OpenGL. */

// This module is designed to :
//   * Provide a minimal boilerplate api to use some subset of gpu's graphic apis in a vendor agnostic way.
//   * Make it close enough to underlying graphics api's commonalities,
//     so to minimize friction when transitioning to production code.

// types
typedef unsigned char      Gpu_B8;
typedef unsigned char      Gpu_U8;
typedef unsigned int       Gpu_U32;
typedef unsigned long long Gpu_U64;
typedef float              Gpu_F32;

// const
#define GPU_NUM_VERTICES_MAX (1ULL << 16)

/************************************************
RESSOURCES 
************************************************/

/* Opaque Vendor Specific Types / Handles */
typedef void Gpu_Command_List;
typedef void Gpu_Texture;

// handles
typedef void* Gpu_Shader;
typedef void* Gpu_Pipeline_State;
//typedef void* Gpu_Device;
typedef struct Gpu_Device    Gpu_Device;
typedef void* Gpu_Buffer;
typedef void* Gpu_Command_Allocator;
typedef void* Gpu_Command_Queue;
typedef void* Gpu_Buffer;

/* In practice, a Gpu_Texture is an index into a large SRV array
 * reffrenced by some root argument located at a constant and know index in the root signature. */
typedef void *Gpu_Descriptor_Root;
typedef void *Gpu_Descriptor;

typedef struct Gpu_Uncompiled_Shader { /* @DEPRECATED */
    const void *data;
    size_t      length;
} Gpu_Uncompiled_Shader;

/* Format */
#define GPU_FORMAT_TABLE(ENTRY) \
    /*   | Engine                                      | OpenGl Backend      |*/\
    /*   | Gpu_Format           | GPU_FORMAT_SIZE      | n | GLenum          |*/\
    ENTRY( GPU_FORMAT_UNKNOWN   , (0                  ), 0 , 0               ), \
    ENTRY( GPU_FORMAT_I32       , (    sizeof(Gpu_U32)), 1 , GL_INT          ), \
    ENTRY( GPU_FORMAT_U32       , (    sizeof(Gpu_U32)), 1 , GL_UNSIGNED_INT ), \
    ENTRY( GPU_FORMAT_F32       , (    sizeof(Gpu_F32)), 1 , GL_FLOAT        ), \
    ENTRY( GPU_FORMAT_V2F32     , (2 * sizeof(Gpu_F32)), 2 , GL_FLOAT        ), \
    ENTRY( GPU_FORMAT_V3F32     , (3 * sizeof(Gpu_F32)), 3 , GL_FLOAT        ), \
    ENTRY( GPU_FORMAT_V4F32     , (4 * sizeof(Gpu_F32)), 4 , GL_FLOAT        ), \
    ENTRY( GPU_FORMAT_V4F32_RGBA, (4 * sizeof(Gpu_F32)), 4 , GL_FLOAT        ), \
    ENTRY( GPU_FORMAT_V4C32     , (4 * sizeof(C32    )), 4 , GL_UNSIGNED_INT ), \
    ENTRY( GPU_FORMAT_V4I32     , (4 * sizeof(I32    )), 4 , GL_INT          ), \
    ENTRY( GPU_FORMAT_V4U8_RGBA , (4 * sizeof(Gpu_U8 )), 1 , GL_UNSIGNED_INT ), \
    ENTRY( GPU_FORMAT_COUNT     , (0                  ), 0 , 0               ), \
/* endof GPU_FORMAT_TABLE */
typedef unsigned char Gpu_Format;
typedef enum Gpu_Format_ {
    #define OP(identifier, _0, _1, _2) identifier
        GPU_FORMAT_TABLE(OP)
    #undef OP
} Gpu_Format_;
static Gpu_U8 GPU_FORMAT_SIZE[] = {
    #define OP(_0, size, _1, _2) ((Gpu_U8)(size))
        GPU_FORMAT_TABLE(OP)
    #undef OP
};

/* Texture Format */
#define GPU_TEXTURE_FORMAT_TABLE(ENTRY) \
    /*   | Engine               | OpenGl Backend                         |*/\
    /*   | Gpu_Texture_Format_  | internaFormat, format, type            |*/\
    ENTRY( GPU_TEXTURE_FORMAT_R8, GL_R8        , GL_RED, GL_UNSIGNED_BYTE), \
/* endof GPU_TEXTURE_FORMAT_TABLE */
typedef unsigned char Gpu_Texture_Format;
typedef enum Gpu_Texture_Format_ {
    #define OP(identifier, _0, _1, _2) identifier
        GPU_TEXTURE_FORMAT_TABLE(OP)
    #undef OP
    GPU_TEXTURE_FORMAT_COUNT
} Gpu_Texture_Format_;

typedef unsigned char Gpu_Texture_Filtering;
typedef enum Gpu_Texture_Filtering_ {
    GPU_TEXTURE_FILTERING_NEAREST,
    GPU_TEXTURE_FILTERING_LINEAR
} Gpu_Texture_Filtering_;

/* Depth */
typedef unsigned char Gpu_Depth_Function;
typedef enum Gpu_Depth_Function_ {
    GPU_GPU_DEPTH_FUNCTION_GREATER_EQUAL
} Gpu_Depth_Function_;

/* Primitives */
#define GPU_PRIMITIVE_TOPOLOGY_TABLE(ENTRY)\
    ENTRY(GPU_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST , GL_TRIANGLES	  )\
    ENTRY(GPU_PRIMITIVE_TOPOLOGY_LINE_LIST     , GL_LINES         )\
    ENTRY(GPU_PRIMITIVE_TOPOLOGY_POINT_LIST    , GL_POINTS        )\
    ENTRY(GPU_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, GL_TRIANGLE_STRIP)\
/* endof GPU_PRIMITIVE_TOPOLOGY_TABLE */
typedef unsigned char Gpu_Primitive_Topology;
typedef enum Gpu_Primitive_Topology_ {
    #define OP(identifier, _) identifier,
        GPU_PRIMITIVE_TOPOLOGY_TABLE(OP)
    #undef OpenGl
    GPU_PRIMITIVE_TOPOLOGY_COUNT
} Gpu_Primitive_Topology_;

/* Descriptor */
typedef struct Gpu_Buffer_Range {
    Gpu_Buffer buffer;
    Gpu_U64    offset;
    Gpu_U64    size;
} Gpu_Buffer_Range;

/* Vertex Attribute */
typedef struct Gpu_Vertex_Attribute {
    // per vertex (& per instance) input data
    Gpu_U8     slot;
    Gpu_Format format;
    Gpu_U8     offset;
    Gpu_U8     buffer_slot;
    B8         is_per_instance : 1;
    B8         unused6         : 1;
    B8         unused5         : 1;
    B8         unused4         : 1;
    B8         unused3         : 1;
    B8         unused2         : 1;
    B8         unused1         : 1;
    B8         unused0         : 1;
} Gpu_Vertex_Attribute;

/* Fill Mode */
typedef unsigned char Gpu_Fill_Mode;
typedef enum Gpu_Fill_Mode_ {
    GPU_FILL_MODE_SOLID,
    GPU_FILL_MODE_WIREFRAME
} Gpu_Fill_Mode_;

/* Cull Mode */

typedef unsigned char Gpu_Cull_Mode;
typedef enum Gpu_Cull_Mode_ {
    GPU_CULL_MODE_NONE  = 0x00,
    GPU_CULL_MODE_FRONT = 0x01,
    GPU_CULL_MODE_BACK  = 0x02,
    GPU_CULL_MODE_BOTH  = 0x03
} Gpu_Cull_Mode_;

/* Rasterizer */
typedef struct Gpu_Rasterizer_State_Desc {
    Gpu_Fill_Mode fill_mode;
    Gpu_Cull_Mode cull_mode;
    Gpu_B8        winding_counter_vs_clockwise;
    // add support for conservative rasterization?
    // add support for depth bias?
    // add support for forcing sample count?
    Gpu_B8        disable_depth_clip;
    // add support for scissors?
    // add support for multisampling?
    // add support for line anti-aliasing?
} Gpu_Rasterizer_State_Desc;

/* Blending Mode */
typedef Gpu_B8 Gpu_Blend;
typedef enum Gpu_Blend_{
    GPU_BLEND_ONE,
    GPU_BLEND_SRC_ALPHA,
    GPU_BLEND_ONE_MINUS_SRC_ALPHA
} Gpu_Blend_;

/* Output Merger */
typedef struct Gpu_Output_Merger_State_Desc {
    /* Blend */
    Gpu_Blend blend_src     : 4;
    Gpu_Blend blend_dst     : 4;
    Gpu_B8    blend_enabled : 1;

    /* Depth */
    Gpu_B8 depth_test_enabled   : 1;
    Gpu_B8 depth_write_disabled : 1;

    // stencil state
} Gpu_Output_Merger_State_Desc;

/* Pipeline State */
typedef struct Gpu_Pipeline_State_Desc {
    size_t                       num_vertex_attributes;
    Gpu_Vertex_Attribute*        vertex_attributes;
    Gpu_Uncompiled_Shader        vertex_shader; /* @DEPRECATED */
    size_t                       vertex_shader_num_bytes;
    char                        *vertex_shader_bytes;
    Gpu_Rasterizer_State_Desc   *rasterizer_state;
    Gpu_Uncompiled_Shader        fragment_shader; /* @DEPRECATED */
    size_t                       fragment_shader_num_bytes;
    char                        *fragment_shader_bytes;
    Gpu_Output_Merger_State_Desc output_merger_state;
} Gpu_Pipeline_State_Desc;
Gpu_Pipeline_State gpu_make_pipeline_state(
    Gpu_Device              *device,
    Gpu_Pipeline_State_Desc *pipeline_state_desc
);

typedef void* Gpu_Descriptor_Root_Desc;

Gpu_Pipeline_State gpu_make_descriptor_root(
    Gpu_Device               *device,
    Gpu_Descriptor_Root_Desc *descriptor_root_desc
);

Gpu_Command_Allocator gpu_make_command_allocator(Gpu_Device *device);
Gpu_Command_List *gpu_make_command_list(Gpu_Command_Allocator allocator);



/* @FIXME(menyuu) Alignment may be runtime specific. */
/* Since we gonna bake asset with an alignment, might just as well find
* the biggest alignment on all target platforms. */
#define ALIGNMENT 256
size_t     gpu_align_constant_buffer_offset(size_t offset) { return (offset + (ALIGNMENT - 1)) & (~(ALIGNMENT - 1)); }
#undef ALIGNMENT
Gpu_Buffer gpu_make_buffer(Gpu_U64 size);
Gpu_Buffer gpu_make_buffer_coherent(Gpu_U64 size, void** data);
void*      gpu_buffer_map(Gpu_Buffer buffer, Gpu_U64 size);
void       gpu_buffer_unmap(Gpu_Buffer buffer);

void gpu_command_queue_execute_command_lists(
    Gpu_Command_Queue command_queue,
    Gpu_U32           count_of_command_list,
    Gpu_Command_List* command_lists
);

Gpu_Texture *gpu_make_render_texture(U64 width, U64 height);

typedef struct Gpu_Texture_Desc {
    Gpu_Format format;
    size_t     num_dimentions : 2;
    size_t     channel_rooting;
} Gpu_Texture_Desc;

Gpu_Texture *gpu_create_texture();

Gpu_Texture *gpu_create_texture_from_memory_blocking(
    int                width,
    int                height,
    Gpu_Texture_Format format,
    char              *data
);

/************************************************
 COMMANDS 
 ************************************************/

/* Pipeline */
void gpu_cmd_set_pipeline_state(
    Gpu_Command_List *command_list, 
    Gpu_Pipeline_State pipeline_state
);
void gpu_cmd_set_topology(
    Gpu_Command_List       *command_list, 
    Gpu_Primitive_Topology  primitive_topology
);

/* Clear */
typedef unsigned char Gpu_Clear_Flags;
typedef enum Gpu_Clear_Flags_ {
    GPU_CLEAR_FLAGS_NONE    = 0x00,
    GPU_CLEAR_FLAGS_COLOR   = 0x01,
    GPU_CLEAR_FLAGS_DEPTH   = 0x02,
    GPU_CLEAR_FLAGS_STENCIL = 0x04,
    GPU_CLEAR_FLAGS_ALL = 
    GPU_CLEAR_FLAGS_COLOR |
    GPU_CLEAR_FLAGS_DEPTH |
    GPU_CLEAR_FLAGS_STENCIL,
} Gpu_Clear_Flag_;
void gpu_cmd_clear(
    Gpu_Command_List *command_list,
    Gpu_Clear_Flags   flags,
    Gpu_F32           color_rgba[4],
    Gpu_F32           depth,
    Gpu_U8            stencil
);

void gpu_cmd_close(Gpu_Command_List *command_list);

/* Bindings */
void gpu_cmd_bind_vertex_buffer_range(
    Gpu_Command_List *command_list,
    Gpu_U8            slot, 
    Gpu_Buffer        buffer,
    Gpu_U64           offset,
    Gpu_U64           size,
    Gpu_U64           stride
);
void gpu_cmd_bind_index_buffer_range(
    Gpu_Command_List *command_list,
    Gpu_U8            slot, 
    Gpu_Buffer        buffer,
    Gpu_U64           offset,
    Gpu_U64           size,
    Gpu_U64           stride
);
void gpu_cmd_bind_constant_buffer_range(
    Gpu_Command_List *command_list,
    Gpu_U8            slot, 
    Gpu_Buffer        buffer,
    Gpu_U64           offset,
    Gpu_U64           size
);
void gpu_cmd_bind_uniform_buffer( /* @DEPRECATED */
    Gpu_Command_List *command_list,
    Gpu_U8            slot, 
    Gpu_Buffer        buffer,
    Gpu_U64           size
);


// DescriptorSet == RootSignature
// Descriptor    == RootDescriptorTable == uniform

void gpu_cmd_set_descriptor_root(
    Gpu_Command_List *command_list,
    Gpu_Descriptor_Root descriptor_root
);

void gpu_cmd_bind_descriptor(
    Gpu_Command_List *command_list,
    Gpu_Descriptor    descriptor,
    U32               slot
);

void gpu_cmd_bind_texture(
    Gpu_Command_List *command_list,
    Gpu_Texture      *descriptor,
    U32               slot
);

void gpu_cmd_bind_render_texture(
    Gpu_Command_List *command_list,
    Gpu_Texture      *descriptor,
    U32               slot
);

/* Uniforms */
void gpu_cmd_set_uniform_i32(
	Gpu_Command_List *command_list,
	I32               i32,
	U32               slot
);
void gpu_cmd_set_uniform_v2(
	Gpu_Command_List *command_list,
	V2                vector2d,
	U32               slot
);
void gpu_cmd_load_uniform_v2(
    Gpu_Command_List *command_list,
    size_t            slot,
    float             xy[2]
);
void gpu_cmd_set_uniform_m3x2(
    Gpu_Command_List *command_list,
    M3x2             *matrix3x2,
    U32               slot
);
void gpu_cmd_set_uniform_m4(
    Gpu_Command_List *command_list,
    M4               *matrix4x4,
    U32               slot
);
void gpu_cmd_load_uniform_m4(
    Gpu_Command_List *command_list,
    size_t            slot,
    float             matrix4x4[16]
);
void gpu_cmd_set_render_texture(
    Gpu_Command_List *command_list,
    Gpu_Texture      *render_texture
);

/* Viewport & Scisors */
void gpu_cmd_set_viewport(
    Gpu_Command_List *command_list, 
    float             x, 
    float             y, 
    float             w, 
    float             h
);
void gpu_cmd_load_viewport(
    Gpu_Command_List *command_list, 
    float             xywh[4] 
);

Gpu_Texture *gpu_srv_pool_remove_one();
Gpu_Texture *gpu_srv_pool_insert_one(Gpu_Texture *srv);

/* Draw Calls */
void gpu_cmd_draw_instanced(
    Gpu_Command_List *command_list,
    Gpu_U32 count_of_vertex_per_instance,
    Gpu_U32 count_of_instance,
    Gpu_U32 start_vertex_location, // offset
    Gpu_U32 start_instance_location
);
void gpu_cmd_draw_indexed_instanced(
    Gpu_Command_List *command_list,
    Gpu_U32 count_of_index_per_instance,
    Gpu_U32 count_of_instance,
    Gpu_U32 start_index_location, // offset
    Gpu_U32 start_instance_location
);

/* Transfers */
void gpu_cmd_copy_buffer(
    Gpu_Command_List *command_list,
    Gpu_Buffer        dst,
    ptrdiff_t         dst_offset,
    size_t            num_bytes,
    Gpu_Buffer        src,
    ptrdiff_t         src_offset
);

/* Staging */
void *gpu_stage(Gpu_Buffer destination, size_t *num_bytes) {

}

typedef struct Gpu_Buffer_Map_Description {
    /* is_single_use / recycle_after_use / discard_after_use / keep_after_use */
    /* preference */
    /* write_directly_to_vram */
    /* */

    int dummy;
} Gpu_Buffer_Map_Description;

void* gpu_buffer_map_begin(Gpu_Buffer destination, size_t offset, size_t num_bytes);
void  gpu_buffer_map_end(Gpu_Buffer destination, size_t offset, size_t num_bytes, void *bytes, size_t *status);
//void  gpu_buffer_upload(Gpu_Buffer destination, size_t offset, size_t num_bytes, void *bytes);

/* buffer may be directly mappable, */

/* 

    size_t num_remaining_bytes = 128;
    for(;;) {
        size_t num_staging_bytes = num_remaining_bytes;
        void *data = gpu_stage(Gpu_Buffer destination, &num_remaining_bytes);
        if (!num_remaining_bytes)
            break;
        num_remaining_bytes -= num_staging_bytes;
    }

*/

/* Swapchain */

typedef struct Gpu_Swapchain_Descripion {
    Os_Surface *surface;
    /* vsync */
    size_t width;
    size_t height;
} Gpu_Swapchain_Descripion;

typedef struct Gpu_Swapchain Gpu_Swapchain;

Gpu_Swapchain *gpu_swapchain_create(
    Gpu_Device               *device,
    Gpu_Swapchain_Descripion *description
);

/* swapchain MUST have been created with a non null surface. */
void gpu_swapchain_present(Gpu_Swapchain *swapchain);

/* Present */
void gpu_present(Os_Surface *surface);