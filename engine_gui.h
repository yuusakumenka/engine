
#ifndef GUI_ASSERT
#define GUI_ASSERT(test, msg, ...)
#endif

#ifndef GUI_RETURN_ADDRESS
#ifdef _MSC_VER
#define GUI_RETURN_ADDRESS() (_ReturnAddress())
#elif __GNUC__ /* GCC */
#define GUI_RETURN_ADDRESS() (__builtin_return_address())
#endif /* Compiler */
#endif

#ifndef GUI_NOINLINE
#ifdef _MSC_VER
#define GUI_NOINLINE __declspec(noinline)
#elif __GNUC__ /* GCC */
#define GUI_NOINLINE _noinline
#endif /* Compiler */
#endif


typedef struct Gui_Input {
    V2 mouse_position;
    B8 lmb_got_pressed;
    B8 lmb_got_released;
    B8 lmb_is_pressed;
} Gui_Input;

typedef struct Gui_Output {
    B8  mouse_is_obstructed_by_gui;
    U64 num_indices;
} Gui_Output;

typedef U64 Gui_Uid;
#define GUI_INVALID_UID ((Gui_Uid)-1)

typedef I32 Gui_Color;

typedef struct Gui_Vertex {
    V2        position;
    Gui_Color color;
} Gui_Vertex;

typedef U16 Gui_Index;

typedef struct Gui_Theme {
    Gui_Color button_color_idle;
    Gui_Color button_color_hover;
    Gui_Color button_color_pressed;
    Gui_Color button_text_color;
    V2        button_text_allignement;
} Gui_Theme;

typedef struct Gui_Init {
    U64         num_vertices_max;
    U64         num_indices_max;
    Gui_Vertex *vertex_buffer;
    Gui_Vertex *index_buffer;
    Gui_Theme  *theme;
} Gui_Init;

typedef struct Gui {
    U64         num_vertices_max;
    U64         num_indices_max;
    U64         num_vertices;
    U64         num_indices;
    Gui_Vertex *vertex_buffer;
    Gui_Index  *index_buffer;
    Gui_Uid     active_element;
    Gui_Input  *this_frame_input;
    Gui_Theme  *theme;
    B8          mouse_is_obstructed_by_gui;
} Gui;

typedef U8 Gui_Layout_Kind;
typedef enum Gui_Layout_Kind_ {
    GUI_LAYOUT_KIND_VETICAL,
    GUI_LAYOUT_KIND_COUNT
} Gui_Layout_Kind_;

typedef struct Gui_Layout {
    Gui             *gui;
    Gui_Layout_Kind  kind;
} Gui_Layout;

typedef Bool (*Gui_Layout_Button_Proc)(Gui_Layout *layout, Gui_Uid uid, char* label);

typedef struct Gui_Vertical_Layout Gui_Vertical_Layout;
Bool gui_vertical_layout_button(Gui_Vertical_Layout *layout, Gui_Uid uid, char* label);

typedef struct Gui_Layout_Vtable_Entry {
    Gui_Layout_Button_Proc gui_layout_button;
} Gui_Layout_Vtable_Entry;

static const Gui_Layout_Vtable_Entry GUI_LAYOUT_VTABLE[GUI_LAYOUT_KIND_COUNT] = {
    {
        gui_vertical_layout_button
    }
};

typedef struct Gui_Vertical_Layout {
    Gui_Layout base;

} Gui_Vertical_Layout;

U64 gui_vertex_buffer_size(U64 num_vertices_max) {
    return sizeof(Gui_Vertex) * num_vertices_max;
}

U64 gui_index_buffer_size(U64 num_indices_max) {
    return sizeof(Gui_Index) * num_indices_max;
}

void gui_init(Gui *gui, Gui_Init *init) {
    GUI_ASSERT(init->vertex_buffer, "Missing vertex buffer.");
    GUI_ASSERT(init->index_buffer , "Missing index buffer." );

    gui->num_vertices_max = init->num_vertices_max;
    gui->num_indices_max  = init->num_indices_max;
    gui->vertex_buffer    = init->vertex_buffer;
    gui->index_buffer     = init->index_buffer;
    gui->theme            = init->theme;
}

void gui_clear(Gui *gui) {
    gui->num_vertices = 0;
    gui->num_indices = 0;
}

void gui_begin_frame(Gui *gui, Gui_Input *input) {
    gui->this_frame_input = input;
}

void gui_end_frame(Gui *gui, Gui_Output *output) {
    output->mouse_is_obstructed_by_gui = gui->mouse_is_obstructed_by_gui;
    output->num_indices                = gui->num_indices;
}

