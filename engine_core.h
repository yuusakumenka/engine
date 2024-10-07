/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

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

typedef struct Window_State {
	S8  title;
	V4  resolution; /* x: width, y: height, z: 1/width, w: 1/height */
	F32 aspect;

	B8   is_hovered    : 1;
	B8  was_hovered    : 1;
	B8  got_hovered    : 1;
	B8  got_unhovered  : 1;

	B8   is_focussed   : 1;
	B8  was_focussed   : 1;
	B8  got_focussed   : 1;
	B8  got_unfocussed : 1;

	B8   is_maximized   : 1;
	B8  was_maximized   : 1;
	B8  got_maximized   : 1;
	B8  got_unmaximized : 1;

	B8   is_minimized   : 1;
	B8  was_minimized   : 1;
	B8  got_minimized   : 1;
	B8  got_unminimized : 1;
} Window_State;

Window_State *window(U8 id /* reserved for future use*/);

typedef struct Engine_Startup_Desc {
	S8  window_title;
	F32 prefered_aspect_ratio;
} Engine_Startup_Desc;

typedef struct Engine {
    // memory
    Arena* frame_arena;
    Arena*   app_arena;

    // windowing & rendering
    ENGINE_String16           window_title;
	ENGINE_V2F32              window_size_in_px; /* @DEPRECATED use Os_Surface API instead. */
	Engine_Windowing_Flags    window_flags;
    Engine_Graphics_Buffering window_buffering;

	// gpu
	Gpu_Device           *gpu_device;
	Gpu_Command_Allocator gpu_command_allocator;
	Gpu_Command_List     *gpu_command_list;
	Gpu_Command_Queue     gpu_command_queue;

    // mouse cursor
    Os_Cursor cursor;
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
	size_t     frame_generation;

    // inputs
    V2 mouse_wheel;
    V2 mouse_delta;
    V2 mouse_position;
    V2 mouse_position_at_last_frame;

	#ifndef NEW_BUTTON_SYSTEM
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
	#else
	Button_State buttons[BUTTON_COUNT];
	#endif


	// camera
	Camera      camera;

	// draw
	Draw_Context* draw_context;

	/* Procs */
	void (*update)();
	void (*audio )(size_t num_samples, signed short *samples);

	/* Scheduling */
	Job_Ring64 job_ring;
	Job_Ring64 job_ring_graphics;
	void (*worker_init_proc)(void *data);
	void (*worker_graphic_init_proc)(void *data);
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

	#ifndef ENGINE_DISABLE_GPU

	// gpu
	if (0) {
		engine->gpu_command_allocator = 
			gpu_make_command_allocator(engine->gpu_device);
		engine->gpu_command_list = 
			gpu_make_command_list(engine->gpu_command_allocator);
	}

	#if defined(ENGINE_ENABLE_OLD_DRAW_MODULE)
	// init draw module
	{
		Draw_Context* context = ARENA_INSERT_STRUCT(engine->app_arena, Draw_Context);
		
		{
			Gpu_Vertex_Attribute       vertex_attributes[2];
			{
				// position
				vertex_attributes[0].slot   = 0;
				vertex_attributes[0].format = GPU_FORMAT_V2F32;
				vertex_attributes[0].offset = 0;
				// color
				vertex_attributes[1].slot   = 1;
				vertex_attributes[1].format = GPU_FORMAT_V4U8_RGBA;
				vertex_attributes[1].offset = 
					vertex_attributes[0].offset + 
					GPU_FORMAT_SIZE[vertex_attributes[0].format];
			}

			Gpu_Pipeline_State_Desc gpu_pipeline_state_desc = { 0 };
			gpu_pipeline_state_desc.vertex_attributes     = vertex_attributes;
			gpu_pipeline_state_desc.num_vertex_attributes = 
				sizeof(vertex_attributes) / sizeof(vertex_attributes[0]);

			gpu_pipeline_state_desc.output_merger_state.depth_test_enabled   = 0;
			gpu_pipeline_state_desc.output_merger_state.depth_write_disabled = 1;

			Arena *arena = engine->app_arena;
			ARENA_SCOPE(arena) {
				gpu_pipeline_state_desc.vertex_shader.data = 
					asset_read(
					arena, 
					"shaders/draw.vert", 
					&gpu_pipeline_state_desc.vertex_shader.length
				);
				gpu_pipeline_state_desc.fragment_shader.data = 
					asset_read(
					arena, 
					"shaders/draw.frag", 
					&gpu_pipeline_state_desc.fragment_shader.length
				);
				context->gpu_pipeline_state = 
					gpu_make_pipeline_state(
					engine->gpu_device, 
					&gpu_pipeline_state_desc
				);
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
	#endif
	#endif /* !ENGINE_DISABLE_GPU */
}

void engine_pre_update(Engine* engine) {
    // memory
    //ENGINE_arena_clear(engine->frame_arena);
	arena_clear(engine->frame_arena);
    
    // gui

	#ifdef ENGINE_ENABLE_OLD_DRAW_MODULE
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
    }
	#endif

	union {
		I32 as_i32;
		F32 as_f32;
	} x;
	x.as_i32 = interlocked_compare_exchange32(&engine->window_size_in_px.x, 0, 0);
	union {
		I32 as_i32;
		F32 as_f32;
	} y;
	y.as_i32 = interlocked_compare_exchange32(&engine->window_size_in_px.y, 0, 0);
	gpu_cmd_set_viewport(
		engine->gpu_command_list,
		0,
		0,
		x.as_f32,
		y.as_f32
	);
	gpu_cmd_clear(
		engine->gpu_command_list,
		GPU_CLEAR_FLAGS_ALL,
		(F32[]){ 0,0,0,0 },
		1,
		0
	);
}

// void engine_post_update(Engine* engine) {
// //	gpu_command_queue_execute_command_lists(
// //		engine->gpu_command_queue,
// //		1,
// //		engine->gpu_command_list
// //	);

// 	#ifdef ENGINE_ENABLE_OLD_DRAW_MODULE
// 	Gpu_Command_List *command_list = &engine->draw_context->gpu_command_list;
// 	// post update draw module
// 	{
// 		Draw_Context     *draw_context = engine->draw_context;
// 		gpu_cmd_set_pipeline_state(
// 			command_list,
// 			draw_context->gpu_pipeline_state
// 		);
// 		gpu_cmd_set_topology(
// 			command_list,
// 			GPU_PRIMITIVE_TOPOLOGY_LINE_LIST
// 		);
//         gpu_cmd_bind_vertex_buffer_range(
//             command_list,
//             0,
//             draw_context->gpu_vertex_buffer.buffer,
//             draw_context->gpu_vertex_buffer.offset,
//             draw_context->gpu_vertex_buffer.size,
//             sizeof(Draw_Vertex)
//         );
// 		gpu_cmd_bind_index_buffer_range(
// 			command_list,
// 			1,
// 			draw_context->gpu_index_buffer.buffer,
// 			draw_context->gpu_index_buffer.offset,
// 			draw_context->gpu_index_buffer.size,
// 			sizeof(U16)
// 		);
// 		gpu_cmd_draw_indexed_instanced(
// 			command_list,
// 			draw_context->num_indices,
// 			1,
// 			0,
// 			0
// 		);
// 	}
// 	#endif
// 	//gpu_present(engine->gpu_command_list);
	
// 	++engine->frame_generation;
// }

void engine_main_loop_body();

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

#ifndef _UPDATE
USER_API void engine_default_update_proc() {}
#define _UPDATE engine_default_update_proc
#endif /* _UPDATE */

#ifndef _STARTUP
USER_API void engine_default_startup_proc(Engine* engine) {
	engine->update = _UPDATE;
}
#define _STARTUP engine_default_startup_proc
#endif /* _STARTUP */

//USER_API void _update();
//USER_API void _audio(int count_of_sample, signed short *sample);

#undef  USER_API
#define USER_API extern

#ifndef ENGINE_DISABLE_GLOBAL_DEFINITIONS
#define S16LIT ENGINE_S16LIT
typedef ENGINE_V2F32 V2;
#endif

// windowing
#pragma region windowing
F32  window_aspect(Os_Surface *surface) {
	V2 resolution = os_surface_get_resolution(surface);
    return engine->window_size_in_px.x / engine->window_size_in_px.y;
}
M3x2 window_to_viewport(Os_Surface *surface) {
	V2 resolution = os_surface_get_resolution(surface);
    return m3x2(    
        2.0f / resolution.x, 0                  , -1.0f,
        0                  , 2.0f / resolution.y, -1.0f
	);
}
M3x2 viewport_to_window(Os_Surface *surface) {
	V2 resolution = os_surface_get_resolution(surface);
    F32 half_resolution_x = resolution.x * 0.5f;
    F32 half_resolution_y = resolution.y * 0.5f;
    return m3x2(
        half_resolution_x, 0.0f             , half_resolution_x,
        0.0f             , half_resolution_y, half_resolution_y
	);
}
M3x2 window_to_world(Camera* dst_camera, Os_Surface *src_surface) {
    return m3x2_multiply_matrix(
        viewport_to_world(dst_camera, window_aspect(src_surface)),
        window_to_viewport(src_surface)
    );
}
M3x2 world_to_window(Os_Surface *dst_surface, Camera* src_camera) {
    return m3x2_multiply_matrix(
        world_to_viewport(src_camera, window_aspect(dst_surface)),
        viewport_to_window(dst_surface)
    );
}
#pragma endregion // windowing

/* @DEPRECATED Core inputs are deprecated use engine_in.h instead. */
// inputs
#pragma region inputs
/* in */

/* @DEPRECATED */
void engine_collect_user_inputs_job(void *data);

/* @DEPRECATED use in_mouse().wheel instead */
V2 mouse_wheel() {
    return engine->mouse_wheel;
}
/* @DEPRECATED use in_mouse().delta instead */
V2 mouse_delta() {
    return engine->mouse_delta;
}
V2 mouse_position(Os_Surface *surface);

/* @DEPRECATED use in_button() API instead */
B8 button_is_pressed(Button button) {
    return (engine->buttons[button >> 5] & ((U64)1 << ((button & (32 - 1)) << 1))) != 0;
}
/* @DEPRECATED use in_button() API instead */
B8 button_was_pressed(Button button) {
    return (engine->buttons[button >> 5] & ((U64)2 << ((button & (32 - 1)) << 1))) != 0;
}
/* @DEPRECATED use in_button() API instead */
B8 button_got_pressed(Button button) {
    return button_is_pressed(button) && !(button_was_pressed(button));
}
/* @DEPRECATED use in_button() API instead */
B8 button_got_released(Button button) {
    return !(button_is_pressed(button)) && button_was_pressed(button);
}
#pragma endregion // inputs

// crash
void engine_error(char *title, char *msg, ...);
void engine_crash(int error);