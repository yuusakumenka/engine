/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* Brainstorm folows. Strong raincoat recommended. */

//#define IN_DISABLE_DEFAULT_SURFACE
//#define IN_DISABLE_DEFAULT_COUCH_PLAYER

/* API

input_
    input_is_pressed
    input_was_pressed
    input_touch_
    input_pen_

    input_get_mouse_position
    input_get_mouse_scroll_y

mouse_
    mouse_get_position
    mouse_get_position_x
    mouse_get_position_y // input_mouse_position

    mouse_get_position_delta
    
    mouse_get_scroll
    mouse_get_scroll_x // input_mouse_scroll_y
    mouse_get_scroll_y // input_scroll_y

    mouse_get_wheel_delta_y();
    mouse_

*/


// B8 button_is_pressed  (Button button);
// B8 button_got_pressed (Button button);
// B8 button_is_released (Button button);
// B8 button_got_released(Button button);


// V2 mouse_position();
// V2 mouse_delta();
// V2 mouse_wheel();



// V2 get_mouse_position(Inputs);
// V2 get_mouse_position_in_surface(Input_State, Os_Surface *surface);

// void mouse_set_position(F32 x, F32 y);

// /* Keyboard */
// typedef U8 Keyboard_Layout;
// typedef enum Keyboard_Layout_ {
//     KEYBOARD_LAYOUT_QWERTY,
//     KEYBOARD_LAYOUT_AZERTY
// } Keyboard_Layout_;

// Keyboard_Layout keyboard_get_layout();


// Button_State inputs_button(Inputs* inputs, Button button);

// typedef enum Analog {

// } Analog;
// float analog(Analog analog);

// typedef struct Keyboard_State {
//     char is_azerty      : 1;
//     char layout_changed : 1;
// } Keyboard_State;

// Keyboard_State keyboard();

/* inputs_button*/

/* if (in_button('A').is_pressed) { } */

// //typedef ptrdiff_t 
// void input_test() {
//     if(button(keyboard_is_azerty() ? 'Z' : 'W').is_pressed) {

//     }

//     typedef struct Bindings {
//         Keybind jump;
//         Button forward;
//     } Bindings;
//     Bindings bindings_qwerty;

//     Bindings *bindings = keyboard_is_azerty() ? &bindings_qwerty : &bindings_qwerty;

//     if(button(bindings->forward).is_pressed) {

//     }
//     if(keybind(&bindings->jump).is_pressed) {

//     }

//     //keyboard().is_azerty

//     //if(keyboard().layout_changed)
// }

/* > button('W').is_pressed
*  Is equivant to:
*  > engine->buttons['W'].is_pressed */

/* Advanced inputs, configurable bindings: 
 * We can replace
 * > button(BUTTON_SPACE).got_pressed
 * by
 * > button(bindings->jump).got_pressed
 * Where bindings is a global pointer you overwrite
 * at the begening of each frame. 
 * Of course it doesnt have to be a global.
 * And if you are in a module:
 * > button(platformer_module->bindings->jump).got_pressed
 * What about AZERTY ?
 * > Bindings *bindings;
 * > ...
 * > bindings = keyboard_is_azerty() ? azerty_bindings : qwerty_bindings; */

/* Many terrible ideas above. Adventurer be warned. */

#ifndef IN_NUM_COUCH_PLAYERS_MAX
#define IN_NUM_COUCH_PLAYERS_MAX 4
#endif

#ifndef IN_DEFAULT_COUCH_PLAYER_INDEX
#define IN_DEFAULT_COUCH_PLAYER_INDEX 0
#endif

typedef struct In_Backend In_Backend;

typedef char In_Couch_Player_Index;
typedef char In_Surface_Index;

