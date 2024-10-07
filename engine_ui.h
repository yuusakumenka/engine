/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* Roadmap:
 * * Become full standalone:
 *     Handle all dependencies with macros:
 *     * UI_ASSERT (Done)
 *     * Arena (Todo) :
 *         Replace with a choice between either
 *          * Specifying an Arena api with macros.
 *          * Specifying budgets with macros.
 *     * Color (Todo)
 *         We probably wanna keep 32bit internal storage,
 *         but ask for your specified format in the API?
 *     * Geoprint (Done)
 *         Disabled by default. At this point user can change the code.
 *     * memcpy, memset, size_t, ptrdiff_t
 *         We could replace those with macros
 *         and define them in our UI... scope if you don't provide them. */

/* Dependencies */
#pragma region Dependencies

/* Assertion */
#ifndef UI_ASSERT
#ifndef UI_DISABLE_ASSERT
#pragma message("`UI_ASSERT` is not defined before including `engine_ui.h`. Define UI_DISABLE_ASSERT if this is intenional.")
#endif /* !UI_DISABLE_ASSERT */
#define UI_ASSERT(...)
#endif

/* Alocation scheme:
 * * Budget -> Zero allocations 
 * * Arena */

/* Color */

/* Debugging: Geoprint */
#ifdef UI_ENABLE_GEOPTRINT
#endif /* UI_ENABLE_GEOPTRINT */

#pragma endregion /* Dependencies */

#define UI_SCALE 1

#define UI_UP     (v2( 0.f, +1.f))
#define UI_DOWN   (v2( 0.f, -1.f))
#define UI_RIGHT  (v2(+1.f,  0.f))
#define UI_LEFT   (v2(-1.f,  0.f))
#define UI_TOP    (v2( 0.f, +1.f))
#define UI_BOTTOM (v2( 0.f, -1.f))
#define UI_MIDDLE ( 0.0f)
#define UI_ICON_ELLIPSIS   "\x01"
#define UI_ICON_SPHERE     "\x02\x03"
#define UI_ICON_DIRECTORY  "\x04\x05"
#define UI_ICON_MESH2D     "\x06\x07"
#define UI_ICON_FLOPPY     "\x08\x09"
#define UI_ICON_NEW_FLOPPY "\x0A\x0B"
#define UI_ICON_NEW_FILE   "\x0C\x0D"
#define UI_ICON_GEAR       "\x0E\x0F"
#define UI_ICON_QUESTION   "\x10\x11"
#define UI_ICON_LIGHT      "\x12\x13"
#define UI_ICON_IMPORT     "\x14\x15"
#define UI_ICON_EYE        "\x16\x17"
#define UI_ICON_EYE_CLOSED "\x18\x19"
#define UI_ICON_SOLO       "\x1A\x1B"
#define UI_ICON_SOLO_FADED "\x1C\x1D"

#define UI_DEFER_LOOP(start, end) for(size_t _i_ = ((start), 0); !_i_; _i_ = ((end), 1))
#define UI_DEFER_LOOP_TEST(start, end) for(size_t _i_ = ((start)); !_i_; _i_ = ((end), 1))

/* Ui Hash */
typedef U64 Ui_Hash; /* may be renamed to Ui_Key */
#define UI_HASH_INVALID (~0ull)
#define UI_HASH_CREATE  (UI_HASH_INVALID - (__COUNTER__ + 1))

typedef struct Ui_Event {
    Bool clicked         : 1;
    Bool got_pressed     : 1;
    Bool got_released    : 1;
    Bool is_pressed      : 1;
    Bool is_released     : 1;
    Bool is_hovered      : 1;
    Bool drag            : 1;
    Bool drop            : 1;
    Bool clicked_rmb     : 1;
    Bool is_pressed_rmb  : 1;
} Ui_Event;

typedef struct Ui_Button_State {
    B8 is_pressed         : 1;
    B8 was_pressed        : 1;
    B8 got_pressed        : 1;
    B8 got_unpressed      : 1;
	B8 is_hovered         : 1;
	B8 was_hovered        : 1;
	B8 got_hovered        : 1;
	B8 got_unhovered      : 1;

	B8 is_focussed        : 1;
	B8 was_focussed       : 1;
	B8 got_focussed       : 1;
	B8 got_unfocussed     : 1;
	B8 got_clicked        : 1;
	B8 got_double_clicked : 1;
	B8 got_dragged        : 1;
	B8 got_undragged      : 1;
} Ui_Button_State;

#define UI_GLOBAL static

/*

parent.rect.width = max(child.width)
parent.rect.height = sum(child.height)

parent.rect.wifth = sum(child.width)
parent.rect.height = ? max(child.height) : fixed_height

child.rect.width =  text_width
child.rect.height = parent.height


*/

/* F32_RIGHT, F32_LEFT, F32_CENTER, F32_CENTER, V2_CENTER, V2_MIDDLE_RIGHT V2_TOP_RIGHT V2_BOTTOM_LEFT */

// typedef enum Ui_Node_Flag_ {
//     UI_NODE_FLAG_MONOSPACE
//     UI_NODE_FLAG_USE_TEXT_WIDTH
//     UI_NODE_FLAG_USE_TEXT_HEIGHT
// } Ui_Node_Flag_;

typedef struct Ui_Aabb2 {
    F32 x0;
    F32 y0;
    F32 x1;
    F32 y1;
} Ui_Aabb2;

typedef U8 Ui_Node_Kind;
typedef enum Ui_Node_Kind_ {
    UI_NODE_KIND_REGULAR,
    UI_NODE_KIND_ROOT,
    UI_NODE_KIND_CONTEXT_MENU,
    UI_NODE_KIND_BAR,
    UI_NODE_KIND_DROPDOWN_MENU,
} Ui_Node_Kind_;

typedef U8 Ui_Cursor;
typedef enum Ui_Cursor_ {
    UI_CURSOR_DEFAULT,
    UI_CURSOR_POINT,
    UI_CURSOR_GRAB,
    UI_CURSOR_GRABBING,
    UI_CURSOR_COUNT,
    UI_CURSOR_INVALID = UI_CURSOR_COUNT
} Ui_Cursor_;

typedef struct Ui_Node_Bar {
    U8 anchor;
} Ui_Node_Bar;

typedef U8 Ui_Docking_Axis;
typedef enum Ui_Docking_Axis_ {
    UI_DOCKING_AXIS_VERTICAL   = 0,
    UI_DOCKING_AXIS_HORIZONTAL = 1,
} Ui_Docking_Axis_;

typedef struct Ui_Node_Docking {
    Ui_Docking_Axis axis : 1; /* x===0, y===1 */
} Ui_Node_Docking;

typedef enum Ui_Size_Unit Ui_Size_Unit;
enum Ui_Size_Unit_ {
    UI_SIZE_UNIT_PIXEL,
    UI_SIZE_UNIT_EM_QUADRAT,
    UI_SIZE_UNIT_PARENT,
    UI_SIZE_UNIT_CONTENT,
};

