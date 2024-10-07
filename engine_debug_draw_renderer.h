/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

#define DEBUG_DRAW_RENDERER_NUM_LAYER_MAX 2

typedef struct Debug_Draw_Renderer_Layer {
    Gpu_Buffer_Range   gpu_vertex_buffer;
    Gpu_Buffer_Range   gpu_index_buffer;
    Debug_Draw_Dst     dst;
    M4                 vp;
} Debug_Draw_Renderer_Layer;

typedef struct Debug_Draw_Renderer {
    Gpu_Buffer         gpu_buffer;
    Gpu_Pipeline_State pipeline_state;
    Debug_Draw_Renderer_Layer layers[DEBUG_DRAW_RENDERER_NUM_LAYER_MAX];
} Debug_Draw_Renderer;

/* Shaders */
#define GLSL(code) #code
static char DEBUG_DRAW_RENDERER_VERTEX_SHADER_DATA[] = "#version 450\n"
GLSL(
/* Vertex Shader START */                      
layout(location = 0) in vec3 position;         \n
layout(location = 1) in int  color;            \n
layout(location = 0) uniform mat4 mvp;         \n
out vec4 fs_input_color;                       \n
void main() {                                  \n
    float inv_255 = 1./255.;                   \n
    float r = ((color >>  0) & 0xFF) * inv_255;\n
    float g = ((color >>  8) & 0xFF) * inv_255;\n
    float b = ((color >> 16) & 0xFF) * inv_255;\n
    float a = ((color >> 24) & 0xFF) * inv_255;\n
    fs_input_color = vec4(r, g, b, a);         \n
    gl_Position    = mvp * vec4(position, 1.);       \n
}                                              \n
/* Vertex Shader END */
);
static char DEBUG_DRAW_RENDERER_FRAGMENT_SHADER_DATA[] = "#version 450\n"
GLSL(
/* Fragment Shader START */
in vec4 fs_input_color;                    \n
layout(location = 0) out vec4 fs_out_color;\n
void main() {                              \n
	fs_out_color = fs_input_color;         \n
}                                          \n
/* Fragment Shader END */
);
#undef GLSL

void debug_draw_renderer_init_job(Debug_Draw_Renderer *dst, Gpu_Device *gpu_device) {
    /* Buffer */
    {
        size_t vertex_buffer_size   = ALIGN(sizeof(Debug_Draw_Vertex) * U16_MAX    , sizeof(__m128));
        size_t  index_buffer_size   = ALIGN(sizeof(U16)               * U16_MAX * 2, sizeof(__m128));        
        size_t buffer_size = (vertex_buffer_size + index_buffer_size) * DEBUG_DRAW_RENDERER_NUM_LAYER_MAX;
        char *staging_buffer;
        Gpu_Buffer gpu_buffer = gpu_make_buffer_coherent(buffer_size + sizeof(__m128), &staging_buffer);
        dst->gpu_buffer       = gpu_buffer;
        ptrdiff_t offset = ALIGN(staging_buffer, sizeof(__m128)) - (ptrdiff_t)staging_buffer;

        for (size_t layer_index = 0; layer_index != DEBUG_DRAW_RENDERER_NUM_LAYER_MAX; ++layer_index) {
        
            ptrdiff_t vertex_buffer_offset = offset;
            offset += vertex_buffer_size;
            ptrdiff_t index_buffer_offset = offset;
            offset += index_buffer_size;
            /* Export */

            Debug_Draw_Renderer_Layer *layer = &dst->layers[layer_index];
            layer->dst.vertex_buffer        = staging_buffer + vertex_buffer_offset;
            layer->dst.index_bufer          = staging_buffer + index_buffer_offset;
            layer->gpu_vertex_buffer.buffer = gpu_buffer;
            layer->gpu_vertex_buffer.offset = vertex_buffer_offset;
            layer->gpu_vertex_buffer.size   = vertex_buffer_size;
            layer->gpu_index_buffer .buffer = gpu_buffer;
            layer->gpu_index_buffer .offset = index_buffer_offset;
            layer->gpu_index_buffer .size   = index_buffer_size;
        }
    }
    /* Pipeline State */
    {
        Gpu_Vertex_Attribute vertex_attributes[2] = {0};
        vertex_attributes[0].format = GPU_FORMAT_V3F32;
        vertex_attributes[0].offset = &((Debug_Draw_Vertex*)0)->as_struct.position;
        vertex_attributes[0].slot   = 0;
        vertex_attributes[1].format = GPU_FORMAT_U32;
        vertex_attributes[1].offset = &((Debug_Draw_Vertex*)0)->as_struct.color;
        vertex_attributes[1].slot   = 1;
        
        Gpu_Pipeline_State_Desc desc = {0};
        desc.num_vertex_attributes      = sizeof(vertex_attributes) / sizeof(vertex_attributes[0]);
        desc.vertex_attributes          = vertex_attributes;
        desc.vertex_shader.data     = DEBUG_DRAW_RENDERER_VERTEX_SHADER_DATA;
        desc.vertex_shader.length   = sizeof(DEBUG_DRAW_RENDERER_VERTEX_SHADER_DATA);
        desc.fragment_shader.data   = DEBUG_DRAW_RENDERER_FRAGMENT_SHADER_DATA;
        desc.fragment_shader.length = sizeof(DEBUG_DRAW_RENDERER_FRAGMENT_SHADER_DATA);

        desc.output_merger_state.depth_test_enabled   = 0;
        desc.output_merger_state.depth_write_disabled = 1;
        desc.output_merger_state.blend_enabled        = 1;
        desc.output_merger_state.blend_src            = GPU_BLEND_SRC_ALPHA;
        desc.output_merger_state.blend_dst            = GPU_BLEND_ONE_MINUS_SRC_ALPHA;

        /* Export */
        dst->pipeline_state = gpu_make_pipeline_state(gpu_device, &desc);
    }
}

