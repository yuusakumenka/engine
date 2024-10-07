/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* Handles are widgets to manipulate data in 3d space within the scene */

typedef struct Widget_Tanslate {
    int dummy;
} Widget_Tanslate;

/* we need some way of projecting the mouse
 * to one of the xyz axis
 * kinda like the nearest point on screen
 * because the relationship is screen space
 * we probably need to project the axis into screen space
 * project mouse onto that screen space axis
 * and convert the point on the screen axis
 * to a point on the world axis
 * 
 * how do we select a world axis?
 * thid depends on what arrow, plane or center the user picks
 * if it's 2d, then either plane is obvious or plane and center is same thing
 * let's just support arrows for the time beeing... 
 * altho I do want to be able to pick plane...
 * plane is more partcial, we'll do snap arrows later
 * Blender 3d widget is pretty cool, let's use that as a ref
 * So we take the point, project it on screen, display a circle,
 * if user drag circle, we move along camera facing plane.
 * That's raycasting work...
 * The axis version like finding the closest point between a ray and a line 
 * (or two lines)
 * So I need a matrix that transform from screen coordinate
 * to world coordinate, and lets me generate a ray.
 * That's the inverse projection * view matrix.
 * what do we do?
 * camera_get_ray?
 * ray_from_m4_v2?
 * Minimu dependencies means we favour m4 over camera.
 * Cause a matrix is much more fundamental than a camera.
 * And we can make it work with just matrix.
 * Camera api is the cherry.
 * What is a ray from a matrix?
 * Ray is origin, normalized_direction, magnitude...
 * We just want line plane intersection, the output is a point.
 * let's forget the intermediary
 * we have a v2 cursor postion in screen space
 * a m4 from screen to space (just feed (x, y, 0, 1))
 * we want a v3 on some arbitrary plane
 * plane is (origin, normal)
 * let's cheat and just use normal and origin always be (0,0,0)
 * how do we get (normal, m4, v2) -> v3
 * what algoritm?
 * parametric line plugged into implicit plane ?
 * implicit plane : dot(n, p) = 0
 * parametric line : origin + ray_dir * t
 * that's anoying I wanted to avoid having a ray...
 * is there any way to ditch either of the two...
 * no... origin tells us where we end up on the plane
 * dir tells us if we hit the plane or not, and how
 * we need both but it still feels like too much
 * there is infinite ways to represent the same line
 * by having x, y belong to line, and some magnitude direction...
 * we don't care about the magnitude
 * but the only other way would be to use a quaternion and that's just
 * one more float
 * 
 * dot(n, o + d * t) = 0
 * nx * (ox + dx * t) + ...
 * nx dx t + nx ox + nx dx +...
 * t(dot(n, d)) + ...
 * t = dot(-o, n) / dot(n,d) 
 */

typedef struct Plane {
    V3 origin;
    V3 normal;
} Plane;
typedef struct Line {
    V3 origin;
    V3 normal;
} Line;

typedef struct Widget_Tanslate_V2_State {
    B8 beeing_dragged;
    V2 inital_screen_delta;
} Widget_Tanslate_V2_State;
typedef struct Widget_Tanslate_V2 {
    Widget_Tanslate_V2_State *state;
    Draw_Context *draw;
    F32 radius;
    V2 mouse_position;
    B8 got_pressed;
    B8 got_released;
    M3x2 world_to_screen;
    M3x2 screen_to_world;
    M3x2 screen_to_viewport;
} Widget_Tanslate_V2;

B8 widget_tanslate_v2(Widget_Tanslate_V2 *data, V2 *v2) {

    V2 v2_screen = m3x2_multiply_point(data->world_to_screen, *v2);

    B8 hovering;
    {
        hovering = 
            v2_distance_squared(v2_screen, data->mouse_position) <= 
            data->radius * data->radius;
    }

    /* レンディングはワールド座標にする */
    M3x2 old_matrix = draw_matrix(data->draw, data->screen_to_viewport);
    
    /*
     * debug_circ_to(ctx, )*/

    draw_circ(
        data->draw,
        v2_screen.x,
        v2_screen.y,
        data->radius,
        32,
        0,
        hovering ? 0xFF0000FF : 0xFFFFFFFF
    );

    if(hovering) {
        if(data->got_pressed) {
            data->state->beeing_dragged = TRUE;
            data->state->inital_screen_delta =
                v2_a_to_b(
                    data->mouse_position,
                    v2_screen
                );
        }
    }

    if(data->state->beeing_dragged) {
        V2 v2_world = 
            m3x2_multiply_point(
                data->screen_to_world,
                v2_add(
                    data->mouse_position,
                    data->state->inital_screen_delta
                )
            );
        *v2 = v2_world;
    }

    if(data->got_released) {
        data->state->beeing_dragged = FALSE;
    }

    // if(hovering) {
    //     Int axis;
    //     {
            
    //     }

    //     /* highlight axis */
    //     {

    //     }
    // }

    /* 行列を戻す */
    draw_matrix(data->draw, old_matrix);
}