// typedef ui Ui_Dimension {
//     __m128
// }

// __m128

// typedef struct Ui_Size1 {
//     F32 px;
//     F32 em;
//     F32 pr;
//     F32 ch;
// } Ui_Size1;

// typedef struct Ui_Size2 {
//     Ui_Size_Unit unit;
//     F32          x;
//     F32          y;
// } Ui_Size1;


#define UI_PX(num_pixels)
#define UI_EM(num_pixels)
#define UI_PR(num_pixels)
#define UI_CH(num_pixels)

typedef struct Ui_Node_Style {
    V2   size;
    C32  corner_colors[4];
    C32  border_colors[4];
    F32  padding[4];
    F32  outset[4];
    V2   translate;
    C32  text_color;
    F32  ignore_layout;
    F32  border_widths[4];
    V2   child_spacing;
    V2   layout_direction;
    F32  align_main;
    F32  align_sub;
    B8  skip_draw_background : 1;
    B8  skip_draw            : 1;
} Ui_Node_Style;

typedef struct Ui_Node Ui_Node;
typedef struct Ui_Node {
    Ui_Hash hash;

    Ui_Node *parent;
    Ui_Node *next;
    Ui_Node *previous;
    Ui_Node *first;
    Ui_Node *last;

    /* Collision Detection */
    size_t last_touched_frame_generation;

    
    /* Size */
    /* width_in_pixel, height_in_pixel */
    /* width_in_parent_percentage, in_parent_percentage */
    V2  size; /* size_min, size_max */
    C32 corner_colors[4];
    C32 border_colors[4];
    F32 padding[4];
    F32 outset[4];
    V2  translate;
    C32 text_color;
    F32 text_padding; /* Can be replaced with Padding only */
    F32 ignore_layout;
    //F32 border_width;
    F32 border_widths[4];
    V2  child_spacing;
    // F32 corner_radii [4]; /* Todo */


    //V2 content_size;

    V2 layout_direction;
    //V2 alignment_direction; /* deprecated */
    F32 align_main;
    F32 align_sub;
    
    /* Text */
    S8  name;
    S8 *lines;
    S8  num_lines;

    /* Flags */
    B8           is_node_first_frame : 1;
    Ui_Node_Kind kind : 4;
    B8           skip_draw_background : 1;
    B8           skip_draw            : 1;

    /* Per construction */
    Ui_Aabb2 aabb;
    V2       content_num_px_sum;
    V2       content_num_px_max;
    /* @MEMORY(menyuu) Put this on the stack at construction. */
    V2       cursor;
    V2       empty;
    F32      empty_consumed;

    union As {
        Ui_Node_Bar     bar;
        Ui_Node_Docking docking;
    } as;
} Ui_Node;
#define UI_NODE_STYLE_PTR_FROM_NODE(node_ptr) (&(node_ptr)->style)

typedef struct Ui_Style {
    V2  layout_direction;
    V2  alignment_direction; /* fixme: rename to align main, align sub*/
    V2  size;
    C32 color;
    C32 text_color;
    F32 text_padding;
} Ui_Style;
static const Ui_Style UI_STYLE_DEFAULT = {
    /* layout_direction    */ {0.f, -1.f},
    /* alignment_direction */ {0.0f, 0.0f},
    /* size                */ {0.f,  0.f},
    /* color               */ C32_HEX(0x735f8b),
    /* text_color          */ C32_HEX(0xFFFFFF),
    /* text_padding        */ 4
};
#define UI_DEFER_STYLE_LOOP(ui, type, field, value) for(size_t _i_ = ((*((type*)ui_style_push((ui), &(((Ui*)(0))->style.field), sizeof(type))) = (value)), 0); !_i_; _i_ = (ui_style_pop((ui), &((Ui*)(0))->style.field, sizeof(type)), 1))
#define UI_LAYOUT_DIRECTION(ui, layout_direction_v2) UI_DEFER_STYLE_LOOP(ui, V2 , layout_direction     , (layout_direction_v2))
#define UI_ALIGNMENT(ui, alignment_v2)               UI_DEFER_STYLE_LOOP(ui, V2 , alignment_direction  , (alignment_v2       ))
#define UI_COLOR(ui, color_c32)                      UI_DEFER_STYLE_LOOP(ui, C32, color                , (color_c32          ))
#define UI_TEXT_COLOR(ui, text_color_c32)            UI_DEFER_STYLE_LOOP(ui, C32, text_color           , (text_color_c32     ))
#define UI_TEXT_PADDING(ui, text_padding_f32)        UI_DEFER_STYLE_LOOP(ui, F32, text_padding         , (text_padding_f32   ))
#define UI_WIDTH(ui, width_f32)                      UI_DEFER_STYLE_LOOP(ui, F32, size.x               , (width_f32          ))
#define UI_HEIGHT(ui, height_f32)                    UI_DEFER_STYLE_LOOP(ui, F32, size.y               , (height_f32         ))
#define UI_JUSTIFY(ui, ...) /* todo */
#define UI_ALIGN_MAIN(ui, f32_align_main)            UI_DEFER_STYLE_LOOP(ui, F32, alignment_direction.x, (f32_align_main     ))
#define UI_ALIGN_SUB(ui, f32_align_sub)              UI_DEFER_STYLE_LOOP(ui, F32, alignment_direction.y, (f32_align_sub      ))

#ifndef UI_DOCK_MIN_SIZE
#define UI_DOCK_MIN_SIZE 16
#endif /* UI_DOCK_MIN_SIZE */
#ifndef UI_DOCK_SPLIT_HITBOX_THICKNESS
#define UI_DOCK_SPLIT_HITBOX_THICKNESS 2
#endif /* UI_DOCK_SPLIT_HITBOX_THICKNESS */

typedef struct Ui_Dock_Subdivisions Ui_Dock_Subdivisions;
typedef struct Ui_Dock Ui_Dock;

#ifdef OLD_DOCK
typedef struct Ui_Dock {
    Ui_Dock *parent;
    Ui_Dock *next;
    Ui_Dock *previous;
    Ui_Dock *first;
    Ui_Dock *last;
    Ui_Docking_Axis axis;

    Ui_Aabb2 aabb;

    Ui_Hash key; /* ui key */

    F32 split;

    U8 undocked : 1;

    /* "draw" */
    Ui_Node *content;

    /* undocked only */
    size_t width;
    size_t height;
} Ui_Dock;
#else
typedef struct Ui_Dock_Subdivisions {
    Ui_Dock        *first;
    Ui_Dock        *last;
    Ui_Docking_Axis axis;
} Ui_Dock_Subdivisions;
typedef struct Ui_Dock {
    Ui_Dock             *next;
    Ui_Dock             *previous;
    Ui_Dock             *parent;
    Ui_Dock_Subdivisions subdivisions;
    F32                  split;
    Ui_Node              root;
    void                *userdata;
} Ui_Dock;
#endif

