/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* Label */

void ui_label(Ui *ui, S8 label) {
    //Ui_Node *node = ui_node_from_string(label);
    Ui_Node *node = ui_build_node_transient(ui, ui->head, 0);
    node->name = label;
    //ui_node_clear_per_build_data(node);
    //ui_node_insert_child(ui->head, node);
    node->skip_draw_background = 1;
    ui->last = node;
}

/* Button */
Ui_Event ui_button_common(Ui *ui, S8 label, Ui_Node *node, B8 lmb_disabled, B8 rmb_enabled) {
    if(
        /* only set a cursor if user hasn't already set one */
        ui->next_hover_cursor == UI_CURSOR_INVALID &&
        /* don't set any cursor if we have any drag ongoing */
        !ui_shared_drag_is_ongoing_any(ui->shared)
    )
        ui_next_hover_cursor(ui, UI_CURSOR_POINT);
    Ui_Event event = ui_event_from_node(ui, node, lmb_disabled, rmb_enabled);
    ui_node_clear_per_build_data(node);
    ui_node_insert_child(ui->head, node);
    node->name = label;
    
    node->skip_draw_background = 0;
    if(event.is_pressed || event.is_pressed_rmb) {
        node->corner_colors[0] = c32_darken(node->corner_colors[0], 0.125f);
        node->corner_colors[1] = c32_darken(node->corner_colors[1], 0.125f);
        node->corner_colors[2] = c32_darken(node->corner_colors[2], 0.125f);
        node->corner_colors[3] = c32_darken(node->corner_colors[3], 0.125f);
        node->text_color = c32_darken(node->text_color, 0.125f);
    } else if (event.is_hovered && !ui->shared->lmb_is_pressed) {
        //node->border_width = 1;
        if(ui->next_hover_color_exists) {
            node->corner_colors[0] = ui->next_hover_color;
            node->corner_colors[1] = ui->next_hover_color;
            node->corner_colors[2] = ui->next_hover_color;
            node->corner_colors[3] = ui->next_hover_color;
        } else {
            node->corner_colors[0] = c32_brighten(node->corner_colors[0], 0.125f);
            node->corner_colors[1] = c32_brighten(node->corner_colors[1], 0.125f);
            node->corner_colors[2] = c32_brighten(node->corner_colors[2], 0.125f);
            node->corner_colors[3] = c32_brighten(node->corner_colors[3], 0.125f);
        }
    } else {
        //node->skip_draw_background = 1;
    }
    ui->last = node;
    ui->next_hover_color_exists = 0;
    return event;
}

Ui_Event ui_button(Ui *ui, S8 label, B8 lmb_disabled, B8 rmb_enabled) {
    Ui_Node *node = ui_node_from_string(ui, label);
    Ui_Event event = ui_button_common(ui, label, node, lmb_disabled, rmb_enabled);
    return event;
}

Ui_Event ui_button_custom_hash(Ui *ui, S8 label, B8 lmb_disabled, B8 rmb_enabled, Ui_Hash hash) {
    Ui_Hash  in_hash = hash;
    Ui_Hash out_hash = ui_hash(ui->hash, s8(sizeof(Ui_Hash), &in_hash));
    Ui_Node *node = ui_node(ui, out_hash);
    Ui_Event event = ui_button_common(ui, label, node, lmb_disabled, rmb_enabled);
    return event;
}

// Ui_Event ui_button(S8 label) {
//     ui_next_hover_cursor(UI_CURSOR_POINT);
//     return ui_button_nocursor(label);
// }

/* is this rly usefull? */
Bool ui_toogle(Ui *ui, S8 on, S8 off, B8 lmb_disabled, B8 rmb_enabled, B8 *value) {
    if(ui_button(ui, *value ? on : off, lmb_disabled, rmb_enabled).clicked) {
        *value = !*value;
        return 1;
    } else {
        return 0;
    }
}

