// immediate draw module
typedef unsigned char      Draw_U8;
typedef unsigned int       Draw_U32;
typedef unsigned long long Draw_U64;
typedef float              Draw_F32;
typedef unsigned int       Draw_Color32;
typedef unsigned char      Draw_Palette_Color;
typedef unsigned short     Draw_Index;
typedef enum Draw_Palette_Color_ {
	DRAW_BLACK       = 0x00,
	DRAW_RED         = 0x01,
	DRAW_GREEN       = 0x02,
	DRAW_YELLOW      = 0x03,
	DRAW_BLUE        = 0x04,
	DRAW_MAGENTA     = 0x05,
	DRAW_CYAN        = 0x06,
	DRAW_WHITE       = 0x07,
				     
	DRAW_DARKGRAY    = 0x08,
	DRAW_ORANGE      = 0x09,
	DRAW_MINT        = 0x0A,
	DRAW_BEIGE       = 0x0B,
	DRAW_PURPLE      = 0x0C,
	DRAW_PINK        = 0x0D,
	DRAW_TURQUOISE   = 0x0E,
	DRAW_LIGHTGRAY   = 0x0F,

	DRAW_ALPHA_10000 = 0x00,
	DRAW_ALPHA_09375 = 0x10,
	DRAW_ALPHA_08750 = 0x20,
	DRAW_ALPHA_08125 = 0x30,
	DRAW_ALPHA_07500 = 0x40,
	DRAW_ALPHA_06875 = 0x50,
	DRAW_ALPHA_06250 = 0x60,
	DRAW_ALPHA_05625 = 0x70,
	DRAW_ALPHA_05000 = 0x80,
	DRAW_ALPHA_04375 = 0x90,
	DRAW_ALPHA_03750 = 0xA0,
	DRAW_ALPHA_03125 = 0xB0,
	DRAW_ALPHA_02500 = 0xC0,
	DRAW_ALPHA_01875 = 0xD0,
	DRAW_ALPHA_01250 = 0xE0,
	DRAW_ALPHA_00625 = 0xF0,

	DRAW_ALPHA_100   = DRAW_ALPHA_10000,
	DRAW_ALPHA_75    = DRAW_ALPHA_07500,
	DRAW_ALPHA_50    = DRAW_ALPHA_05000,
	DRAW_ALPHA_25    = DRAW_ALPHA_02500
} Draw_Palette_Color_;
typedef struct Draw_Vertex {
	Draw_F32 x;
	Draw_F32 y;
	//Draw_F32 width;
	Draw_U32 color;
} Draw_Vertex;
typedef struct Draw_Context {
	// gpu
	Gpu_Pipeline_State gpu_pipeline_state;
	Gpu_Command_List   gpu_command_list;
	Gpu_Command_Queue  gpu_command_queue;
	Gpu_Buffer         gpu_buffer;
	Gpu_Buffer_Range   gpu_vertex_buffer;
	Gpu_Buffer_Range   gpu_index_buffer;
	// cpu
	Draw_Vertex*      cpu_vertex_buffer;
	Draw_Index*       cpu_index_buffer;
	Draw_U64          num_vertices_max;
	Draw_U64          num_indices_max;
	Draw_U64          num_vertices;
	Draw_U64          num_indices;
    // matrix 
    M3x2              mvp_matrix;
} Draw_Context;

// API
Draw_Context* get_draw_context();
Draw_Color32 draw_color32(Draw_U8 r, Draw_U8 g, Draw_U8 b, Draw_U8 a);
Draw_Color32 draw_color(Draw_F32 r, Draw_F32 g, Draw_F32 b, Draw_F32 a);
void         draw_circ(Draw_F32 x, Draw_F32 y, Draw_F32 radius, int subdivision, Draw_Palette_Color color);