#define UI_DRAG_NUM_DROP_TARGETS_MAX 2

typedef struct Ui_Drag_Drop_Target {
    V2      position;
    Ui_Hash hash;
} Ui_Drag_Drop_Target;

typedef struct Ui_Drag {
    Ui_Hash             active;
    Ui_Hash             last;
    V2                  origin;
    V2                  delta;
    V2                  last_delta;
    U8                  anchor;
    F32                 since_begin;
    F32                 since_end;
    V2                  parallel_universe_cursor; /* used for drop */
    F32                 drop_preview_previous_normalized_lerp;
    F32                 drop_preview_empty_consumed;
    size_t              num_drop_targets;
    Ui_Drag_Drop_Target drop_targets[UI_DRAG_NUM_DROP_TARGETS_MAX];
} Ui_Drag;

typedef struct Ui_Context_Menu_Flip {
    B8 x : 1;
    B8 y : 1;
} Ui_Context_Menu_Flip;

typedef struct Ui Ui;
typedef struct Ui_Shared Ui_Shared;

typedef struct Ui_Io {
    /* Inputs */
    V2      mouse_position;
    B8      lmb_got_pressed : 1;
    B8      lmb_got_released : 1;
    B8      lmb_is_pressed : 1;
    B8      rmb_got_pressed : 1;
    B8      rmb_got_released : 1;
    B8      cursor_is_occluded : 1;
    Ui_Drag drag;

    /* Outputs */
} Ui_Io;

/* Ui_Context */
typedef struct Ui {
    /* Const Across Frame */
    Ui_Shared *shared;
    size_t     seed;
    
    /* roots */
    Ui_Node *dropdown_menu; /* @SHARED */
    
    /* Context State */
    Ui_Node  *root;
    Ui_Node  *head;
    Ui_Node  *last;
    Ui_Hash   active_dropdown_menu; /* @SHARED @PER_USER */
    Ui_Cursor next_hover_cursor;
    C32       next_hover_color;
    Bool      next_hover_color_exists : 1;
    
    Ui_Hash   hash;

    /* Style */
    Ui_Style style;
    size_t   style_head;
    U8       style_stack[1024];
} Ui;

/* Ui_Shared_Context */
typedef struct Ui_Shared {
    /* Time */
    size_t    frame_generation;
    F32       time_delta;
    Ui_Cursor cursor;

    /* Memory */
    size_t    build_num_transient;
    Ui_Node  nodes[4096 * 2]; /* shared */
    //Ui_Node  nodes_persistent[1024]; /* accessed by hash  */
    Ui_Node  nodes_transient[256];   /* shared */ /* accessed by stack */

    /* Inputs */
    V2      mouse_position;
    B8      lmb_got_pressed : 1;
    B8      lmb_got_released : 1;
    B8      lmb_is_pressed : 1;
    B8      rmb_got_pressed : 1;
    B8      rmb_got_released : 1;
    B8      cursor_is_occluded : 1;
    Ui_Drag drag;
    
    /* Clicks */
    Ui_Hash   active_node;     /* @SHARED @PER_USER */
    Ui_Hash   active_node_rmb; /* @SHARED @PER_USER */

    /* Context Menu */
    size_t               num_context_menus;
    Ui_Hash              active_context_menu;
    V2                   context_menu_anchor;
    Ui_Context_Menu_Flip context_menu_flip;
    size_t               context_menu_generation;
    Ui_Node             *context_menu_root;
    C32                  context_menu_color;
    C32                  context_menu_text_color;
    void                *context_menu_userdata; /* payload */

    /* Globals */
    Ui      *uis;

    /* Docks / Area */
    Arena   *retained_arena;
    Ui_Dock root_dock;
    //Ui_Dock_Subdivisions dock_subdivisions;
    Ui_Dock *dock_free_first;
    Ui_Dock *dock_free_last;
} Ui_Shared;

Ui *ui_get(Ui_Shared *shared, size_t logical_core_index) {
    Ui *ui = &shared->uis[logical_core_index];
    return ui;
}

void ui_node_clear_per_build_data(Ui_Node *node) {
    node->next                 = 0;
    node->previous             = 0;
    node->first                = 0;
    node->last                 = 0;
    node->cursor               = V2_ZERO;
    node->content_num_px_sum     = V2_ZERO;
    node->content_num_px_max     = V2_ZERO;
    node->empty                = V2_ZERO;
    node->ignore_layout        = 0.f;
    node->name.length          = 0;
    node->skip_draw_background = 0;
    node->skip_draw            = 0;
    memset(&node->padding, 0, sizeof(node->padding));
    memset(&node->outset , 0, sizeof(node->outset));
    node->translate            = V2_ZERO;
    //memset(&node->aabb, 0, sizeof(node->aabb));
}

V2 ui_node_prefered_size(Ui_Node *node) {
    V2 size;    
    size.x = lerp(node->content_num_px_max.x, node->content_num_px_sum.x, fabsf(node->layout_direction.x));
    size.y = lerp(node->content_num_px_max.y, node->content_num_px_sum.y, fabsf(node->layout_direction.y));
    size.x = fmaxf(size.x, node->size.x);
    size.y = fmaxf(size.y, node->size.y);
    return size;
}

/* Shared */

Ui_Node *ui_shared_get_node(Ui_Shared *shared, Ui_Hash hash) {
    Ui_Node *node = &shared->nodes[hash & (sizeof(shared->nodes) / sizeof(shared->nodes[0]) - 1)];
    UI_ASSERT(node->hash == hash);
    return node;
}

/* Drag */

Bool ui_shared_drag_is_ongoing_any(Ui_Shared *shared) {
    return shared->drag.active != UI_HASH_INVALID;
}

void ui_shared_drag_begin(Ui_Shared *shared, Ui_Hash hash) {
    shared->drag.active      = hash;
    shared->drag.since_begin = 0;
}

void ui_shared_drag_end(Ui_Shared *shared) {
    shared->drag.last = shared->drag.active;
    shared->drag.last_delta = shared->drag.delta;
    shared->drag.active = UI_HASH_INVALID;
    shared->drag.since_end = 0;
}

void ui_shared_init(
    Ui_Shared *shared,
    //size_t thread_local_storage_index,
    size_t num_logical_cores,
    Arena *arena_retained,
    Ui_Style *style
) {
    shared->uis = ARENA_INSERT_ARRAY(arena_retained, Ui, num_logical_cores);
    shared->retained_arena = arena_retained;
    for (
        size_t logical_core_index = 0;
        logical_core_index != num_logical_cores;
        ++logical_core_index
    ) {
        memcpy(
            &shared->uis[logical_core_index].style,
            style ? style : &UI_STYLE_DEFAULT,
            sizeof(Ui_Style)
        );
    }
    shared->drag.active = UI_HASH_INVALID;
}

