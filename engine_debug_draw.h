/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

typedef union Debug_Draw_Vertex {
    struct As_Struct {
        V3  position;
        C32 color;
    } as_struct;
    __m128 as_m128;
} Debug_Draw_Vertex;

typedef struct Debug_Draw_Dst {
    size_t             num_vertices;
    size_t             num_indices;
    Debug_Draw_Vertex *vertex_buffer;
    U16               *index_bufer;
} Debug_Draw_Dst;

Debug_Draw_Dst *debug_draw_get_default_dst();
Debug_Draw_Dst *debug_draw_get_dst(size_t layer_index);
void debug_reserve(size_t num_vertices, size_t num_indices, Debug_Draw_Vertex **vertices, U16 **indices, size_t *index_of_first_reserved_vertex);
void debug_clear();
void debug_segment(V3 start, V3 end, C32 color);
void debug_ray    (V3 origin, V3 direction, F32 distance, C32 color);
void debug_line   (V3 origin, V3 direction, C32 color);
void debug_aabb(V4 aabb, C32 color);
void debug_circ(V3 pos, F32 radius, V3 normal, C32 color);
void debug_arc (V3 pos, F32 radius, V3 normal, F32 start_revolutions, F32 end_revolutions, size_t num_subdivisions, C32 color);
void debug_polyline(size_t num_points, V3 *points, size_t closed, C32 color);
void debug_points  (size_t num_points, V3 *points, C32 color);
void debug_instance(M4 trs, size_t num_vertices, size_t num_indices, Debug_Draw_Vertex **vertices, U16 *indices);
void debug_grid();
void debug_box(M4 trs, C32 color);
void debug_cone(M4 trs, C32 color);
void debug_arrow(V3 start, V3 end, F32 radius, C32 color);
void debug_vector(V3 origin, V3 direction, C32 color);
void debug_sphere(V3 position, F32 radius, C32 color);
void debug_ascii(V3 position, V2 alignement, C32 color, char *ascii, ...);
void debug_joint(M4 trs);
F32  debug_time();

void debug_segment_to(Debug_Draw_Dst *dst, V3 start, V3 end, C32 color);

#ifndef DEBUG_DRAW_GET_DST
#error To use debug draw, please define "Debug_Draw_Dst *DEBUG_DRAW_GET_DST(size_t layer_index)".
//#define DEBUG_DRAW_GET_DST(...) 0
#endif

#ifndef DEBUG_DRAW_GET_DEFAULT_DST
static Debug_Draw_Dst *debug_draw_default_dst;
#define DEBUG_DRAW_GET_DEFAULT_DST() (debug_draw_default_dst)
#endif

void _debug_draw_reserve(
    size_t    layer_index,
    size_t    num_vertices, 
    size_t    num_indices, 
    __m128  **vertices,
    __m128i **indices,
    size_t    *index_of_first_reserved_vertex
) {
    Debug_Draw_Dst *dst = DEBUG_DRAW_GET_DST(layer_index);
    size_t vertex_index = INTERLOCKED_EXCHANGE_ADD(&dst->num_vertices, num_vertices);
    size_t index_index  = INTERLOCKED_EXCHANGE_ADD(&dst->num_indices, num_indices);
    *vertices = &dst->vertex_buffer[vertex_index];
    *indices  = &dst->index_bufer[index_index];
    *index_of_first_reserved_vertex = vertex_index;
}

void debug_draw_reserve(
    size_t    num_vertices, 
    size_t    num_indices, 
    __m128  **vertices,
    __m128i **indices,
    size_t    *index_of_first_reserved_vertex
) {
    Debug_Draw_Dst *dst = DEBUG_DRAW_GET_DEFAULT_DST();
    size_t vertex_index = INTERLOCKED_EXCHANGE_ADD(&dst->num_vertices, num_vertices);
    size_t index_index  = INTERLOCKED_EXCHANGE_ADD(&dst->num_indices, num_indices);
    *vertices = &dst->vertex_buffer[vertex_index];
    *indices  = &dst->index_bufer[index_index];
    *index_of_first_reserved_vertex = vertex_index;
}

// void debug_clear() {
//     Debug_Draw_Dst *dst = debug_draw_get_default_dst();
//     dst->num_vertices = 0;
//     dst->num_indices  = 0;
// }
void debug_draw_clear_layer(size_t layer_index) {
    Debug_Draw_Dst *dst = DEBUG_DRAW_GET_DST(layer_index);
    INTERLOCKED_EXCHANGE(&dst->num_vertices, 0);
    INTERLOCKED_EXCHANGE(&dst->num_indices, 0);
    //dst->num_vertices = 0;
    //dst->num_indices  = 0;
}

