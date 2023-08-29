#include "engine_win32.h"

#define CRASH(proc_name) ErrorExit(proc_name)
#define CRASH_WITH_MSG(title, msg) GeneralErrorExit(title, msg)
#include <wingdi.h>
#include <gl\GL.h>
#pragma comment (lib, "Gdi32.lib")
#pragma comment (lib, "opengl32.lib")

typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
typedef char GLchar;

#define GL_MAP_READ_BIT         0x0001
#define GL_MAP_WRITE_BIT        0x0002
#define GL_MAP_PERSISTENT_BIT   0x0040
#define GL_MAP_COHERENT_BIT     0x0080
#define GL_TEXTURE0             0x84C0
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_FRAGMENT_SHADER      0x8B30
#define GL_VERTEX_SHADER        0x8B31
#define GL_COMPILE_STATUS       0x8B81
#define GL_INFO_LOG_LENGTH      0x8B84
#define GL_DRAW_FRAMEBUFFER     0x8CA9
#define GL_COLOR_ATTACHMENT0    0x8CE0
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_DEPTH_ATTACHMENT     0x8D00
#define GL_STENCIL_ATTACHMENT   0x8D20
#define GL_FRAMEBUFFER          0x8D40
#define GL_STENCIL_INDEX8       0x8D48
#define GL_DEBUG_OUTPUT         0x92E0

// GL_ARB_uniform_buffer_object
#define GL_UNIFORM_BUFFER  0x8A11

#define GL_VERTEX_SHADER_BIT   0x00000001
#define GL_FRAGMENT_SHADER_BIT 0x00000002

#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020

#define WGL_CONTEXT_MAJOR_VERSION_ARB          0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB          0x2092
#define WGL_CONTEXT_FLAGS_ARB                  0x2094
#define WGL_CONTEXT_OPENGL_NO_ERROR_ARB        0x31B3
#define WGL_CONTEXT_PROFILE_MASK_ARB           0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB              0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB       0x00000001

// wglChoosePixelFormatARB atribs
#define WGL_DRAW_TO_WINDOW_ARB    0x2001
#define WGL_ACCELERATION_ARB      0x2003
#define WGL_SWAP_METHOD_ARB       0x2007
#define WGL_SUPPORT_OPENGL_ARB    0x2010
#define WGL_DOUBLE_BUFFER_ARB     0x2011
#define WGL_PIXEL_TYPE_ARB        0x2013
#define WGL_COLOR_BITS_ARB        0x2014
#define WGL_DEPTH_BITS_ARB        0x2022
#define WGL_STENCIL_BITS_ARB      0x2023
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_SWAP_EXCHANGE_ARB     0x2028
#define WGL_TYPE_RGBA_ARB         0x202B

// tell nvidia and amd driver to prefer dedicated gpu device
#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport) DWORD NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#ifdef __cplusplus
}
#endif