/* The shared context should be updated once per frame, and then
 * read only until next frame boundary. */
void ui_shared_update(
    Ui_Shared *ui_shared,
    V2         mouse_position, 
    B8         lmb_got_pressed, 
    B8         lmb_got_released,
    B8         lmb_is_pressed,
    B8         rmb_got_pressed,
    B8         rmb_got_released,
    B8         cursor_is_occluded,
    F32        time_delta
) {
    /* Memory */
    ui_shared->build_num_transient = 0;

    /* Frame Generation */
    ui_shared->frame_generation += 1;
    ui_shared->time_delta        = time_delta;
    
    /* Cursor */
    ui_shared->cursor = UI_CURSOR_INVALID;

    /* Inputs */
    {
        ui_shared->mouse_position     = mouse_position;
        ui_shared->lmb_got_pressed    = lmb_got_pressed;
        ui_shared->lmb_got_released   = lmb_got_released;
        ui_shared->lmb_is_pressed     = lmb_is_pressed;
        ui_shared->rmb_got_pressed    = rmb_got_pressed;
        ui_shared->rmb_got_released   = rmb_got_released;
        ui_shared->cursor_is_occluded = cursor_is_occluded;
        /* Drag */
        {
            if(lmb_got_pressed) {
                ui_shared->drag.origin = mouse_position;
            }
            ui_shared->drag.delta = v2_sub(mouse_position, ui_shared->drag.origin);
            ui_shared->drag.since_begin += time_delta;
            ui_shared->drag.since_end   += time_delta;
        }
    }
}

void ui_shared_end(
    Ui_Shared *ui_shared
) {
    /* Drag */
    {
        if(ui_shared->lmb_got_released) {
            if(ui_shared_drag_is_ongoing_any(ui_shared)) {
                ui_shared_drag_end(ui_shared);
            }
        }

        if(ui_shared_drag_is_ongoing_any(ui_shared)) {
            Ui_Node *node = ui_shared_get_node(ui_shared, ui_shared->drag.active);
            node->translate = ui_shared->drag.delta;
        }

        if (
            ui_shared->drag.last &&
            ui_shared->drag.last != UI_HASH_INVALID &&
            ui_shared->drag.last != ui_shared->drag.active
        ) {
            Ui_Node *node = ui_shared_get_node(ui_shared, ui_shared->drag.last); /* danger */
            V2 drag_delta = ui_shared->drag.last_delta;
            drag_delta = v2_clamp_magnitude(drag_delta, 8.0f);
            F32 anim;
            anim = ui_shared->drag.since_end * 8.0f;
            anim = 1.f - fminf(anim, 1.f);
            anim = ease_in_overshoot_smooth(anim, -4.0f);
            node->translate.x = drag_delta.x * anim;
            node->translate.y = drag_delta.y * anim;
        }
    }
}

void ui_frame_end() {

}

void ui_node_begin_frame(Ui *ui, Ui_Node *node, Ui_Hash hash) {
    node->last_touched_frame_generation = ui->shared->frame_generation;

    node->is_node_first_frame = node->hash != hash;
    node->hash = hash;

    /* Set fields according to style */
    node->size                = ui->style.size;
    node->layout_direction    = ui->style.layout_direction;
    node->corner_colors[0]    = ui->style.color;
    node->corner_colors[1]    = ui->style.color;
    node->corner_colors[2]    = ui->style.color;
    node->corner_colors[3]    = ui->style.color;
    node->text_color          = ui->style.text_color;
    node->text_padding        = ui->style.text_padding;
    //node->border_width        = 0;
    node->border_widths[0]    = 0;
    node->border_widths[1]    = 0;
    node->border_widths[2]    = 0;
    node->border_widths[3]    = 0;
    node->align_main          = ui->style.alignment_direction.x;
    node->align_sub           = ui->style.alignment_direction.y;
    node->child_spacing       = V2_ZERO; /* fixme */

    return node;
}

void ui_set_head(Ui *ui, Ui_Node *head) {
    ui->head = head;
    if (head)
        ui->hash = head->hash;
    else
        ui->hash = 0;
}

void ui_begin(
    Ui *ui,
    Ui_Shared *shared,
    Ui_Node *root,
    Ui_Hash seed
) {
    /* Shared */
    ui->shared = shared;
    
    /* Root */
    if (root) {
        ui_set_head(ui, root);
        ui->last = root;
        ui_node_clear_per_build_data(root);
        ui_node_begin_frame(ui, root, seed);
        root->skip_draw_background = 1;
    }

    ui->seed = seed;
    ui->hash = seed;
    
    /* Inner Logic */
    ui->next_hover_cursor = UI_CURSOR_INVALID;
}

void ui_end(Ui *ui) { /* @SPEED, could skip */
    ui->head   = 0;
    ui->hash   = 0;
    ui->shared = 0;
    return;
}

/* Id */
Ui_Hash ui_hash(Ui_Hash parent, S8 name) {
  Ui_Hash hash = parent;
  for(size_t i = 0; i < name.length; ++i) {
    hash = ((hash << 5ull) + hash) + name.data[i];
  }
  hash ^= 0xAAAAAAAAAAAAAAAAull;
  return hash;
}

/* Node */

void ui_node_insert_child(Ui_Node *parent, Ui_Node *child) {
    if(parent->last) {
        Ui_Node *last   = parent->last;
        last->next      = child;
        child->previous = last;
    } else {
        parent->first = child;
    }
    parent->last  = child;
    child->parent = parent;
}

Ui_Node *ui_node(Ui *ui, Ui_Hash hash) {
    Ui_Node *node = &ui->shared->nodes[hash & (sizeof(ui->shared->nodes) / sizeof(ui->shared->nodes[0]) - 1)];
    UI_ASSERT(
        node->last_touched_frame_generation != ui->shared->frame_generation, 
        "Node %llu (\"%.*s\") was already touched this frame.\n"
        "This is likely a hash collision.",
        hash,
        node->name.length,
        node->name.data
    );
    ui_node_begin_frame(ui, node, hash);
    return node;
}

Ui_Node *ui_node_from_string(Ui *ui, S8 str) {
    Ui_Hash hash = ui_hash(ui->hash, str);
    /* begin debug */
    {
        Ui_Node *node = &ui->shared->nodes[hash & (sizeof(ui->shared->nodes) / sizeof(ui->shared->nodes[0]) - 1)];
        UI_ASSERT(
            node->last_touched_frame_generation != ui->shared->frame_generation, 
            "Node %llu (\"%.*s\") was already touched this frame.\n"
            "This is likely a hash collision between \"%.*s\" and \"%.*s\" (%llu).",
            hash,
            node->name.length,
            node->name.data,
            node->name.length,
            node->name.data,
            str.length,
            str.data,
            hash
        );
    }
    /* end debug */
    return ui_node(ui, hash);
}