void debug_draw_clear() {
    Debug_Draw_Dst *dst = DEBUG_DRAW_GET_DEFAULT_DST();
    dst->num_vertices = 0;
    dst->num_indices  = 0;
}

void debug_segment2(size_t layer_index, V2 start, V2 end, C32 color) {
    __m128  *vertices;
    __m128i *dst_indices;
    size_t   i_vertex;
    _debug_draw_reserve(layer_index, 2, 8 /* 2 */, &vertices, &dst_indices, &i_vertex);
    
    vertices[0].m128_f32[0] = start.x;
    vertices[0].m128_f32[1] = start.y;
    vertices[0].m128_f32[2] = 0.5f;
    vertices[0].m128_i32[3] = color;
    vertices[1].m128_f32[0] = end.x;
    vertices[1].m128_f32[1] = end.y;
    vertices[1].m128_f32[2] = 0.5f;
    vertices[1].m128_i32[3] = color;
    
    __m128i offset  = _mm_set1_epi16((short)i_vertex);
    __m128i indices = _mm_add_epi16(offset, _mm_set_epi16( 0,  0,  0,  0,  0,  0,  1,  0));
    dst_indices[0]  = indices;
}

void debug_segment3(size_t layer_index, V2 start, V2 end, C32 color) {

}

void debug_segment(V3 start, V3 end, C32 color) {
    __m128  *vertices;
    __m128i *indices;
    size_t   i_vertex;
    debug_draw_reserve(2, 8 /* 2 */, &vertices, &indices, &i_vertex);
    
    vertices[0].m128_f32[0] = start.x;
    vertices[0].m128_f32[1] = start.y;
    vertices[0].m128_f32[2] = start.z;
    vertices[0].m128_i32[3] = color;
    vertices[1].m128_f32[0] = end.x;
    vertices[1].m128_f32[1] = end.y;
    vertices[1].m128_f32[2] = end.z;
    vertices[1].m128_i32[3] = color;
    
    __m128i offset = _mm_set1_epi16((short)i_vertex);
    indices[0] = _mm_add_epi16(offset, _mm_set_epi16( 0,  0,  0,  0,  0,  0,  1,  0));
}

void debug_circ(V2 position, F32 radius, C32 color) {
    __m128  *vertices;
    __m128i *indices;
    size_t   i_vertex;
    debug_draw_reserve(16, 32, &vertices, &indices, &i_vertex);
    __m128 translation = _mm_set_ps(*(float*)(&color), 0, position.y, position.x);
    __m128 scale       = _mm_set1_ps(radius);
    vertices[ 0] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.0000, +1.0000), scale, translation);
    vertices[ 1] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.3827, +0.9239), scale, translation);
    vertices[ 2] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.7071, +0.7071), scale, translation);
    vertices[ 3] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.9239, +0.3827), scale, translation);
    vertices[ 4] = _mm_fmadd_ps(_mm_set_ps(0, 0, +1.0000, +0.0000), scale, translation);
    vertices[ 5] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.9239, -0.3827), scale, translation);
    vertices[ 6] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.7071, -0.7071), scale, translation);
    vertices[ 7] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.3827, -0.9239), scale, translation);
    vertices[ 8] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.0000, -1.0000), scale, translation);
    vertices[ 9] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.3827, -0.9239), scale, translation);
    vertices[10] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.7071, -0.7071), scale, translation);
    vertices[11] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.9239, -0.3827), scale, translation);
    vertices[12] = _mm_fmadd_ps(_mm_set_ps(0, 0, -1.0000, -0.0000), scale, translation);
    vertices[13] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.9239, +0.3827), scale, translation);
    vertices[14] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.7071, +0.7071), scale, translation);
    vertices[15] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.3827, +0.9239), scale, translation);
    
    __m128i offset = _mm_set1_epi16((short)i_vertex);
    indices[0] = _mm_add_epi16(offset, _mm_set_epi16( 4,  3,  3,  2,  2,  1,  1,  0));
    indices[1] = _mm_add_epi16(offset, _mm_set_epi16( 8,  7,  7,  6,  6,  5,  5,  4));
    indices[2] = _mm_add_epi16(offset, _mm_set_epi16(12, 11, 11, 10, 10,  9,  9,  8));
    indices[3] = _mm_add_epi16(offset, _mm_set_epi16( 0, 15, 15, 14, 14, 13, 13, 12));
}