/* Test */
Bool gui_test_overlap(V4 rect, V2 point) {
    return
        point.x >= rect.x && point.x < rect.z &&
        point.y >= rect.y && point.y < rect.w;
}

/* Rendering */
void gui_reserve(Gui *gui, U64 num_vertices, U64 num_indices, Gui_Vertex **vertices, Gui_Index **indices, U64* vertex_index_offset) {
    GUI_ASSERT(gui->num_vertices < gui->num_vertices_max - num_vertices, "Too many vertices.");
    GUI_ASSERT(gui->num_indices  < gui->num_indices_max  - num_indices , "Too many indices." );
    *vertices            = &gui->vertex_buffer[gui->num_vertices];
    *indices             = &gui->index_buffer [gui->num_indices ];
    *vertex_index_offset = gui->num_vertices;
    gui->num_vertices   += num_vertices;
    gui->num_indices    += num_indices;
}
void gui_fill_rect(Gui *gui, V4 rect, Gui_Color color) {
    Gui_Vertex *vertices; Gui_Index *indices; U64 vertex_index_offset;
    gui_reserve(gui, 4, 6, &vertices, &indices, &vertex_index_offset);
    vertices[0].position.x = rect.x;
    vertices[0].position.y = rect.y;
    vertices[0].color      = color;
    vertices[1].position.x = rect.z;
    vertices[1].position.y = rect.y;
    vertices[1].color      = color;
    vertices[2].position.x = rect.x;
    vertices[2].position.y = rect.w;
    vertices[2].color      = color;
    vertices[3].position.x = rect.z;
    vertices[3].position.y = rect.w;
    vertices[3].color      = color;
    indices[0] = vertex_index_offset + 0;
    indices[1] = vertex_index_offset + 1;
    indices[2] = vertex_index_offset + 2;
    indices[3] = vertex_index_offset + 1;
    indices[4] = vertex_index_offset + 3;
    indices[5] = vertex_index_offset + 2;
}
void gui_stroke_rect(Gui *gui, V4 rect, Gui_Color color);
void gui_text(Gui *gui, V4 rect, char *text, Gui_Color color) {
    
}

/* Theme */
void gui_theme_default(Gui_Theme *theme) {
    theme->button_color_idle = 0xFFFFFFFF;
}

/* Atoms */
Bool _gui_button(Gui *gui, Gui_Uid uid, V4 rect, char* label) {
    Bool is_hovered = 
        !gui->mouse_is_obstructed_by_gui && 
        gui_test_overlap(rect, gui->this_frame_input->mouse_position);

    if(is_hovered) {
        if(gui->this_frame_input->lmb_got_pressed) {
            gui->active_element = uid;
        }
        gui->mouse_is_obstructed_by_gui = TRUE;
    }
    Bool is_active   = uid == gui->active_element;
    Bool got_pressed = FALSE;
    if (gui->this_frame_input->lmb_got_released) {
        if (is_active) {
            gui->active_element = GUI_INVALID_UID;
            if (is_hovered) {
                got_pressed = TRUE;
            }
        }
    }
    Gui_Color color;
    if(is_hovered) {
        if(
            is_active &&
            gui->this_frame_input->lmb_is_pressed
        ) {
            color = gui->theme->button_color_pressed;
        } else {
            color = gui->theme->button_color_hover;
        }
    } else {
        color = gui->theme->button_color_idle;
    }

    gui_fill_rect(gui, rect, color);
    gui_text(gui, rect, label, gui->theme->button_text_color);
    return got_pressed;
}

Bool gui_button(Gui *gui, V4 rect, char* label) {
    Gui_Uid uid = (Gui_Uid)(GUI_RETURN_ADDRESS());
    return _gui_button(uid, uid, rect, label);
}

/* Layouts */
GUI_NOINLINE
Bool gui_layout_button(Gui_Layout *layout, char* label) {
    Gui_Uid uid = (Gui_Uid)(GUI_RETURN_ADDRESS());
    return GUI_LAYOUT_VTABLE[layout->kind].gui_layout_button(layout, uid, label);
}

/* Vertical Layout */
Bool gui_begin_vertical_layout(Gui *gui, Gui_Vertical_Layout *layout) {
    layout->base.gui  = gui;
    layout->base.kind = GUI_LAYOUT_KIND_VETICAL;
}
Bool gui_vertical_layout_button(Gui_Vertical_Layout *layout, Gui_Uid uid, char* label) {
    _gui_button(layout->base.gui, uid, V4_ZERO, label);
}

static const char GUI_VERTEX_SHADER_OPENGL[256];
static const char GUI_FRAGMENT_SHADER_OPENGL[256];