typedef unsigned char Button;
typedef enum Button_ { /* @DEPRECATED, use IN_... instead. */
    // anything
    BUTTON_ANY      = 0x00,
    BUTTON_WILDCARD = BUTTON_ANY,

    // mouse
    BUTTON_MOUSE_LEFT          = 0x01,
    BUTTON_LMB                 = BUTTON_MOUSE_LEFT,
    BUTTON_LEFT_MOUSE_BUTTON   = BUTTON_MOUSE_LEFT,
    BUTTON_MOUSE_RIGHT         = 0x02,
    BUTTON_RMB                 = BUTTON_MOUSE_RIGHT,
    BUTTON_RIGHT_MOUSE_BUTTON  = BUTTON_MOUSE_LEFT,
    BUTTON_MOUSE_MIDDLE        = 0x04,
    BUTTON_MMB                 = BUTTON_MOUSE_MIDDLE,
    BUTTON_MIDDLE_MOUSE_BUTTON = BUTTON_MOUSE_MIDDLE,
    BUTTON_MOUSE4              = 0x05,
    BUTTON_MOUSE5              = 0x06,
    BUTTON_MOUSE_WHEEL_UP      = 0x06,
    BUTTON_MOUSE_WHEEL_DOWN    = 0x06,

    // keyboard
    BUTTON_BACKSPACE = 0x08,
    BUTTON_TAB       = 0x09,
    BUTTON_CLEAR     = 0x0C,
    BUTTON_ENTER     = 0x0D,
    BUTTON_RETURN    = BUTTON_ENTER,

    // alter              
    BUTTON_SHIFT   = 0x10,
    BUTTON_CONTROL = 0x11,
    BUTTON_CTRL    = BUTTON_CONTROL,
    BUTTON_ALT     = 0x12,
    BUTTON_MENU    = BUTTON_ALT,
    BUTTON_ESC     = 0x1B,
    BUTTON_ESCAPE  = BUTTON_ESC,

    // directions (WASD/ZQSD + Directional Keys)
    BUTTON_RIGHT,
    BUTTON_LEFT,
    BUTTON_UP,
    BUTTON_DOWN,

    // keyboard
    BUTTON_SPACE     = ' ',
    BUTTON_SPACEBAR  = BUTTON_SPACE,
    BUTTON_SPACE_BAR = BUTTON_SPACE,

    BUTTON_DELETE = 0x2E,
    BUTTON_DEL    = BUTTON_DELETE,

    // controller buttons            
    BUTTON_RIGHT_THUMB_DOWN   = 0x80,
    BUTTON_MICROSOFT_A        = BUTTON_RIGHT_THUMB_DOWN,
    BUTTON_SONY_CROSS         = BUTTON_RIGHT_THUMB_DOWN,
    BUTTON_NINTENDO_B         = BUTTON_RIGHT_THUMB_DOWN,
    BUTTON_RIGHT_THUMB_RIGHT,    
    BUTTON_MICROSOFT_B        = BUTTON_RIGHT_THUMB_RIGHT,
    BUTTON_SONY_CIRCLE        = BUTTON_RIGHT_THUMB_RIGHT,
    BUTTON_NINTENDO_A         = BUTTON_RIGHT_THUMB_RIGHT,
    BUTTON_RIGHT_THUMB_LEFT,
    BUTTON_MICROSOFT_X        = BUTTON_RIGHT_THUMB_LEFT,
    BUTTON_SONY_SQUARE        = BUTTON_RIGHT_THUMB_LEFT,
    BUTTON_NINTENDO_Y         = BUTTON_RIGHT_THUMB_LEFT,
    BUTTON_RIGHT_THUMB_UP,         
    BUTTON_MICROSOFT_Y        = BUTTON_RIGHT_THUMB_UP,
    BUTTON_SONY_TRIANGLE      = BUTTON_RIGHT_THUMB_UP,
    BUTTON_NINTENDO_X         = BUTTON_RIGHT_THUMB_UP,

    // controller directional pad    
    BUTTON_LEFT_THUMB_DOWN,  
    BUTTON_DPAD_DOWN         = BUTTON_LEFT_THUMB_DOWN,
    BUTTON_MICROSOFT_DOWN    = BUTTON_LEFT_THUMB_DOWN,
    BUTTON_SONY_DOWN         = BUTTON_LEFT_THUMB_DOWN,
    BUTTON_NINTENDO_DOWN     = BUTTON_LEFT_THUMB_DOWN,
    BUTTON_LEFT_THUMB_RIGHT,   
    BUTTON_DPAD_RIGHT        = BUTTON_LEFT_THUMB_RIGHT,
    BUTTON_MICROSOFT_RIGHT   = BUTTON_LEFT_THUMB_RIGHT,
    BUTTON_SONY_RIGHT        = BUTTON_LEFT_THUMB_RIGHT,
    BUTTON_NINTENDO_RIGHT    = BUTTON_LEFT_THUMB_RIGHT,
    BUTTON_LEFT_THUMB_LEFT,    
    BUTTON_DPAD_LEFT         = BUTTON_LEFT_THUMB_LEFT,
    BUTTON_MICROSOFT_LEFT    = BUTTON_LEFT_THUMB_LEFT,
    BUTTON_SONY_LEFT         = BUTTON_LEFT_THUMB_LEFT,
    BUTTON_NINTENDO_LEFT     = BUTTON_LEFT_THUMB_LEFT,
    BUTTON_LEFT_THUMB_UP,      
    BUTTON_DPAD_UP           = BUTTON_LEFT_THUMB_UP,
    BUTTON_MICROSOFT_UP      = BUTTON_LEFT_THUMB_UP,
    BUTTON_SONY_UP           = BUTTON_LEFT_THUMB_UP,
    BUTTON_NINTENDO_UP       = BUTTON_LEFT_THUMB_UP,

    // bumpers / shoulder
    BUTTON_RIGHT_INDEX_FINGER,   
    BUTTON_MICROSOFT_RB        = BUTTON_RIGHT_INDEX_FINGER,
    BUTTON_SONY_R1             = BUTTON_RIGHT_INDEX_FINGER,
    BUTTON_NINTENDO_R          = BUTTON_RIGHT_INDEX_FINGER,
    BUTTON_LEFT_INDEX_FINGER,    
    BUTTON_MICROSOFT_LB        = BUTTON_LEFT_INDEX_FINGER,
    BUTTON_SONY_L1             = BUTTON_LEFT_INDEX_FINGER,
    BUTTON_NINTENDO_L          = BUTTON_LEFT_INDEX_FINGER,

    // triggers             
    BUTTON_RIGHT_MIDDLE_FINGER,   
    BUTTON_MICROSOFT_RT         = BUTTON_RIGHT_INDEX_FINGER,
    BUTTON_SONY_R2              = BUTTON_RIGHT_INDEX_FINGER,
    BUTTON_NINTENDO_ZR          = BUTTON_RIGHT_INDEX_FINGER,
    BUTTON_LEFT_MIDDLE_FINGER,    
    BUTTON_MICROSOFT_LT         = BUTTON_LEFT_MIDDLE_FINGER,
    BUTTON_SONY_L2              = BUTTON_LEFT_MIDDLE_FINGER,
    BUTTON_NINTENDO_ZL          = BUTTON_LEFT_MIDDLE_FINGER,

    // stick press
    BUTTON_RIGHT_STICK,          
    BUTTON_LEFT_STICK,           

    // start              
    BUTTON_START,                
    BUTTON_MICROSOFT_START = BUTTON_START,
    BUTTON_SONY_OPTION     = BUTTON_START,
    BUTTON_NINTENDO_PLUS   = BUTTON_START,

    // select               
    BUTTON_SELECT,                 
    BUTTON_MICROSOFT_BACK = BUTTON_SELECT,
    BUTTON_SONY_TOUCHPAD  = BUTTON_SELECT,
    BUTTON_NINTENDO_MINUS = BUTTON_SELECT,

    BUTTON_COUNT
} Button_;