typedef struct Debug_Draw_Renderer_Populate_Command_List_For_Layer_Job {
    Debug_Draw_Renderer *renderer;
    Gpu_Command_List *command_list;
    size_t layer_index;
    M4 *vps;
} Debug_Draw_Renderer_Populate_Command_List_For_Layer_Job;
void debug_draw_renderer_populate_command_list_for_layer_job(
    Debug_Draw_Renderer_Populate_Command_List_For_Layer_Job *job
) {
    gpu_cmd_set_pipeline_state(
        job->command_list,
        job->renderer->pipeline_state
    );
    gpu_cmd_set_topology(
        job->command_list,
        GPU_PRIMITIVE_TOPOLOGY_LINE_LIST
    );
    Debug_Draw_Renderer_Layer *layer =
        &job->renderer->layers[job->layer_index];
    gpu_cmd_bind_vertex_buffer_range(
        job->command_list,
        0,
        layer->gpu_vertex_buffer.buffer,
        layer->gpu_vertex_buffer.offset,
        layer->gpu_vertex_buffer.size,
        sizeof(Debug_Draw_Vertex)
    );
    gpu_cmd_bind_index_buffer_range(
        job->command_list,
        1,
        layer->gpu_index_buffer.buffer,
        layer->gpu_index_buffer.offset,
        layer->gpu_index_buffer.size,
        sizeof(U16)
    );
    gpu_cmd_set_uniform_m4(
        job->command_list,
        &job->vps[job->layer_index],
        0
    );
    gpu_cmd_draw_indexed_instanced(
        job->command_list,
        layer->dst.num_indices,
        1,
        0,
        0
    );
}

typedef struct Debug_Draw_Renderer_Populate_Command_List_Job {
    Debug_Draw_Renderer *renderer;
    Gpu_Command_List *command_list;
    M4 *vps;
} Debug_Draw_Renderer_Populate_Command_List_Job;
void debug_draw_renderer_populate_command_list_job(
    Debug_Draw_Renderer_Populate_Command_List_Job *job
) {
    gpu_cmd_set_pipeline_state(
        job->command_list,
        job->renderer->pipeline_state
    );
    gpu_cmd_set_topology(
        job->command_list,
        GPU_PRIMITIVE_TOPOLOGY_LINE_LIST
    );
    for (size_t layer_index = 0; layer_index != DEBUG_DRAW_RENDERER_NUM_LAYER_MAX; ++layer_index) {
        Debug_Draw_Renderer_Layer *layer = &job->renderer->layers[layer_index];
        gpu_cmd_bind_vertex_buffer_range(
            job->command_list,
            0,
            layer->gpu_vertex_buffer.buffer,
            layer->gpu_vertex_buffer.offset,
            layer->gpu_vertex_buffer.size,
            sizeof(Debug_Draw_Vertex)
        );
        gpu_cmd_bind_index_buffer_range(
            job->command_list,
            1,
            layer->gpu_index_buffer.buffer,
            layer->gpu_index_buffer.offset,
            layer->gpu_index_buffer.size,
            sizeof(U16)
        );
        gpu_cmd_set_uniform_m4(
            job->command_list,
            &job->vps[layer_index], //&renderer->vp, 
            0
        );
        gpu_cmd_draw_indexed_instanced(
            job->command_list,
            layer->dst.num_indices,
            1,
            0,
            0
        );
    } 
}