// utils
inline float _draw_fakesin(float scalar) {
	float two_times_loop_range_minus_one_to_one = 2.0f * (scalar - floorf(scalar) - 0.5f);
	return 
		two_times_loop_range_minus_one_to_one * 
		(1.0f - fabsf(two_times_loop_range_minus_one_to_one)) * 4;
}

// Implementation
void draw_clear(Draw_Context* ctx) { ctx->num_vertices = 0; ctx->num_indices = 0; }

Draw_Color32 draw_color32(Draw_U8 r, Draw_U8 g, Draw_U8 b, Draw_U8 a) {
	return ((int)r) | (((int)g) << 8) | (((int)g) << 16) | (((int)g) << 24);
}
Draw_Color32 draw_color(Draw_F32 r, Draw_F32 g, Draw_F32 b, Draw_F32 a) {
	return draw_color32(
		(Draw_U8)(r * 255), 
		(Draw_U8)(g * 255), 
		(Draw_U8)(b * 255), 
		(Draw_U8)(a * 255)
	);
};

void draw_polyline(
	int n_line,
	Draw_F32          (*x_a)[2], 
	int closed, 
	Draw_F32           width, 
	Draw_Palette_Color color
) {
	//Draw_Contex* ctx = get_draw_context();
}

void draw_matrix(
    Draw_Context* context,
    M3x2 matrix
) {
    context->mvp_matrix = matrix;
}

void draw_line(
    Draw_Context* context,
    Draw_F32      start_x,
    Draw_F32      start_y,
	Draw_F32      end_x,
    Draw_F32      end_y,
    Draw_F32      width,
    Draw_U32      color
) {
    static const int NUM_COORD_PER_VERTEX = 1;
    static const int NUM_VERTEX_PER_EDGE  = 1;

    Draw_Index working_vertex_index = context->num_vertices;

    // reserve
    Draw_Vertex *vertices = &context->cpu_vertex_buffer[context->num_vertices];
    Draw_Index  *indices  = &context->cpu_index_buffer [context->num_indices];
    context->num_vertices += 2;
    context->num_indices  += 2;

    // vertices
    vertices[0].x     = start_x;
    vertices[0].y     = start_y;
    *((V2*)(&vertices[0].x)) =
        m3x2_multiply_point(context->mvp_matrix, *((V2*)(&vertices[0].x)));
    vertices[0].color = color;

    vertices[1].x     = end_x;
    vertices[1].y     = end_y;
    *((V2*)(&vertices[1].x)) =
        m3x2_multiply_point(context->mvp_matrix, *((V2*)(&vertices[1].x)));
    vertices[1].color = color;

    // indicies
    indices[0] = working_vertex_index + 0;
    indices[1] = working_vertex_index + 1;
}

void draw_line_v(
    Draw_Context* context,
    V2            start,
    V2            end,
    Draw_F32      width,
    Draw_U32      color
) {
    draw_line(context, start.x, start.y, end.x, end.y, width, color);
}

void draw_circ(
	Draw_Context* context,
	Draw_F32      x, 
	Draw_F32      y, 
	Draw_F32      radius, 
    Draw_U32      num_subdivisions, 
	Draw_F32      width, 
    Draw_U32      color
) {
	static const int NUM_COORD_PER_VERTEX = 1;
	static const int NUM_VERTEX_PER_EDGE  = 1;

	Draw_Index working_vertex_index = context->num_vertices;

	// reserve
	Draw_Vertex *vertices = &context->cpu_vertex_buffer[context->num_vertices];
	Draw_Index  *indices  = &context->cpu_index_buffer [context->num_indices];
    context->num_vertices += num_subdivisions;
    context->num_indices  += num_subdivisions * 2;

	Draw_F32 radian_per_subdivision = 1.0f / num_subdivisions;
	for (int i_vertex = 0; i_vertex != num_subdivisions; ++i_vertex) {
		Draw_F32 theta = radian_per_subdivision * i_vertex;
		//vertices[i_vertex].x     = x + _draw_fakesin(theta - 0.25f) * radius;
		//vertices[i_vertex].y     = y + _draw_fakesin(theta)         * radius;
        vertices[i_vertex].x     = x + sinf((theta + 0.25f) * 2 * F32_PI) * radius;
        vertices[i_vertex].y     = y + sinf((theta)         * 2 * F32_PI) * radius;

        // apply matrix
        *((V2*)(&vertices[i_vertex].x)) =
            m3x2_multiply_point(context->mvp_matrix, *((V2*)(&vertices[i_vertex].x)));
		
        //vertices[i_vertex].width = width;
		vertices[i_vertex].color = color;
	}
	for (int i_edge = 0; i_edge != num_subdivisions; ++i_edge) {
		indices[i_edge * 2 + 0] = working_vertex_index + i_edge;
		indices[i_edge * 2 + 1] = working_vertex_index + ((i_edge + 1) % num_subdivisions);
	}
}