Ui_Node *ui_build_node_transient(Ui *ui, Ui_Node *parent, Ui_Hash hash) {
    size_t index = ++ui->shared->build_num_transient;
    Ui_Node *node = &ui->shared->nodes_transient[index];
    ui_node_begin_frame(ui, node, hash);
    ui_node_clear_per_build_data(node);
    if(parent)
        ui_node_insert_child(parent, node);
    return node;
}

/* Ui tree traversal */

Ui_Node *ui_node_traversal_depth_first_pre_order_next(Ui_Node *node) {
    if(node->first)
        return node->first;
    if(node->next)
        return node->next;
    if(node->parent)
        return node->parent->next;
    return 0;
}

Ui_Node *ui_node_traversal_depth_first_pre_order_next_skip_subtree(Ui_Node *node) {
    if(node->next)
        return node->next;
    if(node->parent)
        return node->parent->next;
    return 0;
}

Ui_Node *ui_node_next_depth_first_post_order(Ui_Node *node) {
    if(node->next) {
        node = node->next;
        while(node->first)
            node = node->first;
        return node;
    }
    if(node->parent)
        return node->parent;
    return 0;
}

Ui_Node *ui_node_traversal_depth_first_post_order_start(Ui_Node *node) {
    while(node->first)
        node = node->first;
    return node;
}

Ui_Node *ui_node_traversal_depth_first_post_order_next(Ui_Node *node) {
    if(node->next) {
        node = node->next;
        while(node->first)
            node = node->first;
        return node;
    }
    if(node->parent)
        return node->parent;
    return 0;
}

Ui_Node *ui_node_traversal_depth_first_post_order_reverse_start(Ui_Node *node) {
    while(node->last)
        node = node->last;
    return node;
}

Ui_Node *ui_node_traversal_depth_first_post_order_reverse_next(Ui_Node *node) {
    if(node->previous) {
        node = node->previous;
        while(node->last)
            node = node->last;
        return node;
    }
    if(node->parent)
        return node->parent;
    return 0;
}

/* Style */

void *ui_style_push(Ui *ui, ptrdiff_t field_offset, size_t size) {
    void *stack = &ui->style_stack[ui->style_head];
    ui->style_head += size;
    UI_ASSERT(ui->style_head <= sizeof(ui->style_stack), "Out of ui style stack memory.");
    memcpy(stack, (char*)ui + field_offset, size);
    return ((char*)ui) + field_offset;
}

void ui_style_pop(Ui *ui, ptrdiff_t field_offset, size_t size) {
    ui->style_head -= size;
    void *stack = &ui->style_stack[ui->style_head];
    memcpy(((char*)ui) + field_offset, stack, size);
}

void *ui_member_push(Ui *ui, ptrdiff_t member_offset, size_t size) {
    void *stack = &ui->style_stack[ui->style_head];
    ui->style_head += size;
    UI_ASSERT(ui->style_head <= sizeof(ui->style_stack), "Out of ui stack memory.");
    memcpy(stack, (char*)ui + member_offset, size);
    return ((char*)ui) + member_offset;
}

void ui_member_pop(Ui *ui, ptrdiff_t member_offset, size_t size) {
    ui->style_head -= size;
    void *stack = &ui->style_stack[ui->style_head];
    memcpy(((char*)ui) + member_offset, stack, size);
}
#define UI_MEMBER_PUSH(ui, member) ui_member_push(ui, &((Ui*)0)->member, sizeof(((Ui*)0)->member))
#define UI_MEMBER_POP(ui, member)  ui_member_pop (ui, &((Ui*)0)->member, sizeof(((Ui*)0)->member))


Bool ui_aabb_is_point_inside(Ui_Aabb2 aabb, V2 point) {
    return
        point.x >= aabb.x0 &&
        point.x <  aabb.x1 &&
        point.y >  aabb.y0 &&
        point.y <= aabb.y1;
}

Ui_Event ui_event_from_node(Ui *ui, Ui_Node *node, B8 lmb_disabled, B8 rmb_enabled) {
    Ui_Event event = {0};
    event.is_hovered =
        !ui->shared->cursor_is_occluded &&
        ui_aabb_is_point_inside(node->aabb, ui->shared->mouse_position);

    if (
        ui->shared->active_context_menu &&
        ui->shared->context_menu_root &&
        ui->shared->context_menu_root != ui->root
    ) {
        event.is_hovered &=
            !ui_aabb_is_point_inside(
                ui->shared->context_menu_root->aabb,
                ui->shared->mouse_position);
    }
    
    if(
        event.is_hovered &&
        ui->next_hover_cursor != UI_CURSOR_INVALID //&&
        //ui->cursor == UI_CURSOR_INVALID
    )
        ui->shared->cursor = ui->next_hover_cursor;
    else
        ui->next_hover_cursor = UI_CURSOR_INVALID;
    event.got_pressed = event.is_hovered && !lmb_disabled && ui->shared->lmb_got_pressed;
    Bool got_pressed_rmb = event.is_hovered && rmb_enabled && ui->shared->rmb_got_pressed;
    if(event.got_pressed)
        ui->shared->active_node = node->hash;
    if(got_pressed_rmb)
        ui->shared->active_node_rmb = node->hash;
    event.is_pressed = ui->shared->active_node == node->hash;
    event.got_released = ui->shared->active_node == node->hash && ui->shared->lmb_got_released;
    event.clicked = event.is_hovered && event.got_released;
    event.clicked_rmb = event.is_hovered && ui->shared->rmb_got_released && ui->shared->active_node_rmb == node->hash;
    if(ui->shared->active_node == node->hash && ui->shared->lmb_got_released)
        ui->shared->active_node = 0;
    if(ui->shared->active_node_rmb == node->hash && ui->shared->rmb_got_released)
        ui->shared->active_node_rmb = 0;
    event.is_pressed     = ui->shared->active_node     == node->hash;
    event.is_pressed_rmb = ui->shared->active_node_rmb == node->hash;
    //event.drag = event.is_pressed && !event.is_hovered;
    event.drag =
        node->hash != ui->shared->drag.active &&
        event.is_pressed &&
        v2_magnitude_squared(ui->shared->drag.delta) >= 6.f * 6.f;

    event.drop = ui->shared->lmb_got_released;
    //event.is_dragged  = ui->dr
    //event.got_dropepd = event.
    ui->next_hover_cursor = UI_CURSOR_INVALID;
    return event;
}

void ui_next_hover_color(Ui *ui, C32 color) {
    ui->next_hover_color_exists = 1;
    ui->next_hover_color        = color;
}

