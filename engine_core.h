// engine.h   | Engine for easy app development
// |- arena.h | Memory arena allocator

// engine
// |
// |- windowing
// |  |- create window
// |
// |- graphics
// |  |- gizmos
// |  |- gpu
// |
// |- audio
// |

#ifndef ENGINE_Arena
#include "engine_arena.h"
#define ENGINE_Arena        Arena
#define ENGINE_arena_alloc  arena_alloc
#define ENGINE_arena_clear  arena_clear
#define ENGINE_arena_insert arena_insert
#endif // ENGINE_Arena


typedef int Engine_Graphics_Buffering;
typedef enum Engine_Graphics_Buffering_ {
    ENGINE_GRAPHICS_BUFFERING_SINGLE, 
    ENGINE_GRAPHICS_BUFFERING_DOUBLE, 
    ENGINE_GRAPHICS_BUFFERING_TRIPLE
} Engine_Graphics_Buffering_;

typedef int Engine_Windowing_Flags;
typedef enum Engine_Windowing_Flags_ {
	//ENGINE_WINDOWING_LOST_FOCUS 
	ENGINE_WINDOWING_FLAG_FOCUS = 0x01, 
	ENGINE_WINDOWING_FLAG_HOVER = 0x02,
} Engine_Windowing_Flags_;


#define ENGINE_U16 unsigned short
#define ENGINE_U32 unsigned int
#define ENGINE_U64 unsigned long long
#define ENGINE_F32 float

#ifndef ENGINE_V2F32
typedef struct Engine_V2F32 {
    ENGINE_F32 x;
    ENGINE_F32 y;
} Engine_V2F32;
#define ENGINE_V2F32 Engine_V2F32
#endif // ENGINE_V2F32

