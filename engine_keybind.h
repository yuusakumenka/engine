#ifdef __INTELLISENSE__
#include "engine_types.h"
#include "engine_module_input.h"
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
        alternative_is_pressed &= button_is_pressed(modifier_key);
    }
    for (
        Int idx_key = 0; 
        idx_key < alternative->num_keys; 
        ++idx_key
    ) {
        Button key = alternative->keys[idx_key];
        alternative_is_pressed &= button_is_pressed(key);
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
            if(button_got_pressed(key)) {
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