void draw_arc(
	Draw_Context* context,
	Draw_F32      x, 
	Draw_F32      y, 
	Draw_F32      radius, 
	Draw_F32      start_angle_in_revolutions, /* (1 Revolution) = (1 / 2PI Radians) */
	Draw_F32      end_angle_in_revolutions, 
    Draw_U32      num_subdivisions, 
	Draw_F32      width, 
    Draw_U32      color
) {
	static const int NUM_COORD_PER_VERTEX = 1;
	static const int NUM_VERTEX_PER_EDGE  = 1;

	Draw_Index working_vertex_index = context->num_vertices;

	// reserve
	Draw_Vertex *vertices = &context->cpu_vertex_buffer[context->num_vertices];
	Draw_Index  *indices  = &context->cpu_index_buffer [context->num_indices];
	Int num_vertices = num_subdivisions + 1;
    context->num_vertices += num_vertices;
    context->num_indices  += num_subdivisions * 2;

	Draw_F32 revolutions_per_subdivision = 
		(end_angle_in_revolutions - start_angle_in_revolutions) / num_subdivisions;
	for (int i_vertex = 0; i_vertex != num_vertices; ++i_vertex) {
		Draw_F32 theta_in_revolutions = start_angle_in_revolutions + revolutions_per_subdivision * i_vertex;
		//vertices[i_vertex].x     = x + _draw_fakesin(theta - 0.25f) * radius;
		//vertices[i_vertex].y     = y + _draw_fakesin(theta)         * radius;
        vertices[i_vertex].x     = x + sinf((theta_in_revolutions + 0.25f) * 2 * F32_PI) * radius;
        vertices[i_vertex].y     = y + sinf((theta_in_revolutions)         * 2 * F32_PI) * radius;

        // apply matrix
        *((V2*)(&vertices[i_vertex].x)) =
            m3x2_multiply_point(context->mvp_matrix, *((V2*)(&vertices[i_vertex].x)));
		
        //vertices[i_vertex].width = width;
		vertices[i_vertex].color = color;
	}
	for (int i_edge = 0; i_edge != num_subdivisions; ++i_edge) {
		indices[i_edge << 1 | 0] = working_vertex_index + i_edge;
		indices[i_edge << 1 | 1] = working_vertex_index + (i_edge + 1);
	}
}

void draw_points_v(
	Draw_Context *context,
	int           num_points,
	V2           *positions,
    Draw_U32      color
) {
	for(int i = 0; i < num_points; ++i) {
		draw_circ(context, positions[i].x, positions[i].y, 0.03125 * 0.5f, 3, 0, color);
	}
}

void draw_points_strided(
	Draw_Context *context,
	int           num_points,
	V2           *positions,
	U8            stride,
    Draw_U32      color
) {
	for(int i = 0; i < num_points; ++i) {
		V2* position = (V2*)&((char*)positions)[i * stride];
		draw_circ(
			context, 
			position->x, 
			position->y, 
			0.03125 * 0.5f, 
			3, 
			0, 
			color
		);
	}
}
