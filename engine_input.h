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

typedef unsigned char Button;
typedef enum Button_ {
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

    // keyboard
    BUTTON_BACKSPACE = 0x08,
    BUTTON_TAB       = 0x09,
    BUTTON_CLEAR     = 0x0C,
    BUTTON_ENTER     = 0x0D,
    BUTTON_RETURN    = BUTTON_ENTER,

    // alter              
    BUTTON_SHIFT   = 0x10,
    BUTTON_CTRL    = 0x11,
    BUTTON_CONTROL = BUTTON_CTRL,
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

    BUTTON_DEL = 0x2E,

    // controller buttons            
    BUTTON_RIGHT_THUMB_DOWN   = 128,
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

    // sticks             
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
    BUTTON_NINTENDO_MINUS = BUTTON_SELECT
} Button_;

B8 button_is_pressed  (Button button);
B8 button_got_pressed (Button button);
B8 button_is_released (Button button);
B8 button_got_released(Button button);

V2 mouse_position();
V2 mouse_delta();
V2 mouse_wheel();

void mouse_set_position(F32 x, F32 y);