#define GL_API_TABLE(OP) \
OP(void, glGetShaderiv, (GLuint shader, GLenum pname, GLint* param)) \
OP(void, glGetShaderInfoLog, (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)) \
OP(GLuint, glCreateShader, (GLenum type)) \
OP(void, glShaderSource, (GLuint shader, GLsizei count, const GLchar *const* string, const GLint* length)) \
OP(void, glCompileShader, (GLuint shader)) \
OP(GLuint, glCreateProgram, (void)) \
OP(void, glAttachShader, (GLuint program, GLuint shader)) \
OP(void, glDetachShader, (GLuint program, GLuint shader)) \
OP(void, glLinkProgram, (GLuint program)) \
OP(void, glUseProgram, (GLuint program)) \
OP(void, glCreateVertexArrays, (GLsizei n, GLuint *arrays)) \
OP(void, glBindVertexArray, (GLuint array)) \
OP(void, glVertexArrayAttribFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)) \
OP(void, glVertexArrayAttribIFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
OP(void, glEnableVertexArrayAttrib, (GLuint vaobj, GLuint index)) \
OP(void, glVertexArrayAttribBinding, (GLuint vaobj, GLuint attribindex, GLuint bindingindex)) \
OP(void, glCreateBuffers, (GLsizei n, GLuint* buffers)) \
OP(void, glNamedBufferStorage, (GLuint buffer, size_t size, const void *data, GLbitfield flags)) \
OP(void*, glMapNamedBufferRange, (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)) \
OP(GLboolean, glUnmapNamedBuffer, (GLuint buffer)) \
OP(void, glBindBuffer, (GLenum target, GLuint buffer)) \
OP(void, glBindBufferRange, (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
OP(void, glBindVertexBuffer, (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)) \
OP(void, glActiveTexture, (GLenum texture)) \
OP(void, glUniform1i, (GLint location, GLint v0)) \
OP(void, glUniformMatrix3x2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)) \
OP(void, glCreateTextures, (GLenum target, GLsizei n, GLuint *textures)) \
OP(void, glTextureStorage2D, (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)) \
OP(void, glTextureParameteri, (GLuint texture, GLenum pname, GLint param)) \
OP(void, glCreateFramebuffers, (GLsizei n, GLuint *ids)) \
OP(void, glNamedFramebufferTexture, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)) \
OP(void, glNamedFramebufferDrawBuffers, (GLuint framebuffer, GLsizei n, const GLenum *bufs)) \
OP(void, glBindFramebuffer, (GLenum target, GLuint framebuffer)) \
OP(GLenum, glCheckNamedFramebufferStatus, (GLuint framebuffer, GLenum target)) \
OP(BOOL, wglSwapIntervalEXT, (int interval)) 
//OP(void,   glGenProgramPipelines, (GLsizei n, GLuint *pipelines)) \
//OP(void,   glUseProgramStages, (GLuint pipeline, GLbitfield stages, GLuint program)) \
//OP(void,   glBindProgramPipeline, (GLuint pipeline)) \

// typedef gl procedure pointers
#define OP(ret, iden, args) typedef ret (__stdcall *iden##Proc) args;
GL_API_TABLE(OP)
#undef OP

// typedef wgl function pointers
typedef BOOL(__stdcall *wglChoosePixelFormatARBProc) (
	HDC          hdc,
    const int   *piAttribIList,
    const FLOAT *pfAttribFList,
    UINT         nMaxFormats,
    int         *piFormats,
    UINT        *nNumFormats
);
typedef HGLRC (__stdcall *wglCreateContextAttribsARBProc) (
	HDC hDC, 
	HGLRC hShareContext, 
	const int *attribList
);

// static gl procedure pointers
#define OP(ret, iden, args) static iden##Proc iden;
GL_API_TABLE(OP)
#undef OP

// static wgl ext procedure pointers
static wglChoosePixelFormatARBProc wglChoosePixelFormatARB;
static wglCreateContextAttribsARBProc wglCreateContextAttribsARB;

static GLenum GPU_PRIMITIVE_TOPOLOGY_TO_GL[] = {
	GL_TRIANGLES,
	GL_LINES,
	GL_POINTS
};

typedef struct Gpu_Device_Impl {
	Arena* arena;
} Gpu_Device_Impl;

typedef struct Gpu_Pipeline_State_Impl {
	GLuint program;
	GLuint primitive;
	GLuint vaobj;
	unsigned char blend_enabled;
	GLenum        blend_src;
	GLenum        blend_dst;
} Gpu_Pipeline_State_Impl;

typedef struct Gpu_Command_List_Impl {
	Arena* arena;
	GLenum primitive_topology;
	char*  index_buffer_offset;
} Gpu_Command_List_Impl;

typedef struct Win32_OpenGl {
	Gpu_Device_Impl device;
} Win32_OpenGl;
Win32_OpenGl  gl_;
Win32_OpenGl *gl;

#define GPU_COMMAND_TABLE(OP) \
OP(GPU_CMD_CLEAR, gpu_cmd_clear)

typedef unsigned char Gpu_Command_Type;
typedef struct Gpu_Command_Base {
	Gpu_Command_Type type;
	Gpu_U8           length;         
} Gpu_Command_Base;

typedef struct Gpu_Command_Clear {
	Gpu_Command_Base base;
	Gpu_Clear_Flags  flags;
	Gpu_F32          color_rgba[4];
	Gpu_F32          depth;
	Gpu_U8           stencil;
} Gpu_Command_Clear;

void engine_backend_win32_opengl_pre_window_init(
	HINSTANCE hInstance
) {
	// init opengl extentions

	// create a dummy opengl context
	// just to retrieve wgl extention function pointers
	HWND hwnd;
	HDC  hdc;
	{
		const TCHAR WINDOW_CLASS_NAME[]  = TEXT("OpenGlDummyContextWindow");
		{
			WNDCLASS window_class = { 0 };
			window_class.style         = 0; //CS_DISABLED; //CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
			window_class.lpfnWndProc   = WindowProc;
			window_class.hInstance     = hInstance;
			window_class.lpszClassName = WINDOW_CLASS_NAME;
			window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW+3);
			window_class.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(1));
			RegisterClass(&window_class);
		}
		hwnd = CreateWindowEx(
			0,                                   // Optional window styles.
			WINDOW_CLASS_NAME,                   // Window class
			WINDOW_CLASS_NAME,                   // Window text
			WS_DISABLED,                         // Window style
			CW_USEDEFAULT, CW_USEDEFAULT,        // Window position and 
			CW_USEDEFAULT, CW_USEDEFAULT,        // Window size
			NULL,       // Parent window    
			NULL,       // Menu
			hInstance,  // Instance handle
			NULL        // Additional application data
		);
		if (hwnd == NULL) ErrorExit(TEXT("CreateWindowEx"));
		hdc    = GetDC(hwnd);
	}

	// find an existing pixel format that match our program needs
	int                   index_of_pixel_format    = -1;
	PIXELFORMATDESCRIPTOR pixel_format_descriptor = { 0 };
	// "bargain" with operating system
	{
		// describes desired pixel format
		pixel_format_descriptor.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
		pixel_format_descriptor.nVersion     = 1;
		pixel_format_descriptor.dwFlags      = PFD_SUPPORT_OPENGL;
		pixel_format_descriptor.iPixelType   = PFD_TYPE_RGBA;
		pixel_format_descriptor.cColorBits   = 32;
		//pixel_format_descriptor.cAlphaBits   = 8;
		pixel_format_descriptor.cDepthBits   = 24;
		pixel_format_descriptor.cStencilBits = 8;
		//pixel_format_descriptor.iLayerType = PFD_MAIN_PLANE;
		// have operating system name us what existing pixel format is closest from our dream
		index_of_pixel_format = ChoosePixelFormat(hdc, &pixel_format_descriptor);

		// have operating system formally describes us the pixel format it just named
		// (step might be mandatory on some machines)
		DescribePixelFormat(
			hdc,
			index_of_pixel_format,
			sizeof(PIXELFORMATDESCRIPTOR),
			&pixel_format_descriptor
		);
	}
	SetPixelFormat(hdc, index_of_pixel_format, &pixel_format_descriptor);
	// create a dummy gl context to get access to ARB extentions
	HGLRC dummy_wgl_context_handle = wglCreateContext(hdc);
	wglMakeCurrent(hdc, dummy_wgl_context_handle);

	wglChoosePixelFormatARB    =    (wglChoosePixelFormatARBProc)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (wglCreateContextAttribsARBProc)wglGetProcAddress("wglCreateContextAttribsARB");

	wglDeleteContext(dummy_wgl_context_handle);
	ReleaseDC(hwnd, hdc);
	DestroyWindow(hwnd);
}