typedef enum In_ {
    // anything
    IN_ANY      = 0x00,
    IN_WILDCARD = IN_ANY,

    // mouse
    IN_MOUSE_LEFT       = 0x01,
    IN_LMB              = IN_MOUSE_LEFT,
    IN_LEFT_MOUSE_IN    = IN_MOUSE_LEFT,
    IN_MOUSE_RIGHT      = 0x02,
    IN_RMB              = IN_MOUSE_RIGHT,
    IN_RIGHT_MOUSE_IN   = IN_MOUSE_LEFT,
    IN_MOUSE_MIDDLE     = 0x04,
    IN_MMB              = IN_MOUSE_MIDDLE,
    IN_MIDDLE_MOUSE_IN  = IN_MOUSE_MIDDLE,
    IN_MOUSE4           = 0x05,
    IN_MOUSE5           = 0x06,
    IN_MOUSE_WHEEL_UP   = 0x06,
    IN_MOUSE_WHEEL_DOWN = 0x06,

    // keyboard
    IN_BACKSPACE = 0x08,
    IN_TAB       = 0x09,
    IN_CLEAR     = 0x0C,
    IN_ENTER     = 0x0D,
    IN_RETURN    = IN_ENTER,

    // alter              
    IN_SHIFT   = 0x10,
    IN_CONTROL = 0x11,
    IN_CTRL    = IN_CONTROL,
    IN_ALT     = 0x12,
    IN_MENU    = IN_ALT,
    IN_ESC     = 0x1B,
    IN_ESCAPE  = IN_ESC,

    // directions (WASD/ZQSD + Directional Keys)
    IN_RIGHT,
    IN_LEFT,
    IN_UP,
    IN_DOWN,

    // keyboard
    IN_SPACE     = ' ',
    IN_SPACEBAR  = IN_SPACE,
    IN_SPACE_BAR = IN_SPACE,

    IN_DELETE = 0x2E,
    IN_DEL    = IN_DELETE,

    /* Apparently people get confused if they don't see an entry for
     * common ASCII keys so we'll have to write them here at some point. */
    IN_A = 'A', /* and so on... */

    // controller INs            
    IN_RIGHT_THUMB_DOWN   = 0x80,
    IN_MICROSOFT_A        = IN_RIGHT_THUMB_DOWN,
    IN_SONY_CROSS         = IN_RIGHT_THUMB_DOWN,
    IN_NINTENDO_B         = IN_RIGHT_THUMB_DOWN,
    IN_RIGHT_THUMB_RIGHT,    
    IN_MICROSOFT_B        = IN_RIGHT_THUMB_RIGHT,
    IN_SONY_CIRCLE        = IN_RIGHT_THUMB_RIGHT,
    IN_NINTENDO_A         = IN_RIGHT_THUMB_RIGHT,
    IN_RIGHT_THUMB_LEFT,
    IN_MICROSOFT_X        = IN_RIGHT_THUMB_LEFT,
    IN_SONY_SQUARE        = IN_RIGHT_THUMB_LEFT,
    IN_NINTENDO_Y         = IN_RIGHT_THUMB_LEFT,
    IN_RIGHT_THUMB_UP,         
    IN_MICROSOFT_Y        = IN_RIGHT_THUMB_UP,
    IN_SONY_TRIANGLE      = IN_RIGHT_THUMB_UP,
    IN_NINTENDO_X         = IN_RIGHT_THUMB_UP,

    // controller directional pad    
    IN_LEFT_THUMB_DOWN,  
    IN_DPAD_DOWN         = IN_LEFT_THUMB_DOWN,
    IN_MICROSOFT_DOWN    = IN_LEFT_THUMB_DOWN,
    IN_SONY_DOWN         = IN_LEFT_THUMB_DOWN,
    IN_NINTENDO_DOWN     = IN_LEFT_THUMB_DOWN,
    IN_LEFT_THUMB_RIGHT,   
    IN_DPAD_RIGHT        = IN_LEFT_THUMB_RIGHT,
    IN_MICROSOFT_RIGHT   = IN_LEFT_THUMB_RIGHT,
    IN_SONY_RIGHT        = IN_LEFT_THUMB_RIGHT,
    IN_NINTENDO_RIGHT    = IN_LEFT_THUMB_RIGHT,
    IN_LEFT_THUMB_LEFT,    
    IN_DPAD_LEFT         = IN_LEFT_THUMB_LEFT,
    IN_MICROSOFT_LEFT    = IN_LEFT_THUMB_LEFT,
    IN_SONY_LEFT         = IN_LEFT_THUMB_LEFT,
    IN_NINTENDO_LEFT     = IN_LEFT_THUMB_LEFT,
    IN_LEFT_THUMB_UP,      
    IN_DPAD_UP           = IN_LEFT_THUMB_UP,
    IN_MICROSOFT_UP      = IN_LEFT_THUMB_UP,
    IN_SONY_UP           = IN_LEFT_THUMB_UP,
    IN_NINTENDO_UP       = IN_LEFT_THUMB_UP,

    // bumpers / shoulder
    IN_RIGHT_INDEX_FINGER,   
    IN_MICROSOFT_RB        = IN_RIGHT_INDEX_FINGER,
    IN_SONY_R1             = IN_RIGHT_INDEX_FINGER,
    IN_NINTENDO_R          = IN_RIGHT_INDEX_FINGER,
    IN_LEFT_INDEX_FINGER,    
    IN_MICROSOFT_LB        = IN_LEFT_INDEX_FINGER,
    IN_SONY_L1             = IN_LEFT_INDEX_FINGER,
    IN_NINTENDO_L          = IN_LEFT_INDEX_FINGER,

    // triggers             
    IN_RIGHT_MIDDLE_FINGER,   
    IN_MICROSOFT_RT         = IN_RIGHT_INDEX_FINGER,
    IN_SONY_R2              = IN_RIGHT_INDEX_FINGER,
    IN_NINTENDO_ZR          = IN_RIGHT_INDEX_FINGER,
    IN_LEFT_MIDDLE_FINGER,    
    IN_MICROSOFT_LT         = IN_LEFT_MIDDLE_FINGER,
    IN_SONY_L2              = IN_LEFT_MIDDLE_FINGER,
    IN_NINTENDO_ZL          = IN_LEFT_MIDDLE_FINGER,

    // stick press
    IN_RIGHT_STICK,          
    IN_LEFT_STICK,           

    // start              
    IN_START,                
    IN_MICROSOFT_START = IN_START,
    IN_SONY_OPTION     = IN_START,
    IN_NINTENDO_PLUS   = IN_START,

    // select               
    IN_SELECT,                 
    IN_MICROSOFT_BACK = IN_SELECT,
    IN_SONY_TOUCHPAD  = IN_SELECT,
    IN_NINTENDO_MINUS = IN_SELECT,

    IN_COUNT
} In_;