/* Cursor */
/* will be consumed by the next ui_event_from_node() call */
void ui_next_hover_cursor(Ui *ui, Ui_Cursor cursor) {
    ui->next_hover_cursor = cursor;
}

#pragma region Dock

Ui_Dock *ui_shared_dock_create(Ui_Shared *shared, Ui_Dock *parent) {
    Ui_Dock *dock;
    if(shared->dock_free_first) {
        dock = shared->dock_free_first;
        shared->dock_free_first = dock->next;
    } else {
        dock = ARENA_INSERT_STRUCT_NOZEROED(shared->retained_arena, Ui_Dock);
    }
    memset(dock, 0, sizeof(*dock));
    dock->parent = parent;
    return dock;
}

void ui_dock_split(Ui_Dock *dock, size_t subdivisions, Ui_Dock **sub_docks) {

}

void ui_dock_demo(Ui_Dock *dock) {
    Ui_Dock **sub_docks = alloca(sizeof(Ui_Dock*) * 2);
    ui_dock_split(dock, 2, sub_docks);

    Ui_Dock *b = sub_docks[0];
    Ui_Dock *c = sub_docks[1];
}

void ui_dock_begin(Ui_Dock *dock);
void ui_dock_end(Ui_Dock *dock);

typedef U8 Ui_Dock_Action_Kind;
typedef enum Ui_Dock_Action_Kind_ {
    UI_DOCK_ACTION_KIND_NONE_ONGOING,
    UI_DOCK_ACTION_KIND_DRAGGING,
    UI_DOCK_ACTION_KIND_DELETING,
    UI_DOCK_ACTION_KIND_COUNT,
} Ui_Dock_Action_Kind_;
typedef struct Ui_Dock_Action {
    Ui_Dock            *active;
    Ui_Dock_Action_Kind kind;
} Ui_Dock_Action;
void ui_dock_action_drag_begin(Ui_Dock_Action *action, Ui_Dock *target) {
    action->kind   = UI_DOCK_ACTION_KIND_DRAGGING;
    action->active = target;
}
B8 ui_dock_action_drag_ongoing_on_target(Ui_Dock_Action *action, Ui_Dock *target) {
    return
        action->kind   == UI_DOCK_ACTION_KIND_DRAGGING &&
        action->active == target;
}
void ui_dock_action_delete_begin(Ui_Dock_Action *action, Ui_Dock *target) {
    action->kind   = UI_DOCK_ACTION_KIND_DELETING;
    action->active = target;
}
B8 ui_dock_action_delete_ongoing_on_target(Ui_Dock_Action *action, Ui_Dock *target) {
    return
        action->kind   == UI_DOCK_ACTION_KIND_DELETING &&
        action->active == target;
}
void ui_dock_action_end(Ui_Dock_Action *action) {
    action->kind   = UI_DOCK_ACTION_KIND_NONE_ONGOING;
}

