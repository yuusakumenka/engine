/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#define UI_RENDERER_USE_NEW_LAYOUT_SYSTEM

#define UI_STANDALONE_ASCII_RASTER_FONT_WIDTH 96
#define UI_STANDALONE_ASCII_RASTER_FONT_HEIGHT 96
#define UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH 6
#define UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_HEIGHT 12
#define UI_STANDALONE_ASCII_RASTER_FONT_SCALE UI_SCALE
#define UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH_SCALED (UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH * UI_STANDALONE_ASCII_RASTER_FONT_SCALE)
#define UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_HEIGHT_SCALED (UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_HEIGHT * UI_STANDALONE_ASCII_RASTER_FONT_SCALE)

static const char UI_STANDALONE_ASCII_RASTER_FONT_DATA[] = {
    /* Generated with img2carray.html from "asset/typo.png" */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0xE4, 0x00, 0x06, 0x00, 0x00, 0x0F, 0x7C,
    0x40, 0x30, 0xFC, 0xE0, 0x0E, 0x8E, 0x81, 0x16, 0x0E, 0x80, 0x10, 0x84, 0xC3, 0x40, 0xB4, 0x41,
    0x1A, 0x84, 0xC3, 0x3F, 0x19, 0x40, 0x28, 0x74, 0x42, 0x21, 0xF4, 0x02, 0x2F, 0x00, 0x82, 0x19,
    0x15, 0x40, 0x20, 0x04, 0x42, 0x12, 0x04, 0x42, 0x20, 0x04, 0xE2, 0x70, 0x13, 0x40, 0x20, 0x04,
    0x42, 0x74, 0x04, 0x42, 0x20, 0x04, 0xE2, 0x70, 0x0E, 0x40, 0x20, 0x04, 0x42, 0x08, 0xF4, 0x42,
    0x2F, 0x04, 0x82, 0x19, 0x40, 0x85, 0x10, 0x04, 0x42, 0x1E, 0x94, 0x42, 0x29, 0x04, 0xC2, 0x3F,
    0x00, 0x00, 0x0F, 0xFC, 0xC3, 0x01, 0xFC, 0xC3, 0x3F, 0xFC, 0x83, 0x16, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x0F, 0x60,
    0x00, 0x0F, 0x00, 0x80, 0x3F, 0xA8, 0x12, 0x04, 0xF8, 0x81, 0x1F, 0x60, 0x80, 0x16, 0x00, 0xC0,
    0x3F, 0x54, 0xD1, 0x55, 0x98, 0xA1, 0x5F, 0x60, 0x40, 0x2F, 0x00, 0xC0, 0x00, 0x08, 0x50, 0x55,
    0xC0, 0x80, 0x1F, 0xF8, 0x21, 0x6F, 0x02, 0xC4, 0x1F, 0x54, 0xD1, 0x55, 0x60, 0x00, 0x0F, 0xF0,
    0x40, 0x2F, 0x04, 0x82, 0x3F, 0xA8, 0xD2, 0x54, 0x00, 0x40, 0x26, 0x60, 0x80, 0x16, 0x08, 0x01,
    0x30, 0x00, 0x51, 0x55, 0x60, 0x00, 0x00, 0x04, 0x02, 0x0F, 0xF0, 0xC0, 0x3F, 0xA8, 0x52, 0x75,
    0x60, 0x00, 0x06, 0xFC, 0x03, 0x00, 0x00, 0xC0, 0x1F, 0x54, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA1, 0x00, 0xC4,
    0x60, 0x08, 0x88, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0xA1, 0x28, 0xDE, 0x94, 0x08, 0x04, 0x41,
    0x10, 0x00, 0x00, 0x20, 0x00, 0x01, 0x7C, 0x05, 0x52, 0x00, 0x02, 0x52, 0x11, 0x00, 0x00, 0x10,
    0x00, 0x01, 0x28, 0x0E, 0x21, 0x00, 0x02, 0xE2, 0x7C, 0xC0, 0x07, 0x10, 0x00, 0x01, 0x28, 0x94,
    0x50, 0x01, 0x02, 0x52, 0x11, 0x00, 0x00, 0x10, 0x11, 0x00, 0x7C, 0x4F, 0x96, 0x00, 0x04, 0x41,
    0x10, 0x02, 0x40, 0x08, 0x1F, 0x01, 0x28, 0x04, 0x66, 0x01, 0x88, 0x00, 0x00, 0x01, 0x00, 0x08,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0xE1, 0x38, 0xCA,
    0xE7, 0x7C, 0x8E, 0x03, 0x00, 0x00, 0x00, 0x38, 0x91, 0x11, 0x45, 0x49, 0x10, 0x41, 0x51, 0x04,
    0x00, 0x08, 0x20, 0x44, 0x19, 0x01, 0x41, 0x49, 0x10, 0x20, 0x51, 0x24, 0x08, 0xC4, 0x47, 0x40,
    0x15, 0xC1, 0x30, 0x9F, 0xF3, 0x20, 0x8E, 0x07, 0x00, 0x02, 0x80, 0x20, 0x13, 0x21, 0x40, 0x08,
    0x14, 0x11, 0x11, 0x04, 0x00, 0xC4, 0x47, 0x10, 0x11, 0x11, 0x44, 0x48, 0x14, 0x11, 0x51, 0x24,
    0x08, 0x08, 0x20, 0x00, 0x8E, 0xF3, 0x39, 0x88, 0xE3, 0x10, 0x8E, 0x03, 0x04, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8E, 0xF3, 0x78, 0xC7,
    0xF7, 0x39, 0x11, 0x01, 0x45, 0x41, 0x14, 0x39, 0x51, 0x14, 0x05, 0x49, 0x10, 0x44, 0x11, 0x01,
    0x25, 0xC1, 0x36, 0x45, 0x5D, 0x14, 0x05, 0x51, 0x10, 0x04, 0x11, 0x01, 0x1D, 0x41, 0x55, 0x45,
    0xD5, 0xF7, 0x04, 0xD1, 0x71, 0x04, 0x1F, 0x01, 0x25, 0x41, 0x95, 0x45, 0x5D, 0x14, 0x05, 0x51,
    0x10, 0x64, 0x11, 0x11, 0x25, 0x41, 0x14, 0x45, 0x41, 0x14, 0x05, 0x51, 0x10, 0x44, 0x11, 0x11,
    0x45, 0x41, 0x14, 0x45, 0x5E, 0xF4, 0x78, 0xCF, 0x17, 0x38, 0x11, 0xE1, 0x44, 0x5F, 0x14, 0x39,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F, 0xF3, 0x78, 0x5F,
    0x14, 0x45, 0x51, 0xF4, 0x39, 0x82, 0x43, 0x00, 0x51, 0x14, 0x05, 0x44, 0x14, 0x45, 0x51, 0x04,
    0x09, 0x02, 0xA2, 0x00, 0x51, 0x14, 0x05, 0x44, 0x14, 0x45, 0x4A, 0x84, 0x08, 0x04, 0x12, 0x01,
    0x4F, 0x94, 0x38, 0x44, 0xA4, 0x54, 0x84, 0x42, 0x08, 0x04, 0x02, 0x00, 0x41, 0x75, 0x40, 0x44,
    0xA4, 0x54, 0x0A, 0x21, 0x08, 0x04, 0x02, 0x00, 0x41, 0x92, 0x40, 0x44, 0x44, 0x28, 0x11, 0x11,
    0x08, 0x08, 0x02, 0x00, 0x81, 0x15, 0x3D, 0x84, 0x43, 0x28, 0x11, 0xF1, 0x39, 0x88, 0x03, 0x7C,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x10,
    0xC0, 0x00, 0x01, 0x81, 0x04, 0x06, 0x00, 0x00, 0x02, 0x10, 0x00, 0x10, 0x20, 0x01, 0x01, 0x00,
    0x04, 0x04, 0x00, 0x00, 0x84, 0xD3, 0x78, 0x96, 0x23, 0x58, 0x8F, 0xF1, 0x44, 0xC4, 0xD2, 0x38,
    0x00, 0x34, 0x05, 0x59, 0x74, 0x64, 0x11, 0x81, 0x24, 0x44, 0x35, 0x45, 0x80, 0x17, 0x05, 0xD1,
    0x27, 0x44, 0x11, 0x81, 0x1C, 0x44, 0x15, 0x45, 0x40, 0x14, 0x05, 0x51, 0x20, 0x44, 0x11, 0x81,
    0x24, 0x44, 0x14, 0x45, 0x80, 0xF7, 0x78, 0x9E, 0x27, 0x78, 0xD1, 0x87, 0x44, 0x5F, 0x14, 0x39,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x44, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x60,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x84, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x04, 0x01, 0x7C, 0x8D, 0xD7, 0x78, 0x47, 0x14, 0x45, 0x51, 0xF4, 0x11, 0x04, 0x21, 0x54,
    0x53, 0x34, 0x05, 0x42, 0x14, 0x55, 0x4A, 0x84, 0x08, 0x04, 0x52, 0x6D, 0x51, 0x14, 0x38, 0x42,
    0xA4, 0x54, 0x84, 0x42, 0x10, 0x04, 0x81, 0x54, 0x51, 0x16, 0x40, 0x42, 0xA6, 0x28, 0x8A, 0x22,
    0x10, 0x04, 0x01, 0x7C, 0x8F, 0x15, 0x3C, 0x9C, 0x45, 0x28, 0x11, 0xF1, 0x21, 0x84, 0x00, 0x7C,
    0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

typedef struct Ui_Vertex {
    __m128  dst;
    __m128  src;
    __m128i color;
    __m128i info; /* x: use_texture, y: unused, z: unused, w: unused */
} Ui_Vertex;

typedef struct Ui_Renderer {
    Gpu_Buffer          gpu_buffer;
    Gpu_Buffer_Range    gpu_vertex_buffer;
    Gpu_Pipeline_State  pipeline_state;
    Gpu_Buffer_Range    gpu_standalone_font_buffer;
    Gpu_Texture        *gpu_standalone_font_texture;
    
    Ui_Vertex *vertex_buffer;
    size_t     num_vertices;
    M4         view_projection;
} Ui_Renderer;

/* Shaders */
#define GLSL(code) #code
static char UI_RENDERER_VERTEX_SHADER_SRC[] = "#version 450\n"
GLSL(
layout(location = 0) in vec4  im2vs_src;          \n
layout(location = 1) in vec4  im2vs_dst;          \n
layout(location = 2) in uvec4 im2vs_color;        \n
layout(location = 3) in ivec4 im2vs_info;         \n
layout(location = 0) uniform mat4 mvp;            \n
out      vec4 fs_input_color;                     \n
out      vec4 vs2fs_texcoord0;                    \n
out flat int  vs2fs_use_texture;                  \n
void main() {                                     \n
    vec2 position = vec2(                         \n
       im2vs_src[(gl_VertexID & 1) << 1],         \n
       im2vs_src[(gl_VertexID & 2) | 1]);         \n
    gl_Position    = mvp * vec4(position, 0., 1.);\n
    vec2 texcoord0 = vec2(                        \n
       im2vs_dst[(gl_VertexID & 1) << 1],         \n
       im2vs_dst[(gl_VertexID & 2) | 1]);         \n
    vs2fs_texcoord0 = vec4(texcoord0, 0., 0.);    \n
    uint color32 = im2vs_color[gl_VertexID];      \n
    float inv_255 = 1./255.;                      \n
    float r = ((color32 >>  0) & 0xFF) * inv_255; \n
    float g = ((color32 >>  8) & 0xFF) * inv_255; \n
    float b = ((color32 >> 16) & 0xFF) * inv_255; \n
    float a = ((color32 >> 24) & 0xFF) * inv_255; \n
    fs_input_color = vec4(r, g, b, a);            \n
    vs2fs_use_texture = im2vs_info.r;             \n
});
static char UI_RENDERER_FRAGMENT_SHADER_SRC[] = "#version 450\n"
GLSL(
/* Fragment Shader START */
in      vec4 fs_input_color;                                             \n
in      vec4 vs2fs_texcoord0;                                            \n
in flat int  vs2fs_use_texture;                                          \n
layout(location = 0) out vec4 fs2om_color;                               \n
layout(binding = 1) uniform sampler2D tex0;                              \n
void main() {                                                            \n
    if(bool(vs2fs_use_texture)) {                                          \n
        vec4 color = texture(tex0, vs2fs_texcoord0.xy).r * fs_input_color;\n
       if(color.a <= 0.0001) \n
           discard; \n
        fs2om_color = color; \n
    } else                                                                 \n
        fs2om_color = fs_input_color;                                    \n
});
#undef GLSL

void ui_renderer_unpack_standalone_ascii_raster_font_to_r8(char *dst) {
    char  *src  = UI_STANDALONE_ASCII_RASTER_FONT_DATA;
    size_t mask = 1;
    for(
        size_t i = 0; 
        i < (UI_STANDALONE_ASCII_RASTER_FONT_WIDTH * UI_STANDALONE_ASCII_RASTER_FONT_HEIGHT);
        ++i
    ) {
        *dst = ((*src) & mask) ? 0xFF : 0x00;
        mask <<= 1;
        if(mask == (1 << 8)) {
            mask = 1;
            src += 1;
        }
        dst += 1;
    }
}

void ui_renderer_init_job(Ui_Renderer *dst, Arena *temp_arena, Gpu_Device *gpu_device) {    
    /* Buffer */
    {
        size_t vertex_buffer_size   = sizeof(Ui_Vertex) * U16_MAX;      
        size_t buffer_size = vertex_buffer_size;
        char *staging_buffer;
        Gpu_Buffer gpu_buffer = gpu_make_buffer_coherent(buffer_size + sizeof(__m128), &staging_buffer);
        ptrdiff_t vertex_buffer_offset = ALIGN(staging_buffer, sizeof(__m128)) - (ptrdiff_t)staging_buffer;
        vertex_buffer_offset = 0;
        /* Export */
        dst->vertex_buffer            = staging_buffer + vertex_buffer_offset;
        dst->gpu_buffer               = gpu_buffer;
        dst->gpu_vertex_buffer.buffer = gpu_buffer;
        dst->gpu_vertex_buffer.offset = vertex_buffer_offset;
        dst->gpu_vertex_buffer.size   = vertex_buffer_size;
    }
    
    /* Texture */
    {
        Arena_Scope scope = arena_scope_begin(temp_arena);
        size_t size = 
            UI_STANDALONE_ASCII_RASTER_FONT_WIDTH * 
            UI_STANDALONE_ASCII_RASTER_FONT_HEIGHT;
        char *data = ARENA_INSERT_ARRAY(temp_arena, char, size);
        ui_renderer_unpack_standalone_ascii_raster_font_to_r8(data);
        Gpu_Texture *font = gpu_create_texture_from_memory_blocking(
            UI_STANDALONE_ASCII_RASTER_FONT_WIDTH,
            UI_STANDALONE_ASCII_RASTER_FONT_HEIGHT,
            GPU_TEXTURE_FORMAT_R8,
            GPU_TEXTURE_FILTERING_NEAREST,
            data
        );
        arena_scope_end(scope);

        dst->gpu_standalone_font_texture = font;
    }

    /* Pipeline State */
    {
        Gpu_Vertex_Attribute vertex_attributes[4] = {0};
        vertex_attributes[0].format          = GPU_FORMAT_V4F32;
        vertex_attributes[0].offset          = &((Ui_Vertex*)0)->src;
        vertex_attributes[0].slot            = 0;
        vertex_attributes[0].is_per_instance = 1;
        vertex_attributes[1].format          = GPU_FORMAT_V4F32;
        vertex_attributes[1].offset          = &((Ui_Vertex*)0)->dst;
        vertex_attributes[1].slot            = 1;
        vertex_attributes[1].is_per_instance = 1;
        vertex_attributes[2].format          = GPU_FORMAT_V4C32;
        vertex_attributes[2].offset          = &((Ui_Vertex*)0)->color;
        vertex_attributes[2].slot            = 2;
        vertex_attributes[2].is_per_instance = 1;
        vertex_attributes[3].format          = GPU_FORMAT_V4I32;
        vertex_attributes[3].offset          = &((Ui_Vertex*)0)->info;
        vertex_attributes[3].slot            = 3;
        vertex_attributes[3].is_per_instance = 1;
        
        Gpu_Pipeline_State_Desc desc = {0};
        desc.num_vertex_attributes  = sizeof(vertex_attributes) / sizeof(vertex_attributes[0]);
        desc.vertex_attributes      = vertex_attributes;
        desc.vertex_shader.data     = UI_RENDERER_VERTEX_SHADER_SRC;
        desc.vertex_shader.length   = sizeof(UI_RENDERER_VERTEX_SHADER_SRC);
        desc.fragment_shader.data   = UI_RENDERER_FRAGMENT_SHADER_SRC;
        desc.fragment_shader.length = sizeof(UI_RENDERER_FRAGMENT_SHADER_SRC);

        desc.output_merger_state.depth_test_enabled   = 0;
        desc.output_merger_state.depth_write_disabled = 1;
        desc.output_merger_state.blend_enabled        = 1;
        desc.output_merger_state.blend_src            = GPU_BLEND_SRC_ALPHA;
        desc.output_merger_state.blend_dst            = GPU_BLEND_ONE_MINUS_SRC_ALPHA;

        /* Export */
        dst->pipeline_state = gpu_make_pipeline_state(gpu_device, &desc);
    }
}

void ui_renderer_reserve(Ui_Vertex **vertices, Ui_Renderer *renderer, size_t num_vertices) {
    *vertices = &renderer->vertex_buffer[renderer->num_vertices];
    renderer->num_vertices += num_vertices;
}

Ui_Aabb2 ui_aabb_inset(Ui_Aabb2 aabb, F32 inset) {
    aabb.x0 += inset;
    aabb.y0 += inset;
    aabb.x1 -= inset;
    aabb.y1 -= inset;
    return aabb;
}

void ui_renderer_aabb(Ui_Renderer *renderer, Ui_Aabb2 aabb, C32 color) {
    Ui_Vertex *vertices;
    ui_renderer_reserve(&vertices, renderer, 1);
    vertices[0].src   = _mm_set_ps(aabb.y1, aabb.x1, aabb.y0, aabb.x0);
    vertices[0].dst   = _mm_set_ps(1, 1, 0, 0);
    vertices[0].color = _mm_set1_epi32(color);
    vertices[0].info.m128i_i32[0]  = 0;
}

void ui_renderer_aabb_multicolor(Ui_Renderer *renderer, Ui_Aabb2 aabb, C32 color[4]) {
    Ui_Vertex *vertices;
    ui_renderer_reserve(&vertices, renderer, 1);
    vertices[0].src   = _mm_set_ps(aabb.y1, aabb.x1, aabb.y0, aabb.x0);
    vertices[0].dst   = _mm_set_ps(1, 1, 0, 0);
    vertices[0].color = _mm_set_epi32(color[3], color[2], color[1], color[0]);
    vertices[0].info.m128i_i32[0]  = 0;
}

void ui_renderer_point(Ui_Renderer *renderer, V2 position, C32 color) {
    F32 half_size = 2.f;
    Ui_Vertex *vertices;
    ui_renderer_reserve(&vertices, renderer, 1);
    vertices[0].src   = _mm_set_ps(position.y + half_size, position.x + half_size, position.y - half_size, position.x - half_size);
    vertices[0].dst   = _mm_set_ps(1, 1, 0, 0);
    vertices[0].color = _mm_set1_epi32(color);
    vertices[0].info.m128i_i32[0]  = 0;
}

void ui_renderer_img(Ui_Renderer *renderer, Ui_Aabb2 dst, Ui_Aabb2 src,C32 color) {
    Ui_Vertex *vertices;
    ui_renderer_reserve(&vertices, renderer, 1);
    vertices[0].src   = _mm_set_ps(dst.y1, dst.x1, dst.y0, dst.x0);
    vertices[0].dst   = _mm_set_ps(src.y1, src.x1, src.y0, src.x0);
    vertices[0].color = _mm_set1_epi32(color);
    vertices[0].info.m128i_i32[0] = 1;
}

Ui_Aabb2 ui_aabb_from_anchor(Ui_Aabb2 parent_aabb, U8 anchor, V2 size) {
    Ui_Aabb2 aabb;
    switch(anchor) {
        case 0: { /* right */
            aabb.x0 = parent_aabb.x1 - size.x;
            aabb.y0 = parent_aabb.y0;
            aabb.x1 = parent_aabb.x1;
            aabb.y1 = parent_aabb.y1;
        } break;
        case 1: { /* up */
            aabb.x0 = parent_aabb.x0;
            aabb.y0 = parent_aabb.y1 - size.y;
            aabb.x1 = parent_aabb.x1;
            aabb.y1 = parent_aabb.y1;
        } break;
        case 2: { /* down */
            aabb.x0 = parent_aabb.x0;
            aabb.y0 = parent_aabb.y0;
            aabb.x1 = parent_aabb.x1;
            aabb.y1 = parent_aabb.y0 + size.y;
        } break;
        case 3: { /* left */
            aabb.x0 = parent_aabb.x0;
            aabb.y0 = parent_aabb.y0;
            aabb.x1 = parent_aabb.x0 + size.x;
            aabb.y1 = parent_aabb.y1;
        } break;
    }
    return aabb;
}

V2 ui_node_size(Ui_Node *node) {
    V2 size;    
    size.x = lerp(node->content_num_px_max.x, node->content_num_px_sum.x, fabsf(node->layout_direction.x));
    size.y = lerp(node->content_num_px_max.y, node->content_num_px_sum.y, fabsf(node->layout_direction.y));
    size.x = fmaxf(size.x, node->size.x);
    size.y = fmaxf(size.y, node->size.y);
    if(node->name.length != 0) {
        size.x = fmaxf(node->size.x, node->text_padding * 2 + node->name.length * UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH_SCALED);
        size.y = fmaxf(node->size.y, UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_HEIGHT_SCALED);
    }
    size.x += node->padding[0] + node->padding[2];
    size.y += node->padding[1] + node->padding[3];
    return size;
}

void ui_renderer_clear_buffers_job(Ui_Renderer *renderer) {
    renderer->num_vertices = 0;
}

V2 ui_renderer_emplace_center(V2 cursor, Ui_Node *node, V2 *center, F32 *previous_align_main) {

    F32 ignore_layout = node->ignore_layout;
    //ignore_layout = 0;

    V2 cursor_offset = V2_ZERO;

    V2 size = ui_node_size(node);

    /* Main alignment indicates if the ui element should be inserted at the begining or the end of the layout
        * relative to it's parent. */
    float delta_align_main = node->align_main - *previous_align_main;
    delta_align_main = fmaxf(delta_align_main, 0);
    *previous_align_main += delta_align_main; /* /!\ */

    /* Advance by some amount of empty space, dicatated by the main alignment. */
    cursor_offset.x += node->parent->layout_direction.x * node->parent->empty.x * delta_align_main;
    cursor_offset.y += node->parent->layout_direction.y * node->parent->empty.y * delta_align_main;

    V2 local_cursor = v2_add(cursor, cursor_offset);

    /* Locally compensate for any layout ignoring going on. This helps the ui element stay in palce. */
    local_cursor.x -= size.x * node->parent->layout_direction.x * node->align_main * ignore_layout;
    local_cursor.y -= size.y * node->parent->layout_direction.y * node->align_main * ignore_layout;
    
    V2 pivot;
    pivot.x = local_cursor.x + size.x * 0.5f * node->parent->layout_direction.x;
    pivot.y = local_cursor.y + size.y * 0.5f * node->parent->layout_direction.y;

    V2 align_sub;
    align_sub.x = lerp(node->align_sub, 0.5, fabsf(node->parent->layout_direction.x));
    align_sub.y = lerp(node->align_sub, 0.5, fabsf(node->parent->layout_direction.y));

    /* Use the sub-alignment to control how the ui element align on the axis ortogonal to the layout direction. */
    V2 size_content_max_halved = v2_mul_f32(node->parent->content_num_px_max, 0.5f);
    Ui_Aabb2 aabb;
    aabb.x0 = lerp(pivot.x - size_content_max_halved.x, pivot.x + size_content_max_halved.x - size.x, align_sub.x);
    aabb.y0 = lerp(pivot.y - size_content_max_halved.y, pivot.y + size_content_max_halved.y - size.y, align_sub.y);
    aabb.x1 = lerp(pivot.x - size_content_max_halved.x + size.x, pivot.x + size_content_max_halved.x, align_sub.x);
    aabb.y1 = lerp(pivot.y - size_content_max_halved.y + size.y, pivot.y + size_content_max_halved.y, align_sub.y);

    //text_aabb = aabb;

    #if 0
    if(fabsf(node->parent->layout_direction.x) > fabsf(node->parent->layout_direction.y)) {
        aabb->y0 = node->parent->aabb.y0;
        aabb->y1 = node->parent->aabb.y1;
    } else {
        aabb->x0 = node->parent->aabb.x0;
        aabb->x1 = node->parent->aabb.x1;
    }
    #endif
    
    // aabb.x0 += node->padding[0];
    // aabb.y0 += node->padding[1];
    // aabb.x1 -= node->padding[2];
    // aabb.y1 -= node->padding[3];

    //text_aabb.x0 += node->padding[0];
    //text_aabb.y0 += node->padding[1];
    //text_aabb.x1 -= node->padding[2];
    //text_aabb.y1 -= node->padding[3];

    /* The folowing ignore layout parameter allows the node to not affect the layout cursor. */
    
    /* Advance the cursor by the size of the ui element we just created. */
    cursor_offset.x += size.x * node->parent->layout_direction.x * (1.f - ignore_layout);
    cursor_offset.y += size.y * node->parent->layout_direction.y * (1.f - ignore_layout);

    /* Advance the cursor by the space between childs. */
    cursor_offset.x += node->parent->child_spacing.x * node->parent->layout_direction.x * (1.f - ignore_layout) * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;
    cursor_offset.y += node->parent->child_spacing.y * node->parent->layout_direction.y * (1.f - ignore_layout) * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;
    
    V2 drop_center = v2(
        (aabb.x0 + aabb.x1) * 0.5f,
        (aabb.y0 + aabb.y1) * 0.5f
    );
    *center = drop_center;
    return cursor_offset;
}

V2 ui_renderer_emplace(V2 cursor, Ui_Node *node, Ui_Aabb2 *aabb, F32 *empty_consumed) {

    F32 ignore_layout = node->ignore_layout;
    //ignore_layout = 0;

    V2 cursor_offset = V2_ZERO;

    V2 size = ui_node_size(node);

    /* Main alignment indicates if the ui element should be inserted at the begining or the end of the layout
        * relative to it's parent. */
    F32 align_main = node->align_main;
    float delta_align_main = node->align_main - *empty_consumed;
    delta_align_main = fmaxf(delta_align_main, 0);
    *empty_consumed += delta_align_main; /* /!\ */

    /* Advance by some amount of empty space, dicatated by the main alignment. */
    cursor_offset.x += node->parent->layout_direction.x * node->parent->empty.x * delta_align_main;
    cursor_offset.y += node->parent->layout_direction.y * node->parent->empty.y * delta_align_main;

    V2 local_cursor = v2_add(cursor, cursor_offset);

    /* Locally compensate for any layout ignoring going on. This helps the ui element stay in palce. */
    //local_cursor.x -= size.x * node->parent->layout_direction.x * node->align_main * ignore_layout;
    //local_cursor.y -= size.y * node->parent->layout_direction.y * node->align_main * ignore_layout;
    
    V2 pivot;
    pivot.x = local_cursor.x + size.x * 0.5f * node->parent->layout_direction.x;
    pivot.y = local_cursor.y + size.y * 0.5f * node->parent->layout_direction.y;

    V2 align_sub;
    align_sub.x = lerp(node->align_sub, 0.5, fabsf(node->parent->layout_direction.x));
    align_sub.y = lerp(node->align_sub, 0.5, fabsf(node->parent->layout_direction.y));

    /* Use the sub-alignment to control how the ui element align on the axis ortogonal to the layout direction. */
    V2 size_content_max_halved = v2_mul_f32(node->parent->content_num_px_max, 0.5f);
    aabb->x0 = lerp(pivot.x - size_content_max_halved.x, pivot.x + size_content_max_halved.x - size.x, align_sub.x);
    aabb->y0 = lerp(pivot.y - size_content_max_halved.y, pivot.y + size_content_max_halved.y - size.y, align_sub.y);
    aabb->x1 = lerp(pivot.x - size_content_max_halved.x + size.x, pivot.x + size_content_max_halved.x, align_sub.x);
    aabb->y1 = lerp(pivot.y - size_content_max_halved.y + size.y, pivot.y + size_content_max_halved.y, align_sub.y);

    //text_aabb = aabb;

    #if 0
    if(fabsf(node->parent->layout_direction.x) > fabsf(node->parent->layout_direction.y)) {
        aabb->y0 = node->parent->aabb.y0;
        aabb->y1 = node->parent->aabb.y1;
    } else {
        aabb->x0 = node->parent->aabb.x0;
        aabb->x1 = node->parent->aabb.x1;
    }
    #endif
    
    aabb->x0 += node->padding[0];
    aabb->y0 += node->padding[1];
    aabb->x1 -= node->padding[2];
    aabb->y1 -= node->padding[3];

    //text_aabb.x0 += node->padding[0];
    //text_aabb.y0 += node->padding[1];
    //text_aabb.x1 -= node->padding[2];
    //text_aabb.y1 -= node->padding[3];

    /* The folowing ignore layout parameter allows the node to not affect the layout cursor. */
    
    /* Advance the cursor by the size of the ui element we just created. */
    cursor_offset.x += size.x * node->parent->layout_direction.x * (1.f - ignore_layout);
    cursor_offset.y += size.y * node->parent->layout_direction.y * (1.f - ignore_layout);

    /* Advance the cursor by the space between childs. */
    cursor_offset.x += node->parent->child_spacing.x * node->parent->layout_direction.x * (1.f - ignore_layout) * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;
    cursor_offset.y += node->parent->child_spacing.y * node->parent->layout_direction.y * (1.f - ignore_layout) * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;
    
    return cursor_offset;
}

// void ui_renderer_populate_buffers_common_job() {

// }

typedef struct Ui_Renderer_Populate_Buffers_Job {
    Ui_Renderer          *renderer;
    size_t                num_roots;
    Ui_Node             **roots;
    Ui_Aabb2             *root_aabbs;
    Ui_Drag              *drag;
    Ui_Context_Menu_Flip *context_menu_flip;
    V2                    context_menu_anchor;
    Ui_Shared            *shared;
    Ui_Aabb2              context_menu_containing_extents;
} Ui_Renderer_Populate_Buffers_Job;
void ui_renderer_populate_buffers_job_validate(Ui_Renderer_Populate_Buffers_Job *job) {
    ASSERT(job->renderer);
    ASSERT(job->num_roots == 0 || job->roots, "You forgot to specify set roots.");
    ASSERT(job->num_roots == 0 || job->root_aabbs, "You forgot to specify set root_aabbs.");
    ASSERT(job->drag);
    ASSERT(job->context_menu_flip);
}
void ui_renderer_populate_buffers_job(Ui_Renderer_Populate_Buffers_Job *job) { //Ui_Renderer *renderer, Ui_Node *root/*, Ui *ui*/) {
    job->drag->num_drop_targets = 0;
    Ui_Renderer *renderer = job->renderer;
    for (size_t root_index = 0; root_index != job->num_roots; ++root_index) {
        Ui_Node *root      = job->roots     [root_index];
        Ui_Aabb2 root_aabb = job->root_aabbs[root_index];
        if(!root)
            continue;
        /* First pass */
        {
            for(
                Ui_Node *node = ui_node_traversal_depth_first_post_order_start(root); 
                node;
                node = node == root ? 0 : ui_node_traversal_depth_first_post_order_next(node)
            ) {
                if(node->kind == UI_NODE_KIND_BAR)
                    continue;
                /* Accumulate content size */
                if(node->parent) {
                    Ui_Node *parent = node->parent;
                    V2 size = ui_node_size(node);
                    size.x = size.x * (1.f - node->ignore_layout * fabsf(parent->layout_direction.x));
                    size.y = size.y * (1.f - node->ignore_layout * fabsf(parent->layout_direction.y));
                    parent->content_num_px_sum.x += size.x;
                    parent->content_num_px_sum.y += size.y;
                    if(node->previous) {
                        parent->content_num_px_sum.x += parent->child_spacing.x * (1.f - node->previous->ignore_layout) * parent->layout_direction.x * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;
                        parent->content_num_px_sum.y += parent->child_spacing.y * (1.f - node->previous->ignore_layout) * parent->layout_direction.y * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;
                    }
                    parent->content_num_px_max.x  = fmaxf(parent->content_num_px_max.x, size.x);
                    parent->content_num_px_max.y  = fmaxf(parent->content_num_px_max.y, size.y);
                }
            }
        }

        /* Second pass */
        for(Ui_Node *node = root; node;) {
            V2           size = ui_node_size(node);
            Ui_Aabb2     aabb;
            Ui_Aabb2     text_aabb;
            Ui_Node_Kind kind;
            {
                if(!node->parent) {
                    if(node->kind == UI_NODE_KIND_CONTEXT_MENU) {
                        kind = UI_NODE_KIND_CONTEXT_MENU;
                    } else if(node == root)
                        kind = UI_NODE_KIND_ROOT;
                    else {
                        ASSERT(node->kind == UI_NODE_KIND_ROOT, "A regular node can't be a root.");
                        kind = node->kind;
                    }
                } else if(node->parent == root) {
                    if(node->kind == UI_NODE_KIND_BAR) {
                        kind = UI_NODE_KIND_BAR;
                    } else if (node->kind == UI_NODE_KIND_CONTEXT_MENU) {
                        kind = UI_NODE_KIND_CONTEXT_MENU;
                    } else {
                        kind = UI_NODE_KIND_REGULAR;
                    }
                } else {
                    kind = UI_NODE_KIND_REGULAR;
                }
            }
            switch(kind) {
                case UI_NODE_KIND_ROOT: {
                    // aabb.x0 = 0;
                    // aabb.y0 = 0;
                    // aabb.x1 = node->size.x;
                    // aabb.y1 = node->size.y;

                    aabb = job->root_aabbs[root_index];
                } break;
                case UI_NODE_KIND_BAR: {
                    ASSERT(node->parent);
                    aabb = ui_aabb_from_anchor(node->parent->aabb, node->as.bar.anchor, size);
                    /* If dragging node. */
                    if(node->hash == job->drag->active || node->hash == job->drag->last) {
                        Ui_Aabb2 aabb_drop_preview = ui_aabb_from_anchor(node->parent->aabb, job->drag->anchor, v2(16, 16));
                        C32 colors[] = {
                            C32_BLACK, node->corner_colors[1], C32_BLACK, node->corner_colors[3],
                            C32_BLACK, C32_BLACK, node->corner_colors[2], node->corner_colors[3],
                            node->corner_colors[0], node->corner_colors[1], C32_BLACK, C32_BLACK,
                            node->corner_colors[0], C32_BLACK, node->corner_colors[2], C32_BLACK
                        };
                        C32 *color =  &colors[job->drag->anchor << 2];
                        ui_renderer_aabb_multicolor(renderer, ui_aabb_inset(aabb_drop_preview, 1), color);

                        V2 drag_delta = job->drag->delta;
                        F32 anim = fminf(job->drag->since_begin, 0.125f) * 8.f;
                        anim = ease_out(anim);
                        anim = 1.0f;
                        if(node->hash != job->drag->active) {
                            anim = 1.f - fminf(job->drag->since_end, 0.25f) * 4.f;
                            anim = ease_in(anim);
                            anim = 0.0f;
                        }

                        aabb.x0 += drag_delta.x * anim;
                        aabb.y0 += drag_delta.y * anim;
                        aabb.x1 += drag_delta.x * anim;
                        aabb.y1 += drag_delta.y * anim; 
                        /* todo: keep a ptr to the dragged node and render it at the end (so it's on top) */
                    }

                    size.x = aabb.x1 - aabb.x0;
                    size.y = aabb.y1 - aabb.y0;
                } break;
                case UI_NODE_KIND_CONTEXT_MENU: {
                    V2 anchor = job->context_menu_anchor;

                    size.x = lerp(node->content_num_px_max.x, node->content_num_px_sum.x, fabsf(node->layout_direction.x));
                    size.y = lerp(node->content_num_px_max.y, node->content_num_px_sum.y, fabsf(node->layout_direction.y));
                    size.x = fmaxf(size.x, node->size.x);
                    size.y = fmaxf(size.y, node->size.y);

                    aabb.x0 = anchor.x;
                    aabb.y0 = anchor.y - size.y;
                    aabb.x1 = anchor.x + size.x;
                    aabb.y1 = anchor.y;

                    if(aabb.x1 > job->context_menu_containing_extents.x1) {
                        job->context_menu_flip->x = 1;
                    }
                    if(aabb.y0 < 0) {
                        job->context_menu_flip->y = 1;
                    }
                    if(job->context_menu_flip->x) {
                        aabb.x0 = anchor.x - size.x;
                        aabb.x1 = anchor.x;
                    }
                    if(job->context_menu_flip->y) {
                        aabb.y0 = anchor.y;
                        aabb.y1 = anchor.y + size.y;
                    }
                } break;
                case UI_NODE_KIND_DROPDOWN_MENU: {
                    ASSERT(node->previous, "This node should have a previous.");
                    Ui_Aabb2 parent_aabb = node->previous->aabb;
                    aabb.x0 = parent_aabb.x0;
                    aabb.y0 = parent_aabb.y0 - size.y;
                    aabb.x1 = parent_aabb.x0 + size.x;
                    aabb.y1 = parent_aabb.y0;
                } break;
                case UI_NODE_KIND_REGULAR: {
                    ASSERT(node->parent, "This node should have a parent.");

                    //V2 cursor_offset = V2_ZERO;
                    V2 drop_preview_cursor_offset = V2_ZERO;

                    if(job->drag->active != UI_HASH_INVALID) {
                        Ui_Node *dragged = ui_shared_get_node(job->shared, job->drag->active);
                        if(node->parent == dragged->parent) {
                            Ui_Node *parent = dragged->parent;
                            /* parallel universe */
                            V2 parallel_universe_cursor;
                            if (!node->previous) {
                                ASSERT(node == node->parent->first);
                                parallel_universe_cursor = parent->cursor;
                                job->drag->drop_preview_previous_normalized_lerp = 1;

                                job->drag->drop_preview_empty_consumed = 0;
                            } else {
                                parallel_universe_cursor = job->drag->parallel_universe_cursor;
                            }

                            if (node != dragged) {
                                V2 a;
                                V2 b;
                                V2 d;
                                V2 offset = ui_renderer_emplace_center(parallel_universe_cursor, dragged, &a, &job->drag->drop_preview_empty_consumed);
                                drop_preview_cursor_offset = offset;
                                //parallel_universe_cursor = v2_add(parallel_universe_cursor, offset);
                                offset = ui_renderer_emplace_center(parallel_universe_cursor, node, &d, &job->drag->drop_preview_empty_consumed);
                                parallel_universe_cursor = v2_add(parallel_universe_cursor, offset);
                                ui_renderer_emplace_center(parallel_universe_cursor, dragged, &b, &job->drag->drop_preview_empty_consumed);
                                //offset = v2_add(offset, );

                                V2 c = v2(
                                    (dragged->aabb.x0 + dragged->aabb.x1) * 0.5f,
                                    (dragged->aabb.y0 + dragged->aabb.y1) * 0.5f
                                );

                                
                                //debug_segment2(1, a, b, C32_RED & C32_ALPHA_MASK(127));
                                V2 ab = v2_sub(b, a);
                                V2 nab = v2_normalize(ab);
                                V2 ac  = v2_sub(c, a);
                                F32 dot = v2_dot(nab, ac);
                                F32 gfx;// = 1.0f - fminf(drop_preview_distance, 1.0f);
                                gfx = dot / v2_magnitude(ab);
                                F32 previous = job->drag->drop_preview_previous_normalized_lerp;
                                job->drag->drop_preview_previous_normalized_lerp = gfx;
                                if(gfx > 0) {
                                    gfx = 1.f - fminf(fmaxf(gfx, 0), 1);
                                } else {
                                    gfx = fminf(fmaxf(previous, 0), 1);
                                }
                                gfx = ease_in_out(gfx);
                                //gfx = 1.f - fminf(fmaxf(dot / v2_magnitude(ab), 0), 1);
                                //gfx = 1.0f - gfx;
                                //gfx = gfx + (1.0f - previous);

                                //F32 drop_preview_distance = v2_distance(drop_center, job->shared->mouse_position) * (1.0f / 32.0f);
                                
                                //debug_aabb(*(V4*)&drop_aabb, gfx > 0 ? C32_GREEN : C32_RED);

                                //cursor_offset = v2_add(cursor_offset, v2_mul_f32(drop_preview_cursor_offset, gfx));
                                drop_preview_cursor_offset = v2_mul_f32(drop_preview_cursor_offset, gfx);


                                


                                // V2 dragged_position =
                                //     v2(
                                //         (dragged->aabb.x0 + dragged->aabb.x1) * 0.5f,
                                //         (dragged->aabb.y0 + dragged->aabb.y1) * 0.5
                                //     );
                                // V2 drop_target_position = v2((drop_aabb.x0 + drop_aabb.x1) * 0.5f, (drop_aabb.y0 + drop_aabb.y1) * 0.5);
                                // if(job->drag->num_drop_targets != UI_DRAG_NUM_DROP_TARGETS_MAX) {
                                //     job->drag->drop_targets[job->drag->num_drop_targets].position = drop_target_position;
                                //     job->drag->drop_targets[job->drag->num_drop_targets].hash     = node->hash;
                                //     ++job->drag->num_drop_targets;
                                // } else {
                                //     /* replace nearest drop location */
                                //     size_t nearest_drop_target_index = 0;
                                //     V2 nearest_drop_target_position = job->drag->drop_targets[job->drag->num_drop_targets].position;
                                //     for (
                                //         size_t drop_target_index = 1;
                                //         drop_target_index < job->drag->num_drop_targets;
                                //         ++drop_target_index
                                //     ) {
                                //         V2 other_drop_target_position = job->drag->drop_targets[drop_target_index].position;
                                //         if(
                                //             v2_distance_squared(other_drop_target_position, dragged_position) <
                                //             v2_distance_squared(nearest_drop_target_position, dragged_position)
                                //         ) {
                                //             nearest_drop_target_index    = drop_target_index;
                                //             nearest_drop_target_position = other_drop_target_position;
                                //         }
                                //     }
                                //     if(
                                //         v2_distance_squared(drop_target_position, dragged_position) <
                                //         v2_distance_squared(nearest_drop_target_position, dragged_position)
                                //     ) {
                                //         job->drag->drop_targets[job->drag->num_drop_targets].position = drop_target_position;
                                //         job->drag->drop_targets[job->drag->num_drop_targets].hash     = node->hash;
                                //     }
                                // }
                                
                                // // cursor = v2_add(cursor, parallel_universe_cursor_offset);
                                
                                // float drop_target_weight = 0.f;
                                // //if (job->drag->num_drop_targets == 2) {
                                // //    V2 a = 
                                // //}
                                // for (
                                //     size_t drop_target_index = 0;
                                //     drop_target_index != job->drag->num_drop_targets;
                                //     ++drop_target_index
                                // ) {
                                //     Ui_Drag_Drop_Target drop_target = job->drag->drop_targets[job->drag->num_drop_targets];
                                //     if(drop_target.hash == node->hash) {
                                //         debug_aabb(*(V4*)&drop_aabb, C32_RED);
                                //         break;
                                //     }
                                // }
                                // //job->drag->parallel_universe_cursor = v2_add(parallel_universe_cursor, parallel_universe_cursor_offset);
                            }
                        }
                    }

                    V2 layout_cursor  = v2_add(node->parent->cursor, drop_preview_cursor_offset);
                    V2 emplace_cursor = layout_cursor;
                    if(job->drag->active == node->hash) {
                        if(!node->previous) {
                            emplace_cursor = layout_cursor;
                        } else {
                            emplace_cursor = job->drag->parallel_universe_cursor;
                        }
                    }

                    V2 emplace_cursor_offset = ui_renderer_emplace(emplace_cursor, node, &aabb, &node->parent->empty_consumed);
                    text_aabb = aabb;
                    #if 1
                    if(fabsf(node->parent->layout_direction.x) > fabsf(node->parent->layout_direction.y)) {
                        aabb.y0 = node->parent->aabb.y0;
                        aabb.y1 = node->parent->aabb.y1;
                    } else {
                        aabb.x0 = node->parent->aabb.x0;
                        aabb.x1 = node->parent->aabb.x1;
                    }
                    #endif
                    
                    if(job->drag->active != UI_HASH_INVALID) {
                        Ui_Node *dragged = ui_shared_get_node(job->shared, job->drag->active);
                        if(node->parent == dragged->parent) {
                            if (!node->previous) {
                                ASSERT(node == node->parent->first);
                                job->drag->parallel_universe_cursor = node->parent->cursor;
                            }
                            if (node != dragged) {
                                job->drag->parallel_universe_cursor = v2_add(job->drag->parallel_universe_cursor, emplace_cursor_offset);
                            } else {
                                emplace_cursor_offset = v2_mul_f32(emplace_cursor_offset, 0);
                            }
                        }
                    }
                    node->parent->cursor = v2_add(layout_cursor, emplace_cursor_offset);
                } break;
            }
            
            /* Translate */
            {
                aabb.x0 += node->translate.x;
                aabb.y0 += node->translate.y;
                aabb.x1 += node->translate.x;
                aabb.y1 += node->translate.y;

                text_aabb.x0 += node->translate.x;
                text_aabb.y0 += node->translate.y;
                text_aabb.x1 += node->translate.x;
                text_aabb.y1 += node->translate.y;
            }

            if(node->first) {
                Ui_Aabb2 parent_aabb = aabb;
                parent_aabb.x0 += node->padding[0];
                parent_aabb.y0 += node->padding[1];
                parent_aabb.x1 -= node->padding[2];
                parent_aabb.y1 -= node->padding[3];

                size.x = fmaxf(size.x, parent_aabb.x1 - parent_aabb.x0);
                size.y = fmaxf(size.y, parent_aabb.y1 - parent_aabb.y0);
                node->empty = v2_sub(size, node->content_num_px_sum);
                node->empty.x = fmaxf(node->empty.x, 0);
                node->empty.y = fmaxf(node->empty.y, 0);
                node->empty_consumed = 0;

                V2 cursor;
                cursor.x = lerp(parent_aabb.x1, parent_aabb.x0, 0.5f);
                cursor.y = lerp(parent_aabb.y1, parent_aabb.y0, 0.5f);
                cursor.x -= node->content_num_px_sum.x * node->layout_direction.x * 0.5f;
                cursor.y -= node->content_num_px_sum.y * node->layout_direction.y * 0.5f;
                #if 0
                cursor.x -= node->empty.x * node->layout_direction.x * 0.5f;
                cursor.y -= node->empty.y * node->layout_direction.y * 0.5f;
                #else
                cursor.x -= node->empty.x * node->layout_direction.x * 0.5f;
                cursor.y -= node->empty.y * node->layout_direction.y * 0.5f;
                if(node == root) {

                }   
                {
                    V2  prefered_size     = ui_node_size(node);
                    F32 effective_size_x  = fmaxf(prefered_size.x, parent_aabb.x1 - parent_aabb.x0);
                    F32 effective_size_y  = fmaxf(prefered_size.y, parent_aabb.y1 - parent_aabb.y0);
                    F32 empty_x = effective_size_x - prefered_size.x;
                    //size.x = lerp(node->content_num_px_max.x, node->content_num_px_sum.x, fabsf(node->layout_direction.x));
                    //size.y = lerp(node->content_num_px_max.y, node->content_num_px_sum.y, fabsf(node->layout_direction.y));
                    cursor.x -= empty_x * fabsf(node->layout_direction.y) * lerp(+0.5f, -0.5, node->align_sub);
                }
                    //F32 empty_x = size.x - node->content_num_px_max.x;
                //cursor.y -= node->empty.y * node->layout_direction.y * 0.5f;
                #endif

                node->cursor = cursor;

                //text_aabb = aabb;
            }
            
            node->aabb = aabb;
            
            if(node->skip_draw) {
                goto SKIP_RENDERING_NODE_AND_SUBTREE;
            }

            {
                /* aabb rendering */
                if(!node->skip_draw_background) {
                    aabb.x0 -= node->outset[0] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;
                    aabb.y0 -= node->outset[1] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;
                    aabb.x1 += node->outset[2] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;
                    aabb.y1 += node->outset[3] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;

                    if(
                        node->border_widths[0] ||
                        node->border_widths[1] ||
                        node->border_widths[2] ||
                        node->border_widths[3]
                    ) {
                        Ui_Aabb2 aabb_border = aabb;
                        aabb_border.x0 += fminf(node->border_widths[0] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE, 0);
                        aabb_border.y0 += fminf(node->border_widths[1] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE, 0);
                        aabb_border.x1 -= fminf(node->border_widths[2] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE, 0);
                        aabb_border.y1 -= fminf(node->border_widths[3] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE, 0);
                        
                        Ui_Aabb2 aabb_background = aabb;
                        aabb_background.x0 += fmaxf(node->border_widths[0] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE, 0);
                        aabb_background.y0 += fmaxf(node->border_widths[1] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE, 0);
                        aabb_background.x1 -= fmaxf(node->border_widths[2] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE, 0);
                        aabb_background.y1 -= fmaxf(node->border_widths[3] * UI_STANDALONE_ASCII_RASTER_FONT_SCALE, 0);

                        C32 border_color = node->border_colors[0];
                        
                        ui_renderer_aabb(renderer, aabb_border, border_color);
                        ui_renderer_aabb_multicolor(renderer, aabb_background, node->corner_colors);
                    } else {
                        ui_renderer_aabb_multicolor(renderer, aabb, node->corner_colors);
                    }
                }
                /* text rendering */
                if(node->name.length != 0) {
                    Ui_Aabb2 dst;
                    dst = text_aabb;
                    dst.x0 += node->text_padding;
                    dst.y0 = (dst.y0 + dst.y1) * 0.5f - UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_HEIGHT_SCALED + 4 * UI_STANDALONE_ASCII_RASTER_FONT_SCALE;
                    dst.x0 = roundf(dst.x0);
                    dst.y0 = roundf(dst.y0);
                    dst.x1 = dst.x0 + (float)UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH_SCALED;
                    dst.y1 = dst.y0 + (float)UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_HEIGHT_SCALED;
                    for (size_t i = 0; i != node->name.length; ++i) {
                        char c = node->name.data[i];
                        if(c == ' ')
                            goto ADVANCE;
                        Ui_Aabb2 src;
                        size_t y = c / (UI_STANDALONE_ASCII_RASTER_FONT_WIDTH / UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH);
                        size_t x = c %  (UI_STANDALONE_ASCII_RASTER_FONT_WIDTH / UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH);
                        src.x0 = x * ((float)UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH / (float)UI_STANDALONE_ASCII_RASTER_FONT_WIDTH);
                        src.y1 = y * ((float)UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_HEIGHT / (float)UI_STANDALONE_ASCII_RASTER_FONT_HEIGHT);
                        src.x1 = src.x0 + ((float)UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH / (float)UI_STANDALONE_ASCII_RASTER_FONT_WIDTH);
                        src.y0 = src.y1 + ((float)UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_HEIGHT/ (float)UI_STANDALONE_ASCII_RASTER_FONT_HEIGHT);
                        
                        ui_renderer_img(renderer, dst, src, node->text_color);
                        
                        ADVANCE:;
                        dst.x0 += UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH_SCALED;
                        dst.x1 += UI_STANDALONE_ASCII_RASTER_FONT_GLYPH_WIDTH_SCALED;
                    }
                }
            }

            #if 0 /* debug */
            if(0) {
                Ui_Aabb2 test = aabb;
                test.x0 = job->debug_mouse_position.x - 4;
                test.y0 = job->debug_mouse_position.y - 4;
                test.x1 = job->debug_mouse_position.x + 4;
                test.y1 = job->debug_mouse_position.y + 4;
                ui_renderer_aabb(renderer, test, C32_GREEN);
            } else {
                if(ui_aabb_is_point_inside(aabb, job->debug_mouse_position)) {
                    Ui_Aabb2 aabb_border = aabb;
                    aabb_border.x0 += fminf(node->border_widths[0], 0);
                    aabb_border.y0 += fminf(node->border_widths[1], 0);
                    aabb_border.x1 -= fminf(node->border_widths[2], 0);
                    aabb_border.y1 -= fminf(node->border_widths[3], 0);
                    ui_renderer_aabb(renderer, ui_aabb_inset(aabb_border, -1), C32_GREEN);
                }
            }
            #endif
            
            // else {
            //     C32 color[4] = { C32_RED, C32_RED, C32_RED, C32_RED, };
            //     if(node->border_width) {
            //         ui_renderer_aabb(renderer, ui_aabb_inset(aabb, fminf(node->border_width, 0)), C32_WHITE);
            //         ui_renderer_aabb_multicolor(renderer, ui_aabb_inset(aabb, fmaxf(node->border_width, 0)), node->corner_colors);
            //     } else {
            //         C32 color[4] = { C32_RED, C32_RED, C32_RED, C32_RED, };
            //         ui_renderer_aabb_multicolor(renderer, aabb, node->corner_colors);
            //     }
            // }

            SKIP_RENDERING:;
            node = ui_node_traversal_depth_first_pre_order_next(node);
            continue;

            SKIP_RENDERING_NODE_AND_SUBTREE:;
            node = ui_node_traversal_depth_first_pre_order_next_skip_subtree(node);
            continue;
        }
    }
}

void ui_renderer_populate_command_list_job(
    Ui_Renderer *renderer,
    Gpu_Command_List *command_list,
    V2 surface_resolution
) {
    gpu_cmd_set_pipeline_state(
        command_list,
        renderer->pipeline_state
    );
    gpu_cmd_set_topology(
        command_list,
        GPU_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
    );
    gpu_cmd_bind_vertex_buffer_range(
        command_list,
        0,
        renderer->gpu_vertex_buffer.buffer,
        renderer->gpu_vertex_buffer.offset,
        renderer->gpu_vertex_buffer.size,
        sizeof(Ui_Vertex)
    );
    M4 view_projection = m4(
        2.f / surface_resolution.x, 0                         , 0, -1.0f,
        0                         , 2.f / surface_resolution.y, 0, -1.0f,
        0                         , 0                         , 1, 0,
        0                         , 0                         , 0, 1
    );
    gpu_cmd_set_uniform_m4(
        command_list,
        &view_projection,
        0
    );
    gpu_cmd_bind_texture(
       command_list,
       renderer->gpu_standalone_font_texture,
       1
    );
    gpu_cmd_draw_instanced(
        command_list,
        4,
        renderer->num_vertices,
        0,
        0
    );
}