/* Context Menu */
void ui_shared_context_menu_open(
    Ui_Shared *shared,
    Ui_Hash    id,
    void      *userdata
) {
    shared->context_menu_anchor     = shared->mouse_position;
    shared->active_context_menu     = id;
    shared->context_menu_flip.x     = 0;
    shared->context_menu_flip.y     = 0;
    shared->context_menu_userdata   = userdata;
    shared->context_menu_color      = UI_STYLE_DEFAULT.color;
    shared->context_menu_text_color = UI_STYLE_DEFAULT.text_color;
    ++shared->context_menu_generation;
}

void ui_context_menu_open(
    Ui        *ui,
    Ui_Hash    id,
    void      *userdata
) {
    ui_shared_context_menu_open(
        ui->shared,
        id,
        userdata
    );
    ui->shared->context_menu_color      = ui->style.color;
    ui->shared->context_menu_text_color = ui->style.text_color;
}

void ui_shared_context_menu_close(Ui_Shared *shared) {
    shared->active_context_menu = 0;
    shared->context_menu_root   = 0;
}

Ui_Hash ui_shared_context_menu_create(Ui_Shared *shared) {
    return INTERLOCKED_EXCHANGE_ADD(&shared->num_context_menus, 1);
}

Bool ui_context_menu_begin(Ui *ui, Ui_Hash id) {
    if(ui->shared->active_context_menu != id)
        return 1;

    ui->style.color      = ui->shared->context_menu_color;
    ui->style.text_color = ui->shared->context_menu_text_color;

    Ui_Node *node = ui_node(ui, id);
    Bool is_hovered = ui_aabb_is_point_inside(node->aabb, ui->shared->mouse_position);
    if(!is_hovered && ui->shared->lmb_got_pressed) {
        ui_shared_context_menu_close(ui->shared);
        return 1;
    }
    ui_node_clear_per_build_data(node);
    //ui_node_insert_child(ui->context_menu, node);
    //node->name = name;

    node->border_widths[0] = -1;
    node->border_widths[1] = -1;
    node->border_widths[2] = -1;
    node->border_widths[3] = -1;
    node->kind = UI_NODE_KIND_CONTEXT_MENU;

    /* detach head*/
    //node->previous = ui->shared->context_menu_opener; /* push head */
    //ui->shared->context_menu_opener = ui->head; /* push head */
    ui->shared->context_menu_root = node;
    UI_MEMBER_PUSH(ui, root);
    ui->root = node;
    UI_MEMBER_PUSH(ui, head);
    UI_MEMBER_PUSH(ui, hash);
    ui_set_head(ui, node);
    ui->last = node;
    return 0;
}

void ui_context_menu_end(Ui *ui) {
    //ui->head = ui->shared->context_menu_opener; /* pop head */
    UI_MEMBER_POP(ui, hash);
    UI_MEMBER_POP(ui, head);
    UI_MEMBER_POP(ui, root);
}

#define UI_CONTEXT_MENU(ui, id) UI_DEFER_LOOP_TEST(ui_context_menu_begin((ui), (id)), ui_context_menu_end((ui)))

/* Scroll List */

typedef struct Ui_Scroll_List {
    size_t first;
    size_t last;
} Ui_Scroll_List;
#define UI_SCROLL_LIST_SEARCHABLE(id, ...) 

typedef struct Ui_Vertex_Animation {
    V2  position;
    C32 color;
} Ui_Vertex_Animation;

typedef struct Ui_Vertex_Character {
    U8 character; /* -> uvs, texture_slot */
} Ui_Vertex_Character;

typedef struct Ui_Ascii_Line {
    S8 str; /* the size is the width */
} Ui_Ascii_Line;

typedef struct Ui_Dst {
    int i;
} Ui_Dst;

#define UI_MONOSPACE_FONT_GLYPH_SIZE_X 0.25f
#define UI_MONOSPACE_FONT_GLYPH_SIZE_Y 0.25f