typedef struct Ui_Dock_Job {
    /* In */
    Ui_Shared *shared;
    Ui_Dock_Subdivisions *dock_subdivisions_first;
    Ui_Dock_Subdivisions *dock_first;
    /* input */
    /* command_queue */
    V2 mouse_position;
    Os_Cursor *cursor;
    Ui_Docking_Axis *axis;
    Ui_Aabb2 aabb;
    //Ui_Dock **active;
    Ui_Dock_Action *action;
    B8 lmb_got_pressed  : 1;
    B8 lmb_got_released : 1;
    B8 lmb_is_pressed  : 1;
    B8 rmb_got_pressed  : 1;
    B8 rmb_got_released : 1;
    B8 is_splitting     : 1;
    B8 is_occluded      : 1;
    
    /* Out */
    Ui_Aabb2 *dock_0_aabb;
    size_t    num_docks_max;
    size_t   *num_docks;
    Ui_Dock **docks;
    Ui_Aabb2 *dock_aabbs;
    Bool      is_occluding_cursor : 1;
    size_t    num_created_docks;
    Ui_Dock  *created_docks[2];
} Ui_Dock_Job;
#define UI_DOCK_MAX_DEPTH 64
void ui_dock_job_validate(Ui_Dock_Job *job) {
    UI_ASSERT(job->shared);
    UI_ASSERT(job->action);
    UI_ASSERT(job->dock_0_aabb);
    UI_ASSERT(job->dock_first);
    // static size_t num_call_with_rmb_got_pressed_not_set = 0;
    // if (job->rmb_got_pressed)
    //     num_call_with_rmb_got_pressed_not_set = 0;
    // else
    //     ++num_call_with_rmb_got_pressed_not_set;
    // if (num_call_with_rmb_got_pressed_not_set > 600) {
    //     os_error("Warning", "Are you sure you are updating rmb_got_pressed properly?");
    //     num_call_with_rmb_got_pressed_not_set = 0;
    // }
}
void ui_dock_job(Ui_Dock_Job *job) {
    
    #define UI_DOCK_JOB_MIN(relative) ((F32*)((ptrdiff_t)&(relative) + min_offset_fox_axis[*job->axis]))
    #define UI_DOCK_JOB_MAX(relative) ((F32*)((ptrdiff_t)&(relative) + max_offset_fox_axis[*job->axis]))

    #define UI_DOCK_MIN(relative) ((F32*)((ptrdiff_t)&(relative) + min_offset_fox_axis[entry.subdivisions->axis]))
    #define UI_DOCK_MAX(relative) ((F32*)((ptrdiff_t)&(relative) + max_offset_fox_axis[entry.subdivisions->axis]))
    #define UI_DOCK_TANGENT_MIN(relative) ((F32*)((ptrdiff_t)&(relative) + min_offset_fox_axis[!entry.subdivisions->axis]))
    #define UI_DOCK_TANGENT_MAX(relative) ((F32*)((ptrdiff_t)&(relative) + max_offset_fox_axis[!entry.subdivisions->axis]))

    if(job->rmb_got_pressed) {
        *job->axis = !*job->axis;
    }

    typedef struct Ui_Dock_Queue { 
        Ui_Aabb2              aabb;
        Ui_Dock_Subdivisions *subdivisions;
        Ui_Dock_Subdivisions *parent;
        Ui_Dock              *parent_dock;
        B8                    is_hovering_parent : 1;
        B8                    is_hovering_split  : 1;
    } Ui_Dock_Queue;
    Ui_Dock_Queue queue[UI_DOCK_MAX_DEPTH];
    queue[0].subdivisions       = job->dock_subdivisions_first;
    queue[0].aabb               = job->aabb;
    queue[0].parent             = 0;
    queue[0].parent_dock        = job->dock_first;
    queue[0].is_hovering_parent = 1;
    queue[0].is_hovering_split  = 0;
    size_t num_entry = 1;

    Bool lmb_got_pressed = job->lmb_got_pressed;
    
    ptrdiff_t min_offset_fox_axis[2] = {&((Ui_Aabb2*)0)->x0, &((Ui_Aabb2*)0)->y0};
    ptrdiff_t max_offset_fox_axis[2] = {&((Ui_Aabb2*)0)->x1, &((Ui_Aabb2*)0)->y1};
    Os_Cursor cursor_for_axis    [2] = {OS_CURSOR_HORIZONTAL, OS_CURSOR_VERTICAL};

    Bool dock0 = 0;
    //Bool hovering_any_split = 0;
    for(; num_entry;) {
        /* pop an entry from the queue */
        --num_entry;
        Ui_Dock_Queue entry = queue[num_entry];
        if (
            job->action->kind == UI_DOCK_ACTION_KIND_NONE_ONGOING &&
            entry.is_hovering_parent && !entry.is_hovering_split
        ) {            
            if (entry.subdivisions->first && entry.subdivisions->axis == *job->axis) {
                F32 split_min = *UI_DOCK_JOB_MIN(entry.aabb) + UI_DOCK_MIN_SIZE;
                F32 split_max = *UI_DOCK_JOB_MAX(entry.aabb) - UI_DOCK_MIN_SIZE;
                F32 split     = *UI_DOCK_JOB_MIN(job->mouse_position);
                F32 normalized_split = normalize_to_range(split_min, split_max, split);
                /* find insertion location */
                Ui_Dock* next_sibling = entry.subdivisions->first;
                for (
                    ;
                    next_sibling && next_sibling->next;
                    next_sibling = next_sibling->next
                ) {
                    if (normalized_split < next_sibling->split) {
                        break;
                    }
                }
                if(
                    job->is_splitting &&
                    !next_sibling->subdivisions.first &&
                    normalized_split >= 0 &&
                    normalized_split <= 1
                ) {
                    #ifdef UI_ENABLE_GEOPTRINT
                    if(*job->axis == UI_DOCKING_AXIS_VERTICAL) {
                        debug_segment2(
                            1,
                            v2(split, entry.aabb.y0),
                            v2(split, entry.aabb.y1),
                            C32_GRAY
                        );
                    } else {
                        debug_segment2(
                            1,
                            v2(entry.aabb.x0, split),
                            v2(entry.aabb.x1, split),
                            C32_GRAY
                        );
                    }
                    #endif /* UI_ENABLE_GEOPTRINT */

                    if(lmb_got_pressed) {
                        /* eat input */
                        lmb_got_pressed = 0;
                        Ui_Dock *new_dock = ui_shared_dock_create(job->shared, entry.parent_dock);
                        UI_ASSERT(new_dock->parent == entry.parent_dock);

                        new_dock->split = normalized_split;

                        /* find insertion location */
                        Ui_Dock* next_sibling = entry.subdivisions->first;
                        for (
                            ;
                            next_sibling && next_sibling->next;
                            next_sibling = next_sibling->next
                        ) {
                            if (normalized_split < next_sibling->split) {
                                break;
                            }
                        }
                        
                        if (next_sibling == entry.subdivisions->first) {
                            entry.subdivisions->first = new_dock;
                        }
                        if (next_sibling->previous) {
                            next_sibling->previous->next = new_dock;
                        }
                        new_dock->previous = next_sibling->previous;
                        new_dock->next = next_sibling;
                        next_sibling->previous = new_dock;

                        UI_ASSERT(
                            !new_dock->previous ||
                            (new_dock->previous->parent == new_dock->parent)
                        );
                        UI_ASSERT(
                            !new_dock->next ||
                            (new_dock->next->parent == new_dock->parent)
                        );
                        
                        job->created_docks[0] = new_dock;
                        job->num_created_docks = 1;
                    }
                }
            } else if (
                job->is_splitting &&
                !entry.subdivisions->first &&
                (!entry.parent || entry.parent->axis != *job->axis)
            ) {
                F32 split_min = *UI_DOCK_JOB_MIN(entry.aabb) + UI_DOCK_MIN_SIZE;
                F32 split_max = *UI_DOCK_JOB_MAX(entry.aabb) - UI_DOCK_MIN_SIZE;
                F32 split     = *UI_DOCK_JOB_MIN(job->mouse_position);
                if (split >= split_min && split < split_max) {
                    #ifdef UI_ENABLE_GEOPTRINT
                    if(*job->axis == UI_DOCKING_AXIS_VERTICAL) {
                        debug_segment2(
                            1,
                            v2(split, entry.aabb.y0),
                            v2(split, entry.aabb.y1),
                            C32_GRAY
                        );
                    } else {
                        debug_segment2(
                            1,
                            v2(entry.aabb.x0, split),
                            v2(entry.aabb.x1, split),
                            C32_GRAY
                        );
                    }
                    #endif /* UI_ENABLE_GEOPTRINT */
                    if(lmb_got_pressed) {
                        /* eat input */
                        lmb_got_pressed = 0;
                        Ui_Dock *new_dock_first = ui_shared_dock_create(job->shared, entry.parent_dock);
                        Ui_Dock *new_dock_last  = ui_shared_dock_create(job->shared, entry.parent_dock);

                        new_dock_first->split =
                            normalize_to_range(
                                *UI_DOCK_JOB_MIN(entry.aabb),
                                *UI_DOCK_JOB_MAX(entry.aabb),
                                split
                            );
                        new_dock_last->split = 1;

                        new_dock_first->next      = new_dock_last;
                        new_dock_last->previous   = new_dock_first;
                        
                        entry.subdivisions->first = new_dock_first;
                        entry.subdivisions->last  = new_dock_last;
                        entry.subdivisions->axis  = *job->axis;

                        job->created_docks[0] = new_dock_first;
                        job->created_docks[1] = new_dock_last;
                        job->num_created_docks = 2;
                    }
                }
            }
        }

        
        if(!entry.subdivisions->first) {
            /* leaf dock */
            UI_ASSERT(*job->num_docks != job->num_docks_max, "Increase dock capacity.");
            size_t job_index = *job->num_docks;
            ++*job->num_docks;
            job->docks[job_index]      = entry.parent_dock;
            job->dock_aabbs[job_index] = entry.aabb;
            //if(dock->next) {
            //    *UI_DOCK_MAX(job->dock_aabbs[job_index]) -= 4;
            //}
        }

        if(entry.subdivisions->first) {
            F32 previous_split = *UI_DOCK_MIN(entry.aabb);
            B8  hovering_any_previous_dock = 0;
            for (
                Ui_Dock* dock = entry.subdivisions->first;
                dock;
                dock = dock->next
            ) {
                F32 split;
                if (dock->next) {
                    split = lerp(*UI_DOCK_MIN(entry.aabb), *UI_DOCK_MAX(entry.aabb), dock->split); /*!!!!!!!!!!!!!!!!!*/
                    if(ui_dock_action_drag_ongoing_on_target(job->action, dock)) {
                        split = *UI_DOCK_MIN(job->mouse_position);
                        V2 node_prefered_size = ui_node_prefered_size(&dock->root);
                        node_prefered_size = V2_ZERO;
                        F32 min_size = fmaxf(UI_DOCK_MIN_SIZE, *UI_DOCK_MIN(node_prefered_size) + 1);
                        if (dock->previous) {
                            F32 split_previous = lerp(*UI_DOCK_MIN(entry.aabb), *UI_DOCK_MAX(entry.aabb), dock->previous->split);
                            split = fmaxf(split_previous + min_size, split);
                        } else {
                            split = fmaxf(*UI_DOCK_MIN(entry.aabb) + min_size, split);
                        }
                        if (dock->next) {
                            V2 next_node_prefered_size = ui_node_prefered_size(&dock->next->root);
                            node_prefered_size = V2_ZERO;
                            F32 min_size_next = fmaxf(UI_DOCK_MIN_SIZE, *UI_DOCK_MIN(next_node_prefered_size) + 1);
                            F32 split_next = lerp(*UI_DOCK_MIN(entry.aabb), *UI_DOCK_MAX(entry.aabb), dock->next->split);
                            split = fminf(split_next - min_size_next, split);
                        } else {
                            split = fminf(*UI_DOCK_MAX(entry.aabb) - UI_DOCK_MIN_SIZE, split);
                        }
                        if (job->lmb_got_released) {
                            dock->split = normalize_to_range(*UI_DOCK_MIN(entry.aabb), *UI_DOCK_MAX(entry.aabb), split);
                            ui_dock_action_end(job->action);
                        }
                    }
                    Bool hovering = fabsf(split - *UI_DOCK_MIN(job->mouse_position)) <= UI_DOCK_SPLIT_HITBOX_THICKNESS;
                    B8 is_deleting = ui_dock_action_delete_ongoing_on_target(job->action, dock);
                    C32 color = 
                        is_deleting ?
                            (hovering ? C32_RED : C32_PALETTE[6] ) :
                            C32_PALETTE[7];
                    if (is_deleting) {
                        #ifdef UI_ENABLE_GEOPTRINT
                        if (entry.subdivisions->axis == UI_DOCKING_AXIS_VERTICAL) {
                            debug_segment2(
                                1,
                                v2(split, entry.aabb.y0),
                                v2(split, entry.aabb.y1),
                                color
                            );
                        } else {
                            debug_segment2(
                                1,
                                v2(entry.aabb.x0, split),
                                v2(entry.aabb.x1, split),
                                color
                            );
                        }
                        #endif /* UI_ENABLE_GEOPTRINT */
                    }
                    if(
                        hovering &&
                        entry.is_hovering_parent &&
                        !entry.is_hovering_split
                    ) {
                        if (job->lmb_got_pressed) {
                            ui_dock_action_drag_begin(job->action, dock);
                        } else if (job->rmb_got_pressed) {
                            ui_dock_action_delete_begin(job->action, dock);
                        } else if (
                            (job->action->kind == UI_DOCK_ACTION_KIND_DELETING) &&
                            job->rmb_got_released
                        ) {
                            ui_dock_action_end(job->action);
                        }

                        if(!job->lmb_is_pressed || ui_dock_action_drag_ongoing_on_target(job->action, dock)) {
                            job->is_occluding_cursor = 1;
                            *job->cursor = cursor_for_axis[entry.subdivisions->axis];
                        }
                    }
                    if(fabsf(split - *UI_DOCK_MIN(job->mouse_position)) <= UI_DOCK_MIN_SIZE) {
                        //hovering_any_split = 1;
                        entry.is_hovering_split = 1;
                    }
                } else {
                    split = *UI_DOCK_MAX(entry.aabb);
                }

                Bool hovering_dock = entry.is_hovering_parent && !hovering_any_previous_dock && *UI_DOCK_MIN(job->mouse_position) < split;
                B8 propagate_hovering = hovering_dock;// && !hovering_any_split;

                //entry.is_hovered &= !hovering_split;

                Ui_Aabb2 aabb = entry.aabb;
                *UI_DOCK_MIN(aabb) = previous_split;
                *UI_DOCK_MAX(aabb) = split;

                
                if(dock->next) {
                    *UI_DOCK_MAX(aabb) -= 1;
                }

                queue[num_entry + 0].subdivisions       = &dock->subdivisions;
                queue[num_entry + 0].aabb               = aabb;
                queue[num_entry + 0].parent             = entry.subdivisions;
                queue[num_entry + 0].parent_dock        = dock;
                queue[num_entry + 0].is_hovering_parent = hovering_dock;
                queue[num_entry + 0].is_hovering_split  = entry.is_hovering_split;
                num_entry += 1;
                
                hovering_any_previous_dock |= hovering_dock;// || hovering_any_split;
                previous_split = split;

                // if(hovering_dock && !dock->subdivisions.first && !dock0) {
                //     dock0 = 1;
                //     *job->dock_0_aabb = aabb;
                // }

                // if(!dock->subdivisions.first) {
                //     /* leaf dock */
                //     UI_ASSERT(*job->num_docks != job->num_docks_max, "Increase dock capacity.");
                //     size_t job_index = *job->num_docks;
                //     ++*job->num_docks;
                //     job->docks[job_index]      = dock;
                //     job->dock_aabbs[job_index] = aabb;
                //     //if(dock->next) {
                //     //    *UI_DOCK_MAX(job->dock_aabbs[job_index]) -= 4;
                //     //}
                // }
            }
        }
    }

    if(
        job->action->kind == UI_DOCK_ACTION_KIND_DELETING &&
        job->rmb_got_released
    ) {
        ui_dock_action_end(job->action);
    }

    #undef UI_DOCK_MIN
    #undef UI_DOCK_MAX
}

#pragma endregion /* Dock */



/* Style */
void ui_last_ignore_layout_wobble_in(Ui *ui, F32 time) {
    F32 fx = ease_in_overshoot_smooth(1 - fminf(1.0f, time * 4.0f), -2.0f);
    ui->last->ignore_layout = fx;
}
void ui_last_outset_wobble_in(Ui *ui, F32 time) {
    F32 fx = ease_in_overshoot_smooth(1 - fminf(1.0f, time * 4.0f), -2.0f);
    fx = fx * 4.0f;
    ui->last->outset[0] = fx;
    ui->last->outset[1] = fx;
    ui->last->outset[2] = fx;
    ui->last->outset[3] = fx;
}
// void ui_last_color_alpha_wobble_in(Ui *ui, F32 time) {
//     F32 fx = ease_in_overshoot_smooth(1 - fminf(1.0f, time * 4.0f), -2.0f);
//     fx = fx * 4.0f;
//     ui->last->outset[0] = fx;
//     ui->last->outset[1] = fx;
//     ui->last->outset[2] = fx;
//     ui->last->outset[3] = fx;
// }