void engine_backend_win32_opengl_post_window_init() {
	gl = &gl_;

	// find an existing pixel format that match our program needs
	PIXELFORMATDESCRIPTOR pixel_format_descriptor = { 0 };
	// "bargain" with operating system
	{
		int iAttribIList[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,     32,
			WGL_DEPTH_BITS_ARB,     24,
			WGL_STENCIL_BITS_ARB,   8,
			WGL_SWAP_METHOD_ARB,    WGL_SWAP_EXCHANGE_ARB,
			0
		};
		int  iFormat = -1;
		UINT numFormats;
		wglChoosePixelFormatARB(
			win32->hdc,
			iAttribIList,
			0,
			1,
			&iFormat,
			&numFormats
		);
		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(
			win32->hdc,
			iFormat,
			sizeof(pfd),
			&pfd
		);
		SetPixelFormat(win32->hdc, iFormat, &pixel_format_descriptor);
	}
	
	int attribList[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB,   3,
		WGL_CONTEXT_MINOR_VERSION_ARB,   1, 
		WGL_CONTEXT_FLAGS_ARB,           WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,    WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
//		WGL_CONTEXT_OPENGL_NO_ERROR_ARB, GL_TRUE,
		0
	};
	HGLRC context = wglCreateContextAttribsARB(win32->hdc, 0, attribList);
	wglMakeCurrent(win32->hdc, context);
	//SwapBuffers(win32->hdc);

	// config
	glDisable(GL_DEBUG_OUTPUT);
	//glEnable   (GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glEnable   (GL_CULL_FACE);
	glFrontFace(GL_CCW); 
    glDisable(GL_BLEND);

	// get function pointers
	#define OP(ret, iden, args) iden = (iden##Proc)wglGetProcAddress(#iden);
	GL_API_TABLE(OP)
	#undef OP

	// enable vsync by default
	if(wglSwapIntervalEXT) wglSwapIntervalEXT(1);

	// create "device"
	gl->device.arena = arena_alloc();
	engine->gpu_device = &gl->device;
}

