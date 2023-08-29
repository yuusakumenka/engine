// This module is designed to :
//   * Provide a quick & dirty way to use some subset of gpu's graphic apis
//     in a platform agnostic way.
//   * Make it close enough to underlying graphics api's commonalities,
//     so to minimize friction when transitioning to production code.

// How to use?
//
//

// types
typedef unsigned char      Gpu_B8;
typedef unsigned char      Gpu_U8;
typedef unsigned int       Gpu_U32;
typedef unsigned long long Gpu_U64;
typedef float              Gpu_F32;

// const
#define GPU_NUM_VERTICES_MAX (1ULL << 16)

// handles
typedef struct Gpu_Uncompiled_Shader {
	const void *data;
	Gpu_U64     length;
} Gpu_Uncompiled_Shader;
typedef void* Gpu_Shader;
typedef void* Gpu_Pipeline_State;
typedef void* Gpu_Device;
typedef void* Gpu_Buffer;
typedef void* Gpu_Command_Allocator;
typedef void* Gpu_Command_List;
typedef void* Gpu_Command_Queue;
typedef void* Gpu_Buffer;
typedef void* Gpu_Texture;
typedef void* Gpu_Descriptor_Root;
typedef void* Gpu_Descriptor;

// typedef struct Gpu_Texture {
// 	Gpu_Buffer buffer;
// 	U64        offset;
// 	U64        descriptor;
// } Gpu_Texture;

// enums
typedef enum Gpu_Command_Type_ {
	GPU_COMMAND_TYPE_INVALID,
	GPU_COMMAND_TYPE_SET_PIPELINE,
	GPU_COMMAND_TYPE_COUNT
} Gpu_Command_Type_;
typedef unsigned char      Gpu_Clear_Flags;
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

#define GPU_FORMAT_TABLE(OP) \
	OP(GPU_FORMAT_UNKNOWN   , (0                  )), \
	OP(GPU_FORMAT_U32       , (    sizeof(Gpu_U32))), \
	OP(GPU_FORMAT_F32       , (    sizeof(Gpu_F32))), \
	OP(GPU_FORMAT_V2F32     , (2 * sizeof(Gpu_F32))), \
	OP(GPU_FORMAT_V4F32     , (4 * sizeof(Gpu_F32))), \
	OP(GPU_FORMAT_V4F32_RGBA, (4 * sizeof(Gpu_F32))), \
	OP(GPU_FORMAT_V4U8_RGBA , (4 * sizeof(Gpu_U8 ))), \
	OP(GPU_FORMAT_COUNT     , (0                  )),

typedef unsigned char Gpu_Format;
typedef enum Gpu_Format_ {
#define OP(identifier, size) identifier
	GPU_FORMAT_TABLE(OP)
#undef OP
} Gpu_Format_;
static Gpu_U8 GPU_FORMAT_SIZE[] = {
#define OP(identifier, size) ((Gpu_U8)(size))
	GPU_FORMAT_TABLE(OP)
#undef OP
};

typedef unsigned char Gpu_Fill_Mode;
typedef enum Gpu_Fill_Mode_ {
	GPU_FILL_MODE_SOLID,
	GPU_FILL_MODE_WIREFRAME
} Gpu_Fill_Mode_;

typedef unsigned char Gpu_Cull_Mode;
typedef enum Gpu_Cull_Mode_ {
	GPU_CULL_MODE_NONE  = 0x00,
	GPU_CULL_MODE_FRONT = 0x01,
	GPU_CULL_MODE_BACK  = 0x02,
	GPU_CULL_MODE_BOTH  = 0x03
} Gpu_Cull_Mode_;

typedef unsigned char Gpu_Depth_Function;
typedef enum Gpu_Depth_Function_ {
	GPU_GPU_DEPTH_FUNCTION_GREATER_EQUAL
} Gpu_Depth_Function_;

typedef unsigned char Gpu_Primitive_Topology;
typedef enum Gpu_Primitive_Topology_ {
	GPU_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	GPU_PRIMITIVE_TOPOLOGY_LINE_LIST,
	GPU_PRIMITIVE_TOPOLOGY_POINT_LIST
};

typedef struct Gpu_Buffer_Range {
	Gpu_Buffer buffer;
	Gpu_U64    offset;
	Gpu_U64    size;
} Gpu_Buffer_Range;

// descriptions
typedef struct Gpu_Input_Layout {
	// per vertex (& per instance) input data
	Gpu_U8     slot;
	Gpu_Format format;
	Gpu_U8     offset;
} Gpu_Input_Layout;

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