// void ui_ascii_line_l2r(V2 pivot, F32 alignement, S8 line, C32 color) {
//     pivot.x += line.length * (alignement * 0.5f - 0.5f) * UI_MONOSPACE_FONT_GLYPH_SIZE_X;
//     Ui_Vertex_Animation *anims;
//     Ui_Vertex_Character *chars;
//     ui_reserve();
//     for(size_t i = 0; i != line.length; ++i) {
//         anims[i].position = pivot;
//         anims[i].color    = color;
//         pivot.x += UI_MONOSPACE_FONT_GLYPH_SIZE_X;
//     }
//     memcpy(chars, line.data, line.length);
// }

// void ui_ascii_lines_t2b_l2r(V2 pivot, V2 alignement, size_t num_lines, S8 *lines, C32 color) {
//     V2 line_pivot;
//     line_pivot.y = pivot.y + num_lines * (alignement.y * 0.5f + 0.5f) * UI_MONOSPACE_FONT_GLYPH_SIZE_Y;
//     for(size_t i = 0; i != num_lines; ++i) {
//         ui_ascii_line_l2r(line_pivot, alignement.x, lines[i], color);
//     }
// }

// void ui_render_ascii_monospace_unisize() {

// }

/* Bar */
#define UI_BAR
void ui_bar_begin(Ui *ui, S8 name) {
    Ui_Node *node = ui_node_from_string(ui, name);
    Bool is_hovered = ui_aabb_is_point_inside(node->aabb, ui->shared->mouse_position);
    ui_node_clear_per_build_data(node);
    ui_node_insert_child(ui->head, node);
    //node->name = name;

    node->border_widths[0] = -1;
    node->border_widths[1] = -1;
    node->border_widths[2] = -1;
    node->border_widths[3] = -1;
    node->kind = UI_NODE_KIND_BAR;
    ui_set_head(ui, node);

    if(node->is_node_first_frame) {
        node->as.bar.anchor = 1;
    }

    //(*((V2*)ui_style_push(&(((Ui*)(0))->style.layout_direction), sizeof(V2))) = (layout_direction));
    Ui_Event e;
    UI_TEXT_PADDING(ui, 2)
    UI_WIDTH(ui, 8){
        ui_next_hover_cursor(ui, UI_CURSOR_GRAB);
        e = ui_button(ui, S8_LIT("\x7F"), 0, 0);
    }
    if(e.drag && ui->shared->drag.active != node->hash) {
        ui_shared_drag_begin(ui->shared, node->hash);
        ui->shared->drag.anchor = node->as.bar.anchor;
    }
    if(ui->shared->drag.active == node->hash) {
        V2 uv = v2_normalize_to_range(
            v2(node->parent->aabb.x0, node->parent->aabb.y0),
            v2(node->parent->aabb.x1, node->parent->aabb.y1),
            ui->shared->mouse_position
        );
        uv = v2_sub_f32(v2_mul_f32(uv, 2.f), 1.f);
        F32 a = 0;
        F32 b = 0;
        /* hysteresis */
        switch(ui->shared->drag.anchor) {
            case 0: { /* right */
                b = -0.0625f;
            } break;
            case 1: { /* up */
                a = -0.0625f;
            } break;
            case 2: { /* down */
                a = +0.0625f;
            } break;
            case 3: { /* left */
                b = +0.0625f;
            } break;
        }
        Bool anchor_a = uv.x - uv.y + a - b < 0;
        Bool anchor_b = uv.x + uv.y - a - b < 0;
        U8 anchor = ((anchor_b & 1) << 1) | (anchor_a & 1);
        //node->as.bar.anchor = anchor;
        ui->shared->drag.anchor = anchor;
        ui->shared->cursor = UI_CURSOR_GRABBING;
    }
    if(e.drop && ui->shared->drag.active == node->hash) {
        ui_shared_drag_end(ui->shared);
        node->as.bar.anchor = ui->shared->drag.anchor;
        ui->shared->cursor = UI_CURSOR_DEFAULT;
    }

    V2 layout_direction = V2_ZERO;
    switch(node->as.bar.anchor) {
        case 0: { /* right */
            layout_direction.y = -1;
        } break;
        case 1: { /* up */
            layout_direction.x = 1;
        } break;
        case 2: { /* down */
            layout_direction.x = 1;
        } break;
        case 3: { /* left */
            layout_direction.y = -1;
        } break;
    }
    node->layout_direction = layout_direction;
}