Gpu_Buffer gpu_make_buffer(Gpu_U64 size) {
	GLuint buffer;
	glCreateBuffers(1, &buffer);
	glNamedBufferStorage(
		buffer,
		size,
		0,
		GL_MAP_WRITE_BIT
	);
	return (Gpu_Buffer)(Gpu_U64)buffer;
}

Gpu_Buffer gpu_make_buffer_coherent(Gpu_U64 size, void** data) {
	GLuint buffer;
	glCreateBuffers(1, &buffer);
	glNamedBufferStorage(
		buffer,
		size,
		0,
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
	);
	*data = glMapNamedBufferRange(
		buffer,
		0,
		size,
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
	);
	return (Gpu_Buffer)(Gpu_U64)buffer;
}

// Gpu_Texture gpu_make_texture(
// 	Gpu_Device device,
//     int   width,
//     int   height,
// 	void* data
// ) {
//     //int  width;
//     //int  height;
//     //int  channel;
//     //auto data = stbi_load_from_memory(typo_data, typo_size, &width, &height, &channel, 4);
//     GLuint texture_name;
//     glGenTextures(1, &texture_name);
//     glBindTexture(GL_TEXTURE_2D, texture_name);
//     glTexImage2D(
//         GL_TEXTURE_2D,
//         0,
//         GL_RGBA8,
//         width,
//         height,
//         0,
//         GL_RGBA,
//         GL_UNSIGNED_BYTE,
//         data
//     );
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     return (Gpu_Texture)texture_name;
// }

void* gpu_buffer_map(Gpu_Buffer buffer, Gpu_U64 size) {
	return glMapNamedBufferRange(
		buffer,
		0,
		size,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT
	);
}

void gpu_buffer_unmap(Gpu_Buffer buffer) {
	glUnmapNamedBuffer(buffer);
}