typedef struct In_Button_Event {
    char is_pressed   : 1;
    char was_pressed  : 1;
    char got_pressed  : 1; /* rename to "begin"? */
    char got_released : 1; /* rename to "end" ? */
    char unused3      : 1;
    char unused2      : 1;
    char unused1      : 1;
    char unused0      : 1;
} In_Button_Event;

typedef struct In_Mouse {
    V2 position;
    V2 position_last_frame;
    V2 delta;
    V2 wheel;
} In_Mouse;

typedef struct In {
    In_Button_Event couch_player_buttons [IN_NUM_COUCH_PLAYERS_MAX * BUTTON_COUNT];
    In_Mouse        mouse;
} In;

static In *in;

/* Backend */
void in_on_dll_reload(In *in_) { /* We should probably do like UI and ban global variables. */
    in = in_;
}

void in_next(In **in) { /* what is this @DEPRECATED? */

}

void in_backend_swap_buffer_job(In **in);

/*{
    for (
        size_t button_index = 0;
        button_index < IN_NUM_COUCH_PLAYERS_MAX * BUTTON_COUNT;
        ++button_index
    ) {
        In_Button *button = &in->couch_player_buttons[button_index];
        button->was_pressed = button->is_pressed;
    }
    
}*/

In *get_in() {
    In *in_ = (In *)INTERLOCKED_COMPARE_EXCHANGE(&in, 0, 0); /* Probably no need for interlocked here. */
    return in;
}