void ui_bar_end(Ui *ui) {
    //ui_style_pop(&((Ui*)(0))->style.layout_direction, sizeof(V2));
    ui_set_head(ui, ui->head->parent);
}

#define UI_BAR(ui, name) UI_DEFER_LOOP(ui_bar_begin((ui), S8((name))), ui_bar_end((ui)))

/* Dropdown */
size_t ui_dropdown_menu_begin(Ui *ui, S8 label) {
    Ui_Hash button_hash = ui_hash(ui->head->hash, label);

    Ui_Event event = ui_button(ui, label, 0, 0);

    Bool active_dropdown_menu_exists = ui->active_dropdown_menu;
    Bool is_active                   = ui->active_dropdown_menu == button_hash;

    if(event.clicked || (active_dropdown_menu_exists && !is_active && event.is_hovered)) {
        // ui_open_dropdown_menu();
        ui->active_dropdown_menu = button_hash;
    }

    /* if(!ui_dropdown_menu_is_open(dropdown_menu)) return 1; */
    if(ui->active_dropdown_menu != button_hash)
        return 1;
    
    Ui_Hash container_hash = ui_hash(button_hash, S8_LIT("[]"));
    Ui_Node *node = ui_node(ui, container_hash);
        
    Bool is_hovered = ui_aabb_is_point_inside(node->aabb, ui->shared->mouse_position);
    if(!is_hovered && ui->shared->lmb_got_pressed) {
        if(ui->shared->active_node == button_hash) ui->shared->active_node = 0;
        ui->active_dropdown_menu = 0;
        ui->dropdown_menu = 0;
        return 1;
    }

    ui->dropdown_menu = node;

    ui_node_clear_per_build_data(node);

    node->border_widths[0] = -1;
    node->border_widths[1] = -1;
    node->border_widths[2] = -1;
    node->border_widths[3] = -1;
    node->kind = UI_NODE_KIND_DROPDOWN_MENU;
    
    /* detatch ourselves from the main tree */
    node->previous = ui->last;
    ui_set_head(ui, node);
    return 0;
}

void ui_dropdown_menu_end(Ui *ui) {
    if(!ui->head->first)
        ui->head->skip_draw_background = 1;
    /* reatatch ourselves to the main tree */
    ui_set_head(ui, ui->head->previous->parent);
}

#define UI_DROPDOWN_MENU(name) UI_DEFER_LOOP_TEST(ui_dropdown_menu_begin(S8((name))), ui_dropdown_menu_end())

#define UI_SPLIT

#define UI_BLOCK

#define UI_PIVOT(...)
#define UI_ANCHOR(...)


/* Panel */

void ui_panel_begin() {

}

void ui_panel_end() {

}

/* Box */

void ui_box_begin(Ui *ui) {
    Ui_Node *node = ui_build_node_transient(ui, ui->head, ui->hash);
    node->skip_draw_background = 1;
    ui_set_head(ui, node);
    ui->last = node;
}

void ui_box_end(Ui *ui) {
    ui_set_head(ui, ui->head->parent);
}
#define UI_BOX(ui) UI_DEFER_LOOP(ui_box_begin(ui), ui_box_end(ui))

void ui_box_named_begin(Ui *ui, S8 name) {
    Ui_Hash hash = ui_hash(ui->hash, name);
    Ui_Node *node = ui_build_node_transient(ui, ui->head, hash);
    node->skip_draw_background = 0;
    //node->skip_draw_background = 1;
    ui_set_head(ui, node);
    ui->last = node;
}
#define UI_BOX_NAMED(ui, name_s8) UI_DEFER_LOOP(ui_box_named_begin((ui), name_s8), ui_box_end((ui)))


#define UI_FLEX(flex_f32)
//define UI_BOX_NAMED(name_s8)

/* Pannel / Tab / Area */
#define UI_DOCK(...)