Gpu_Pipeline_State gpu_make_pipeline_state(
	Gpu_Device               device,
	Gpu_Pipeline_State_Desc *pipeline_state_desc
) {
	GLuint program_name;
	GLuint vertex_shader_name; 
	GLuint fragment_shader_name;
	Gpu_U64 has_vertex_shader =
		pipeline_state_desc->vertex_shader.length;
	Gpu_U64 has_fragment_shader =
		pipeline_state_desc->fragment_shader.length;
	program_name = glCreateProgram();
	// shaders
	if (has_vertex_shader) {
		vertex_shader_name = glCreateShader(GL_VERTEX_SHADER);
		GLint size = pipeline_state_desc->vertex_shader.length;
		glShaderSource(
			vertex_shader_name, 
			1, 
			(GLchar**)&(pipeline_state_desc->vertex_shader.data), 
			&size
		);
		glCompileShader(vertex_shader_name);
		{
			GLint compile_status = GL_FALSE;
			glGetShaderiv(vertex_shader_name, GL_COMPILE_STATUS, &compile_status);
			if (compile_status == GL_FALSE) {
				int info_log_length;
				glGetShaderiv(vertex_shader_name, GL_INFO_LOG_LENGTH, &info_log_length);

				if (info_log_length) {
					const char log_intro[] = "Failed to compile vertex shader:\n";
					char log_buffer[1024];
					strcpy(log_buffer, log_intro);
					GLsizei log_length;
					glGetShaderInfoLog(vertex_shader_name, sizeof(log_buffer) - sizeof(log_intro), &log_length, &log_buffer[sizeof(log_intro) - 1]);
					log_buffer[sizeof(log_intro) - 1 + log_length] = '\0';
                    CRASH_WITH_MSG("glCompileShader", log_buffer);
				}
			}
		}
		glAttachShader(program_name, vertex_shader_name);
	}
	if (has_fragment_shader) {
		fragment_shader_name = glCreateShader(GL_FRAGMENT_SHADER);
		GLint size = pipeline_state_desc->fragment_shader.length;
		glShaderSource(
			fragment_shader_name, 
			1, 
			(GLchar**)&(pipeline_state_desc->fragment_shader.data), 
			&size
		);
		glCompileShader(fragment_shader_name);
		{
			GLint compile_status = GL_FALSE;
			glGetShaderiv(fragment_shader_name, GL_COMPILE_STATUS, &compile_status);
			if (compile_status == GL_FALSE) {
				int info_log_length;
				glGetShaderiv(fragment_shader_name, GL_INFO_LOG_LENGTH, &info_log_length);

				if (info_log_length) {
					const char log_intro[] = "Failed to compile fragment shader:\n";
					char log_buffer[1024];
					strcpy(log_buffer, log_intro);
					GLsizei log_length;
					glGetShaderInfoLog(fragment_shader_name, sizeof(log_buffer) - sizeof(log_intro), &log_length, &log_buffer[sizeof(log_intro) - 1]);
					log_buffer[sizeof(log_intro) - 1 + log_length] = '\0';
                    CRASH_WITH_MSG("glCompileShader", log_buffer);
				}
			}
		}
		glAttachShader(program_name, fragment_shader_name);
	}
	glLinkProgram(program_name);

	if(has_vertex_shader)
		glDetachShader(program_name, vertex_shader_name);
	if(has_fragment_shader)
		glDetachShader(program_name, fragment_shader_name);

	// vertex array object
	GLuint vaobj;
	glCreateVertexArrays(1, &vaobj);
	//glBindVertexArray(vaobj);
	int num_input_layouts = pipeline_state_desc->num_input_layouts;
	Gpu_Input_Layout* input_layout;
	for (
		int i_input_layout = 0; 
		i_input_layout < num_input_layouts; 
		++i_input_layout
	) {
		input_layout = &pipeline_state_desc->input_layouts[i_input_layout];
		GLint size = (GLint[]){ 
			0, // GPU_FORMAT_UNKNOWN
			1, // GPU_FORMAT_U32
			1, // GPU_FORMAT_F32
			2, // GPU_FORMAT_V2F32
			4, // GPU_FORMAT_V4F32
			4, // GPU_FORMAT_V4F32_RGBA
			1, // GPU_FORMAT_V4U8_RGBA
			0, // GPU_FORMAT_COUNT
		} [input_layout->format];
		GLenum type = (GLenum[]){ 
			0,               // GPU_FORMAT_UNKNOWN
			GL_UNSIGNED_INT, // GPU_FORMAT_U32
			GL_FLOAT,        // GPU_FORMAT_F32
			GL_FLOAT,        // GPU_FORMAT_V2F32
			GL_FLOAT,        // GPU_FORMAT_V4F32
			GL_FLOAT,        // GPU_FORMAT_V4F32_RGBA
            GL_UNSIGNED_INT, // GPU_FORMAT_V4U8_RGBA
			0,               // GPU_FORMAT_COUNT
		} [input_layout->format];
		glEnableVertexArrayAttrib(vaobj, input_layout->slot);
        if(type == GL_FLOAT) {
            glVertexArrayAttribFormat(
                vaobj,
                input_layout->slot,
                size,
                type,
                GL_FALSE,
                input_layout->offset
            );
        } else {
            glVertexArrayAttribIFormat(
                vaobj,
                input_layout->slot,
                size,
                type,
                input_layout->offset
            );
        }
		glVertexArrayAttribBinding(
			vaobj,
			input_layout->slot,
			0
		);
	}

	Gpu_Device_Impl* device_impl = (Gpu_Device_Impl*)device;
	Gpu_Pipeline_State_Impl *pipeline_state = 
		ARENA_INSERT_STRUCT(device_impl->arena, Gpu_Pipeline_State_Impl);
	pipeline_state->primitive = GL_TRIANGLES;
	pipeline_state->program   = program_name;
	pipeline_state->vaobj     = vaobj;
	pipeline_state->blend_enabled = pipeline_state_desc->output_merger_state.blend_enabled;
	{
		GLenum *map_engine_blend_to_opengl = (GLenum[]){
			GL_ONE,                 /* GPU_BLEND_ONE */
			GL_SRC_ALPHA,           /* GPU_BLEND_SRC_ALPHA */
			GL_ONE_MINUS_SRC_ALPHA  /* GPU_BLEND_ONE_MINUS_SRC_ALPHA */
		};
		pipeline_state->blend_src = map_engine_blend_to_opengl[pipeline_state_desc->output_merger_state.blend_src];
		pipeline_state->blend_dst = map_engine_blend_to_opengl[pipeline_state_desc->output_merger_state.blend_dst];
	}
	return (Gpu_Pipeline_State)pipeline_state;
}