#ifndef IN_DISABLE_DEFAULT_COUCH_PLAYER
In_Button_Event in_button(Button button) {
    In *in = get_in();
    return in->couch_player_buttons[
        IN_DEFAULT_COUCH_PLAYER_INDEX * BUTTON_COUNT + button
    ];
}
#endif /* !IN_DISABLE_DEFAULT_COUCH_PLAYER */

In_Button_Event in_button_for_player(
    Button                button,
    In_Couch_Player_Index couch_player_index
) {
    In *in = get_in();
    return in->couch_player_buttons[
        couch_player_index * BUTTON_COUNT + button
    ];
}

In_Mouse in_mouse()          { In *in = get_in(); return in->mouse; }
V2       in_mouse_position() { In *in = get_in(); return in->mouse.position; }
V2       in_mouse_delta()    { In *in = get_in(); return in->mouse.delta; }
V2       in_mouse_wheel()    { In *in = get_in(); return in->mouse.wheel; }

#ifndef IN_DISABLE_KEYBINDS

#ifdef __INTELLISENSE__
#include "engine_types.h"
//#include "engine_module_input.h"
#endif

#define KEYBIND_NUM_KEYS_MAX 4
#define KEYBIND_NUM_MODIFIER_KEY_MAX 2
#define KEYBIND_NUM_ALTERNATIVES_MAX 2
#define KEYBIND_NUM_FORMAT_BYTES_MAX 128