void debug_sphere(V3 position, F32 radius, C32 color) {
    __m128  *vertices;
    __m128i *indices;
    size_t   i_vertex;
    debug_draw_reserve(16, 32, &vertices, &indices, &i_vertex);
    ASSERT(indices == ALIGN(indices, sizeof(__m128)), 
        "debug_draw index pointer is missalligned. 0x%llX, should be 0x%llX", vertices, ALIGN(vertices, sizeof(__m128))
    );
    ASSERT(indices == ALIGN(indices, sizeof(__m128)), 
        "debug_draw vertex pointer is missalligned. 0x%llX, should be 0x%llX", indices, ALIGN(indices, sizeof(__m128))
    );
    
    __m128 translation = _mm_set_ps(*(float*)(&color), position.z, position.y, position.x);
    __m128 scale       = _mm_set1_ps(radius);
    vertices[ 0] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.0000, +1.0000), scale, translation);
    vertices[ 1] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.3827, +0.9239), scale, translation);
    vertices[ 2] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.7071, +0.7071), scale, translation);
    vertices[ 3] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.9239, +0.3827), scale, translation);
    vertices[ 4] = _mm_fmadd_ps(_mm_set_ps(0, 0, +1.0000, +0.0000), scale, translation);
    vertices[ 5] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.9239, -0.3827), scale, translation);
    vertices[ 6] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.7071, -0.7071), scale, translation);
    vertices[ 7] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.3827, -0.9239), scale, translation);
    vertices[ 8] = _mm_fmadd_ps(_mm_set_ps(0, 0, +0.0000, -1.0000), scale, translation);
    vertices[ 9] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.3827, -0.9239), scale, translation);
    vertices[10] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.7071, -0.7071), scale, translation);
    vertices[11] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.9239, -0.3827), scale, translation);
    vertices[12] = _mm_fmadd_ps(_mm_set_ps(0, 0, -1.0000, -0.0000), scale, translation);
    vertices[13] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.9239, +0.3827), scale, translation);
    vertices[14] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.7071, +0.7071), scale, translation);
    vertices[15] = _mm_fmadd_ps(_mm_set_ps(0, 0, -0.3827, +0.9239), scale, translation);
    
    __m128i offset = _mm_set1_epi16((short)i_vertex);
    indices[0] = _mm_add_epi16(offset, _mm_set_epi16( 4,  3,  3,  2,  2,  1,  1,  0));
    indices[1] = _mm_add_epi16(offset, _mm_set_epi16( 8,  7,  7,  6,  6,  5,  5,  4));
    indices[2] = _mm_add_epi16(offset, _mm_set_epi16(12, 11, 11, 10, 10,  9,  9,  8));
    indices[3] = _mm_add_epi16(offset, _mm_set_epi16( 0, 15, 15, 14, 14, 13, 13, 12));
}

void debug_vector(V3 origin, V3 direction, C32 color) {
    __m128  *vertices;
    __m128i *indices;
    size_t   i_vertex;
    debug_draw_reserve(
        4,
        8, 
        &vertices, 
        &indices, 
        &i_vertex
    );
    
    vertices[0].m128_f32[0] = origin.x;
    vertices[0].m128_f32[1] = origin.y;
    vertices[0].m128_f32[2] = origin.z;
    vertices[0].m128_i32[3] = color;

    V3 stylized_direction = v3_move_towards_zero(direction, 0.0625f);
    vertices[1] = _mm_add_ps(_mm_set_ps(0, stylized_direction.z, stylized_direction.y, stylized_direction.x), vertices[0]);
    
    __m128i offset = _mm_set1_epi16((short)i_vertex);
    indices[0] = _mm_add_epi16(offset, _mm_set_epi16( 0,  0,  0,  0,  0,  0,  1,  0));
    
    debug_sphere(v3_add(origin, direction), 0.0625f, color);
}

void debug_aabb(V4 aabb, C32 color) {
    __m128  *vertices;
    __m128i *indices;
    size_t   i_vertex;
    debug_draw_reserve(4, 8, &vertices, &indices, &i_vertex);
    vertices[0].m128_f32[0] = aabb.x;
    vertices[0].m128_f32[1] = aabb.y;
    vertices[0].m128_f32[2] = 0.5f;
    vertices[0].m128_i32[3] = color;
    vertices[1].m128_f32[0] = aabb.z;
    vertices[1].m128_f32[1] = aabb.y;
    vertices[1].m128_f32[2] = 0.5f;
    vertices[1].m128_i32[3] = color;
    vertices[2].m128_f32[0] = aabb.x;
    vertices[2].m128_f32[1] = aabb.w;
    vertices[2].m128_f32[2] = 0.5f;
    vertices[2].m128_i32[3] = color;
    vertices[3].m128_f32[0] = aabb.z;
    vertices[3].m128_f32[1] = aabb.w;
    vertices[3].m128_f32[2] = 0.5f;
    vertices[3].m128_i32[3] = color;
    __m128i offset = _mm_set1_epi16((short)i_vertex);
    indices[0] = _mm_add_epi16(offset, _mm_set_epi16(3,  2,  3,  1,  2,  0,  1,  0));
}