// gpu commands
void gpu_cmd_clear(
	Gpu_Command_List *command_list,
	Gpu_Clear_Flags   flags,
	Gpu_F32           color_rgba[4],
	Gpu_F32           depth,
	Gpu_U8            stencil
) {
	//glViewport(0, 0, engine->window_size_in_px.x, engine->window_size_in_px.y);
	//GLenum a = glGetError();
	GLbitfield mask = 0;
	if (flags & GPU_CLEAR_FLAGS_COLOR) {
		glClearColor(
			color_rgba[0],
			color_rgba[1],
			color_rgba[2],
			color_rgba[3]
		);
		mask |= GL_COLOR_BUFFER_BIT;
	}
	if (flags & GPU_CLEAR_FLAGS_COLOR) {
		mask |= GL_DEPTH_BUFFER_BIT;
		glClearDepth(depth);
	}
	if (flags & GPU_CLEAR_FLAGS_COLOR) {
		mask |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(stencil);
	}
	glClear(mask);
    int error = glGetError();
	//glFlush();
}

void gpu_cmd_close(Gpu_Command_List *command_list) {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glFlush();
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glDisable(GL_CULL_FACE);
	//int error = glGetError();
    //error = glGetError();
	SwapBuffers(win32->hdc);
	//ReleaseDC (win32->hwnd, win32->hdc );
	//glFlush();
}