typedef struct Keybind_Alternative {
    Button keys         [KEYBIND_NUM_KEYS_MAX];
    Button modifier_keys[KEYBIND_NUM_MODIFIER_KEY_MAX];
    U8     num_keys;
    U8     num_modifier_keys;
} Keybind_Alternative;

typedef struct Keybind {
    Keybind_Alternative alternatives[KEYBIND_NUM_ALTERNATIVES_MAX];
    U8                  num_alternatives;
} Keybind;

Bool keybind_alternative_is_pressed(Keybind_Alternative *alternative) {
    Bool alternative_is_pressed = TRUE;
    for (
        Int idx_modifier_key = 0; 
        idx_modifier_key < alternative->num_modifier_keys; 
        ++idx_modifier_key
    ) {
        Button modifier_key = alternative->modifier_keys[idx_modifier_key];
        alternative_is_pressed &= in_button(modifier_key).is_pressed;
    }
    for (
        Int idx_key = 0; 
        idx_key < alternative->num_keys; 
        ++idx_key
    ) {
        Button key = alternative->keys[idx_key];
        alternative_is_pressed &= in_button(key).is_pressed;
    }
    return alternative_is_pressed;
}

Bool keybind_is_pressed(Keybind *keybind) {
    Bool keybind_is_pressed = FALSE;
    for (
        Int idx_alternative = 0; 
        idx_alternative < keybind->num_alternatives; 
        ++idx_alternative
    ) {
        Keybind_Alternative *alternative = 
            &keybind->alternatives[idx_alternative];
        keybind_is_pressed |= keybind_alternative_is_pressed(alternative);
    }
    return keybind_is_pressed;
}

Bool keybind_got_pressed(Keybind *keybind) {
    Bool keybind_got_pressed = FALSE;
    for (
        Int idx_alternative = 0; 
        idx_alternative < keybind->num_alternatives; 
        ++idx_alternative
    ) {
        Keybind_Alternative *alternative = 
            &keybind->alternatives[idx_alternative];
        Bool all_alternative_keys_are_pressed = 
            keybind_alternative_is_pressed(alternative);
        Bool any_alternative_key_got_pressed = FALSE;
        for (
            Int idx_key = 0; 
            idx_key < alternative->num_keys; 
            ++idx_key
        ) {
            Button key = alternative->keys[idx_key];
            if(in_button(key).got_pressed) {
                any_alternative_key_got_pressed = TRUE;
                break;
            }
        }
        keybind_got_pressed |=
            all_alternative_keys_are_pressed && any_alternative_key_got_pressed;
    }
    return keybind_got_pressed;
}

void keybind_to_cstr(Keybind *keybind, char *buffer);
void keybind_from_cstr(Keybind *keybind, char *cstr);
// ie.: keybind_from_cstr(&keybind, "Shift+A");
// ie.: keybind_from_cstr(&keybind, "SHIFT+A");
// ie.: keybind_from_cstr(&keybind, "Left Arrow key");

In_Button_Event keybind(Keybind *keybind);

//#define KEYBIND_FORMAT_MAX_BYTES (sizeof("Right Mouse Button") - 1)
//void keybind_format(S8 *dst, char *bytes, U8 alternative, Keybind *keybind);

#endif /* */