#ifndef ENGINE_String16
typedef struct Engine_String16 {
    ENGINE_U16 *data;
    ENGINE_U64  length;
} Engine_String16;
#define ENGINE_String16 Engine_String16
#define ENGINE_S16LIT(string_literal) (ENGINE_String16){ \
	(ENGINE_U16*)(L##string_literal), \
	((sizeof(L##string_literal) / sizeof(ENGINE_U16)) - 1) \
}
#endif // ENGINE_String16s

typedef struct Engine {
    // memory
    Arena* frame_arena;
    Arena*   app_arena;

    // windowing & rendering
    ENGINE_String16           window_title;
	ENGINE_V2F32              window_size_in_px;
	Engine_Windowing_Flags    window_flags;
    Engine_Graphics_Buffering window_buffering;

	// gpu
	Gpu_Device            gpu_device;
	Gpu_Command_Allocator gpu_command_allocator;
	Gpu_Command_List      gpu_command_list;
	Gpu_Command_Queue     gpu_command_queue;

    // mouse cursor
    ENGINE_U32 mouse_cursor;
    struct Mouse {
        ENGINE_U32   cursor;
        ENGINE_V2F32 position;
    } mouse;
    // the app's icon
    ENGINE_U32 icon;

	// audio
	int audio_latency_in_audio_frames;

    // timing
	ENGINE_F32 delta_time_in_seconds;

    // inputs
    V2 mouse_wheel;
    V2 mouse_delta;
    V2 mouse_position;
    V2 mouse_position_at_last_frame;

    //              button: 31 ... 10 9 8  7 6 5 4  3 2 1 0
    //  current frame mask: 01 ... 010101 01010101 01010101
    // previous frame mask: 10 ... 101010 10101010 10101010
    //
    //   current frame state --------------------> is_pressed     (button)
    //   v  
    //  00 -> user left the button untouched ----> got_untouched  (button) ?
    //  01 -> user just pressed the button ------> got_pressed    (button)
    //  10 -> user just released the button -----> got_released   (button)
    //  11 -> user is holding the button down ---> got_maintained (button) ?
    //  ^
    //  previous frame state --------------------> was_pressed    (button)
    //
    U64 buttons[256 / ((sizeof(U64) * 8) / 2)];

	// camera
	Camera      camera;

	// draw
	Draw_Context* draw_context;
} Engine;

void engine_pre_init(Engine* engine) {
	// memory
	{
		engine->frame_arena = arena_alloc();
		engine->app_arena   = arena_alloc();
	}

    // windowing
    {
        // size defaults to 720p
        engine->window_size_in_px.x = 1280.0f;
        engine->window_size_in_px.y =  720.0f;
        // title
        engine->window_title = ENGINE_S16LIT("利用者");
        // default to triple buffering
        engine->window_buffering = ENGINE_GRAPHICS_BUFFERING_TRIPLE;
    }

	// audio
	{
		engine->audio_latency_in_audio_frames = (44100 / 60);
	}

	// gpu
	{
		engine->gpu_command_allocator = 
			gpu_make_command_allocator(engine->gpu_device);
		engine->gpu_command_list = 
			gpu_make_command_list(engine->gpu_command_allocator);
	}

	// init draw module
	{
		Draw_Context* context = ARENA_INSERT_STRUCT(engine->app_arena, Draw_Context);
		
		{
			Gpu_Input_Layout       input_layouts[2];
			{
				// position
				input_layouts[0].slot   = 0;
				input_layouts[0].format = GPU_FORMAT_V2F32;
				input_layouts[0].offset = 0;
				// color
				input_layouts[1].slot   = 1;
				input_layouts[1].format = GPU_FORMAT_V4U8_RGBA;
				input_layouts[1].offset = 
					input_layouts[0].offset + 
					GPU_FORMAT_SIZE[input_layouts[0].format];
			}

			Gpu_Pipeline_State_Desc gpu_pipeline_state_desc = { 0 };
			gpu_pipeline_state_desc.input_layouts     = input_layouts;
			gpu_pipeline_state_desc.num_input_layouts = 
				sizeof(input_layouts) / sizeof(input_layouts[0]);
			
			{
				Arena_Scope scope = arena_scope_begin(engine->app_arena);
				gpu_pipeline_state_desc.vertex_shader.data = 
					asset_read(
					scope.arena, 
					"shaders/draw.vert", 
					&gpu_pipeline_state_desc.vertex_shader.length
				);
				gpu_pipeline_state_desc.fragment_shader.data = 
					asset_read(
					scope.arena, 
					"shaders/draw.frag", 
					&gpu_pipeline_state_desc.fragment_shader.length
				);
				context->gpu_pipeline_state = 
					gpu_make_pipeline_state(
					engine->gpu_device, 
					&gpu_pipeline_state_desc
				);
				arena_scope_end(scope);
			}
		}

		// set to use default gpu components
		context->gpu_command_list  = engine->gpu_command_list;
		context->gpu_command_queue = engine->gpu_command_queue;

		// config
		context->num_vertices_max = (1ULL << (sizeof(Draw_Index) * 8));
		context->num_indices_max  = context->num_vertices_max;
		
		// alloc global coherent (gpu+cpu) buffer
		void* cpu_buffer;
		context->gpu_buffer = gpu_make_buffer_coherent(
			sizeof(Draw_Vertex) * context->num_vertices_max +
			sizeof(Draw_Index)  * context->num_indices_max,
			&cpu_buffer
		);

		// reference vertex buffer range		
		context->gpu_vertex_buffer.buffer = context->gpu_buffer;
		context->gpu_vertex_buffer.offset = 0;
		context->gpu_vertex_buffer.size   = sizeof(Draw_Vertex) * context->num_vertices_max;
		
		// reference index buffer range
		context->gpu_index_buffer.buffer = context->gpu_buffer;
		context->gpu_index_buffer.offset = context->gpu_vertex_buffer.offset + context->gpu_vertex_buffer.size;
		context->gpu_index_buffer.size   = sizeof(Draw_Vertex) * context->num_vertices_max;
	
		context->cpu_vertex_buffer = ((char*)cpu_buffer) + context->gpu_vertex_buffer.offset;
		context->cpu_index_buffer  = ((char*)cpu_buffer) + context->gpu_index_buffer.offset;

        context->mvp_matrix = (M3x2) {
            1, 0, 0,
            0, 1, 0
        };

		engine->draw_context = context;
	}
}

void engine_pre_update(Engine* engine) {
    // memory
    //ENGINE_arena_clear(engine->frame_arena);
    
    // gui

	// draw
	draw_clear(engine->draw_context);
    // post update draw module
    {
        Draw_Context     *draw_context = engine->draw_context;
        Gpu_Command_List *command_list = &engine->draw_context->gpu_command_list;
		gpu_cmd_set_viewport(
			command_list, 
			0, 
			0, 
			engine->window_size_in_px.x, 
			engine->window_size_in_px.y
		);
        gpu_cmd_clear(
            command_list,
            GPU_CLEAR_FLAGS_ALL,
            (F32[]){ 0,0,0,0 },
            0,
            0
        );
    }
}
void engine_post_update(Engine* engine) {
//	gpu_command_queue_execute_command_lists(
//		engine->gpu_command_queue,
//		1,
//		engine->gpu_command_list
//	);

	
	// post update draw module
	{
		Draw_Context     *draw_context = engine->draw_context;
		Gpu_Command_List *command_list = &engine->draw_context->gpu_command_list;
		gpu_cmd_set_pipeline_state(
			command_list,
			draw_context->gpu_pipeline_state
		);
		gpu_cmd_set_topology(
			command_list,
			GPU_PRIMITIVE_TOPOLOGY_LINE_LIST
		);
        gpu_cmd_bind_vertex_buffer_range(
            command_list,
            0,
            draw_context->gpu_vertex_buffer.buffer,
            draw_context->gpu_vertex_buffer.offset,
            draw_context->gpu_vertex_buffer.size,
            sizeof(Draw_Vertex)
        );
		gpu_cmd_bind_index_buffer_range(
			command_list,
			1,
			draw_context->gpu_index_buffer.buffer,
			draw_context->gpu_index_buffer.offset,
			draw_context->gpu_index_buffer.size,
			sizeof(U16)
		);
		gpu_cmd_draw_indexed_instanced(
			command_list,
			draw_context->num_indices,
			1,
			0,
			0
		);
		gpu_cmd_close(command_list);
	}
}

#ifdef ENGINE_ENABLE_DYNAMIC_LINKING_USER_APP
#define USER_API extern
#else
#define USER_API
#endif

// windowing
#pragma region windowing
/* 
window_ // windowing
window_get_size
window_set_resize_callback
window_set_lost_focus_callback
window_set_fullscreen_change_callback
*/
F32  window_aspect();
V2   window_size();
M3x2 window_to_viewport();
M3x2 window_to_world(Camera* camera);
#pragma endregion // windowing

// user
USER_API Engine* engine;
USER_API void _init();
USER_API void _update();
USER_API void _audio(int count_of_sample, signed short *sample);

#undef  USER_API
#define USER_API extern

#ifndef ENGINE_DISABLE_GLOBAL_DEFINITIONS
#define S16LIT ENGINE_S16LIT
typedef ENGINE_V2F32 V2;
#endif

// windowing
#pragma region windowing
F32  window_aspect() {
    return engine->window_size_in_px.x / engine->window_size_in_px.y;
}
M3x2 window_to_viewport() {
    return m3x2(    
        2.0f / engine->window_size_in_px.x, 0                                 , -1.0f,
        0                                 , 2.0f / engine->window_size_in_px.y, -1.0f
	);
}
M3x2 viewport_to_window() {
    F32 half_window_size_x = engine->window_size_in_px.x * 0.5f;
    F32 half_window_size_y = engine->window_size_in_px.y * 0.5f;
    return m3x2(
        half_window_size_x, 0.0f   , half_window_size_x,
        0.0f              , half_window_size_y, half_window_size_y
	);
}
M3x2 window_to_world(Camera* camera) {
    return m3x2_multiply_matrix(
        viewport_to_world(camera, window_aspect()),
        window_to_viewport()
    );
}
M3x2 world_to_window(Camera* camera) {
    return m3x2_multiply_matrix(
        world_to_viewport(camera, window_aspect()),
        viewport_to_window()
    );
}
#pragma endregion // windowing

// inputs
#pragma region inputs
V2 mouse_wheel() {
    return engine->mouse_wheel;
}
V2 mouse_delta() {
    return engine->mouse_delta;
}
V2 mouse_position() {
    return engine->mouse_position;
}
B8 button_is_pressed(Button button) {
    return (engine->buttons[button >> 5] & ((U64)1 << ((button & (32 - 1)) << 1))) != 0;
}
B8 button_was_pressed(Button button) {
    return (engine->buttons[button >> 5] & ((U64)2 << ((button & (32 - 1)) << 1))) != 0;
}
B8 button_got_pressed(Button button) {
    return button_is_pressed(button) && !(button_was_pressed(button));
}
B8 button_got_released(char button) {
    return !(button_is_pressed(button)) && button_was_pressed(button);
}
#pragma endregion // inputs