void gpu_cmd_set_pipeline_state(
	Gpu_Command_List *command_list,
	Gpu_Pipeline_State pipeline_state
) {
	Gpu_Pipeline_State_Impl *pipeline_state_impl = 
		(Gpu_Pipeline_State_Impl*) pipeline_state;
	glUseProgram(pipeline_state_impl->program);
	glBindVertexArray(pipeline_state_impl->vaobj);

	if(pipeline_state_impl->blend_enabled) {
		glEnable(GL_BLEND);
		glBlendFunc(
			pipeline_state_impl->blend_src, 
			pipeline_state_impl->blend_dst
		);
	} else {
		glDisable(GL_BLEND);
	}
    //glViewport(0, 0, engine->window_size_in_px.x, engine->window_size_in_px.y);
}

void gpu_cmd_set_topology(
	Gpu_Command_List       *command_list, 
	Gpu_Primitive_Topology  primitive_topology
) {
	Gpu_Command_List_Impl *command_list_impl = 
		(Gpu_Pipeline_State_Impl*)command_list;
	command_list_impl->primitive_topology =
		GPU_PRIMITIVE_TOPOLOGY_TO_GL[primitive_topology];
}

void gpu_cmd_draw_instanced(
	Gpu_Command_List *command_list,
	Gpu_U32 count_of_vertex_per_instance,
	Gpu_U32 count_of_instance,
	Gpu_U32 start_vertex_location, // offset
	Gpu_U32 start_instance_location
) {
	Gpu_Command_List_Impl* command_list_impl =
		(Gpu_Command_List_Impl*)command_list;
	glDrawArrays(
		command_list_impl->primitive_topology, 
		start_vertex_location, 
		count_of_vertex_per_instance
	);
}

void gpu_cmd_draw_indexed_instanced(
	Gpu_Command_List *command_list,
	Gpu_U32 count_of_index_per_instance,
	Gpu_U32 count_of_instance,
	Gpu_U32 start_index_location, // offset
	Gpu_U32 start_instance_location
) {
	Gpu_Command_List_Impl* command_list_impl =
		(Gpu_Command_List_Impl*)command_list;
	glDrawElements(
		command_list_impl->primitive_topology, 
		count_of_index_per_instance, 
		GL_UNSIGNED_SHORT,
		command_list_impl->index_buffer_offset +
		start_index_location * sizeof(unsigned short)
	);
}

void gpu_cmd_bind_uniform_buffer(
	Gpu_Command_List *command_list,
	Gpu_U8            slot, 
	Gpu_Buffer        buffer,
	Gpu_U64           size
) {
	glBindBufferRange(
		GL_UNIFORM_BUFFER,
		slot,
		buffer,
		0,
		size
	);
}

void gpu_cmd_bind_vertex_buffer_range(
	Gpu_Command_List *command_list,
	Gpu_U8            slot, 
	Gpu_Buffer        buffer,
	Gpu_U64           offset,
	Gpu_U64           size,
	Gpu_U64           stride
) {
	glBindVertexBuffer(
		slot,
		buffer,
		offset,
        stride
	);
}

void gpu_cmd_bind_index_buffer_range(
	Gpu_Command_List *command_list,
	Gpu_U8            slot, 
	Gpu_Buffer        buffer,
	Gpu_U64           offset,
	Gpu_U64           size,
	Gpu_U64           stride
) {
	Gpu_Command_List_Impl* command_list_impl =
		(Gpu_Command_List_Impl*)command_list;
	command_list_impl->index_buffer_offset = offset;
	glBindBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		buffer
	);
}

// descriptors / uniforms
void gpu_cmd_bind_texture(
	Gpu_Command_List *command_list,
	Gpu_Texture       texture,
	U32               slot
) {
	glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
	glUniform1i(slot, slot);
}

void gpu_cmd_bind_render_texture(
	Gpu_Command_List *command_list,
	Gpu_Texture       texture,
	U32               slot
) {
	glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, (GLuint)(((U64)texture) >> 32));
	glUniform1i(slot, slot);
}

void gpu_cmd_set_uniform_m3x2(
	Gpu_Command_List *command_list,
	M3x2             *matrix3x2,
	U32               slot
) {
	glUniformMatrix3x2fv(slot, 1, GL_TRUE, matrix3x2);
}

