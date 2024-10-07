/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

void in_backend_swap_buffer_job(In **in) {
    while(INTERLOCKED_EXCHANGE(&win32->in_lock, 1) != 0); {
        /* Swap Pointers */
        In *in_new = win32->in_current;
        In *in_old = *in;
        /* Button Update */
        for(
            size_t button_index = 0;
            button_index < IN_NUM_COUCH_PLAYERS_MAX * BUTTON_COUNT;
            ++button_index
        ) {
            In_Button_Event *button = &in_new->couch_player_buttons[button_index];
            button->got_pressed  = !button->was_pressed &&  button->is_pressed;
            button->got_released =  button->was_pressed && !button->is_pressed;
        }
        /* Mouse Update */
        POINT p;
        GetCursorPos(&p);
        in_new->mouse.position_last_frame = in_old->mouse.position;
        in_new->mouse.position = v2(p.x, p.y);
        in_new->mouse.delta    = v2_sub(in_new->mouse.position, in_new->mouse.position_last_frame);
        
        /* Recycle */
        in_old->mouse.wheel = V2_ZERO;
        for(
            size_t button_index = 0;
            button_index < IN_NUM_COUCH_PLAYERS_MAX * BUTTON_COUNT;
            ++button_index
        ) {
            In_Button_Event *button_new = &in_new->couch_player_buttons[button_index];
            In_Button_Event *button_old = &in_old->couch_player_buttons[button_index];
            *button_old = *button_new;
            button_old->was_pressed = button_old->is_pressed;
        }
        
        *in               = in_new;
        win32->in_current = in_old;
    } INTERLOCKED_EXCHANGE(&win32->in_lock, 0);
}

void in_win32_process_event(In **in_ptr, UINT uMsg, WPARAM wParam) {
    size_t couch_player_index = IN_DEFAULT_COUCH_PLAYER_INDEX;
    switch(uMsg) {
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        {
            UINT down =
                uMsg == WM_LBUTTONDOWN ||
                uMsg == WM_RBUTTONDOWN ||
                uMsg == WM_MBUTTONDOWN ||
                uMsg == WM_SYSKEYDOWN  ||
                uMsg == WM_KEYDOWN;
                 if (uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDOWN) wParam = VK_LBUTTON;
            else if (uMsg == WM_RBUTTONUP || uMsg == WM_RBUTTONDOWN) wParam = VK_RBUTTON;
            else if (uMsg == WM_MBUTTONUP || uMsg == WM_MBUTTONDOWN) wParam = VK_MBUTTON;
            while(INTERLOCKED_EXCHANGE(&win32->in_lock, 1) != 0); {
                In *in = (In *)INTERLOCKED_COMPARE_EXCHANGE(in_ptr, 0, 0);
                In_Button_Event *button = &in->couch_player_buttons[
                    couch_player_index * BUTTON_COUNT + wParam
                ];
                if (button->was_pressed) {
                    if (!down) button->is_pressed = 0;
                } else {
                    if ( down) button->is_pressed = 1;
                }
            } INTERLOCKED_EXCHANGE(&win32->in_lock, 0);
        } break;
        case WM_MOUSEWHEEL: {
            while(INTERLOCKED_EXCHANGE(&win32->in_lock, 1) != 0); {
                In *in = (In *)INTERLOCKED_COMPARE_EXCHANGE(in_ptr, 0, 0);
                in->mouse.wheel.y += (F32)((signed short)HIWORD(wParam) / WHEEL_DELTA);
            } INTERLOCKED_EXCHANGE(&win32->in_lock, 0);
        } break;
    }
}