typedef Gpu_B8 Gpu_Blend;
typedef enum Gpu_Blend_{
	GPU_BLEND_ONE,
	GPU_BLEND_SRC_ALPHA,
	GPU_BLEND_ONE_MINUS_SRC_ALPHA
} Gpu_Blend_;

typedef struct Gpu_Output_Merger_State_Desc {
	// blend   state
	Gpu_B8    blend_enabled;
	Gpu_Blend blend_src;
	Gpu_Blend blend_dst;

	// depth   state
	Gpu_B8 disable_depth_test;
	Gpu_B8 disable_depth_write;
	// stencil state
} Gpu_Output_Merger_State_Desc;

// pipeline state
typedef struct Gpu_Pipeline_State_Desc {
	// input assembler / vertex input
	Gpu_U8            num_input_layouts;
	Gpu_Input_Layout* input_layouts;
	// vertex shader
	Gpu_Uncompiled_Shader vertex_shader;
	// rasterizer
	Gpu_Rasterizer_State_Desc* rasterizer_state;
	// fragment shader
	Gpu_Uncompiled_Shader fragment_shader;
	// output merger
	Gpu_Output_Merger_State_Desc output_merger_state;
} Gpu_Pipeline_State_Desc;

Gpu_Pipeline_State gpu_make_pipeline_state(
	Gpu_Device               device,
	Gpu_Pipeline_State_Desc *pipeline_state_desc
);

typedef void* Gpu_Descriptor_Root_Desc;

Gpu_Pipeline_State gpu_make_descriptor_root(
	Gpu_Device                device,
	Gpu_Descriptor_Root_Desc *descriptor_root_desc
);

//typedef unsigned int       Gpu_Command_Type;
//typedef struct Gpu_Command_Base {
//	Gpu_Command_Type type;
//} Gpu_Command_Base;
//typedef struct Gpu_Command_Set_Pipeline_State {
//	Gpu_Command_Base   base;
//	Gpu_Pipeline_State pipeline_state;
//} Gpu_Command_Use_Shader;
////typedef struct Gpu_Command_List {
////	//Arena            *arena; // command pool
////	Gpu_U64           count_of_command;
////	Gpu_Command_Base *first;
////} Gpu_Command_List;

// API:

// gpu_cmd_clear()
// gpu_cmd_draw_instanced()
// gpu_cmd_close()

Gpu_Command_Allocator gpu_make_command_allocator(Gpu_Device device);
Gpu_Command_List gpu_make_command_list(Gpu_Command_Allocator allocator);
void gpu_cmd_set_pipeline_state(
	Gpu_Command_List *command_list, 
	Gpu_Pipeline_State pipeline_state
);
void gpu_cmd_set_topology(
	Gpu_Command_List       *command_list, 
	Gpu_Primitive_Topology  primitive_topology
);
void gpu_cmd_clear(
	Gpu_Command_List *command_list,
	Gpu_Clear_Flags   flags,
	Gpu_F32           color_rgba[4],
	Gpu_F32           depth,
	Gpu_U8            stencil
);

void gpu_cmd_close(Gpu_Command_List *command_list);

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

void gpu_cmd_bind_uniform_buffer(
	Gpu_Command_List *command_list,
	Gpu_U8            slot, 
	Gpu_Buffer        buffer,
	Gpu_U64           size
);

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
	Gpu_Texture       descriptor,
	U32               slot
);

void gpu_cmd_bind_render_texture(
	Gpu_Command_List *command_list,
	Gpu_Texture       descriptor,
	U32               slot
);

void gpu_cmd_set_uniform_m3x2(
	Gpu_Command_List *command_list,
	M3x2             *matrix3x2,
	U32               slot
);

void gpu_cmd_copy_buffer(
	Gpu_Command_List *command_list,
	Gpu_Buffer src,
	Gpu_U64    src_offset,
	Gpu_Buffer dst,
	Gpu_U64    dst_offset,
	Gpu_U64    size
);

void gpu_cmd_set_render_texture(
	Gpu_Command_List *command_list,
	Gpu_Texture       render_texture
);

gpu_cmd_set_viewport(
	Gpu_Command_List *command_list, 
	F32 x, 
	F32 y, 
	F32 w, 
	F32 h
);

Gpu_Buffer gpu_make_buffer(Gpu_U64 size);
Gpu_Buffer gpu_make_buffer_coherent(Gpu_U64 size, void** data);
void*      gpu_buffer_map(Gpu_Buffer buffer, Gpu_U64 size);
void       gpu_buffer_unmap(Gpu_Buffer buffer);

void gpu_command_queue_execute_command_lists(
	Gpu_Command_Queue command_queue,
	Gpu_U32           count_of_command_list,
	Gpu_Command_List* command_lists
);

Gpu_Texture gpu_make_render_texture(U64 width, U64 height);