void gpu_cmd_set_render_texture(
	Gpu_Command_List *command_list,
	Gpu_Texture       render_texture
) {
	glBindFramebuffer(GL_FRAMEBUFFER, ((U64)render_texture) & 0xFFFFFFFFULL);
}

gpu_cmd_set_viewport(
	Gpu_Command_List *command_list, 
	F32 x, 
	F32 y, 
	F32 w, 
	F32 h
) {
	glViewport(x, y, w, h);
}

Gpu_Command_Allocator gpu_make_command_allocator(Gpu_Device device) {
	return (Gpu_Command_Allocator)arena_alloc();
}

Gpu_Command_List gpu_make_command_list(Gpu_Command_Allocator allocator) {
	Arena* allocator_impl = (Arena*)allocator;
	Gpu_Command_List_Impl* command_list = 
		ARENA_INSERT_STRUCT(allocator_impl, Gpu_Command_List_Impl);
	command_list->arena            = allocator_impl;
	command_list->primitive_topology = GL_TRIANGLES;
	return (Gpu_Command_Allocator)command_list;
}

Gpu_Texture gpu_make_render_texture(U64 width, U64 height) {
	/* Needs:
	 *   * Framebuffer
	 *   * N Color Textures
	 *   * N Depth Buffer (not needed)
	 *   * N Stensil Buffer (needed)
	 * For started:
	 *   * 1 Frambuffer
	 *   * 1 Color texture buffer (RGBA)
	 *   * 1 Stensil Buffer
	 * */

	GLenum error;
    error = glGetError();

	union Texture_Names {
		GLuint as_array[2];
		struct As_Struct {
			GLuint color;
			GLuint setencil;
		} as_identifiers;
	} texture_names;
	glCreateTextures(GL_TEXTURE_2D, 2, &texture_names);
    {
		glTextureStorage2D(
			texture_names.as_identifiers.color,
			1,
			GL_RGBA8,
			width,
			height
		);
        error = glGetError();
        glTextureParameteri(
			texture_names.as_identifiers.color, 
			GL_TEXTURE_MAG_FILTER, 
			GL_LINEAR
		);
        error = glGetError();
        glTextureParameteri(
			texture_names.as_identifiers.color, 
			GL_TEXTURE_MIN_FILTER, 
			GL_LINEAR
		);
		error = glGetError();
		int k = 1;
    }
    {
		glTextureStorage2D(
			texture_names.as_identifiers.setencil,
			1,
            GL_STENCIL_INDEX8,
			width,
			height
		);
        error = glGetError();
        glTextureParameteri(
			texture_names.as_identifiers.setencil, 
			GL_TEXTURE_MAG_FILTER, 
			GL_NEAREST
		);
        error = glGetError();
        glTextureParameteri(
			texture_names.as_identifiers.setencil, 
			GL_TEXTURE_MIN_FILTER, 
			GL_NEAREST
		);
		error = glGetError();
		int k = 1;
    }

    GLuint framebuffer_name;
	{
		glCreateFramebuffers(1, &framebuffer_name);
		glNamedFramebufferTexture(
			framebuffer_name, 
			GL_COLOR_ATTACHMENT0,
			texture_names.as_identifiers.color, 
			0
		);
		glNamedFramebufferTexture(
			framebuffer_name, 
			GL_STENCIL_ATTACHMENT, 
			texture_names.as_identifiers.setencil, 
			0
		);
		GLenum draw_buffers[1];
		draw_buffers[0] = GL_COLOR_ATTACHMENT0;
		glNamedFramebufferDrawBuffers(
			framebuffer_name,
			1,
			draw_buffers
		);
		error = glGetError();
		int k = 1;
	}


    GLenum status = glCheckNamedFramebufferStatus(framebuffer_name, GL_DRAW_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
		return NULL; /* should release name */

	return ((U64)framebuffer_name) | ((U64)texture_names.as_identifiers.color) << 32ULL;
}

#include "engine_win32_asset_gpu.h"