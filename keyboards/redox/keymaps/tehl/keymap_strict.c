#include "keymap.h"
#include "keymap_strict.h"

bool strict_mode_enabled_flag = false;

const uint16_t PROGMEM strict_keys_left[] = {
    KC_1    ,KC_2    ,KC_3    ,KC_4    ,KC_5    ,
    KC_Q    ,KC_W    ,KC_E    ,KC_R    ,KC_T    ,KC_NUHS ,
    KC_A    ,KC_S    ,KC_D    ,KC_F    ,KC_G    ,KC_LBRC ,
    KC_Z    ,KC_X    ,KC_C    ,KC_V    ,KC_B    ,
    KC_NUBS
};

const uint16_t PROGMEM strict_keys_right[] = {
             KC_6    ,KC_7    ,KC_8    ,KC_9    ,KC_0    ,KC_MINS ,
    KC_QUOT ,KC_Y    ,KC_U    ,KC_I    ,KC_O    ,KC_P    ,KC_EQL  ,
    KC_RBRC ,KC_H    ,KC_J    ,KC_K    ,KC_L    ,KC_SCLN ,
             KC_N    ,KC_M    ,KC_COMM ,KC_DOT  ,KC_SLSH
};

const uint16_t PROGMEM strict_keys_permitted_combos[] = {
    KC_S, KC_Z, KC_X, KC_C, KC_V
};

bool array_contains(uint16_t value, const uint16_t array[], uint16_t array_size) {
    uint16_t i;
    uint16_t element;
    for (i = 0; i < array_size; i++) {
        element = pgm_read_word(&array[i]);
        if (element == value) {
            return true;
        }
    }
    return false;
}

bool is_strict_left_key(uint16_t keycode) {
    return array_contains(keycode, strict_keys_left, sizeof strict_keys_left / sizeof *strict_keys_left);
}

bool is_strict_right_key(uint16_t keycode) {
    return array_contains(keycode, strict_keys_right, sizeof strict_keys_right / sizeof *strict_keys_right);
}

bool is_strict_permitted_combo(uint16_t keycode, uint8_t mod_state) {
    bool is_key_combo = mod_state & MOD_MASK_CTRL;
    if (!is_key_combo) {
        return false;
    }

    return array_contains(keycode, strict_keys_permitted_combos, sizeof strict_keys_permitted_combos / sizeof *strict_keys_permitted_combos);
}

bool is_strict_mode_enabled(void) {
    if (!strict_mode_enabled_flag) {
        return false;
    }

    layer_state_t game_layer = 1UL << _GAME;
    return (layer_state & game_layer) != game_layer;
}

uint8_t adjust_mods_by_hand(uint8_t mod_state, bool left_hand, bool right_hand) {
    if (!left_hand) {
        mod_state &= ~MOD_BIT(KC_LSFT);
        mod_state &= ~MOD_BIT(KC_LCTL);
    }

    if (!right_hand) {
        mod_state &= ~MOD_BIT(KC_RSFT);
        mod_state &= ~MOD_BIT(KC_RCTL);
    }

    return mod_state;
}

void register_strict_code(uint16_t keycode, bool left_hand, bool right_hand) {
    uint8_t original_mods = get_mods();

    if (is_strict_permitted_combo(keycode, original_mods)) {
        register_code(keycode);
        return;
    }

    uint8_t strict_mods = adjust_mods_by_hand(original_mods, left_hand, right_hand);

    if (strict_mods != original_mods) {
        set_mods(strict_mods);
        send_keyboard_report();
    }

    register_code(keycode);

    if (strict_mods != original_mods) {
        set_mods(original_mods);
        send_keyboard_report();
    }
}

bool process_record_strict_mods(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_STRICT_TOGGLE) {
        if (record->event.pressed) {
            strict_mode_enabled_flag = !strict_mode_enabled_flag;
        }
        return false;
    }

    if (!is_strict_mode_enabled()) {
        return true;
    }

    if (is_strict_left_key(keycode)) {
        if (record->event.pressed) {
            /* left-handed keys must be used with right-handed modifiers */
            register_strict_code(keycode, false, true);
        } else {
            unregister_code(keycode);
        }

        return false;
    }

    if (is_strict_right_key(keycode)) {
        if (record->event.pressed) {
            /* right-handed keys must be used with left-handed modifiers */
            register_strict_code(keycode, true, false);
        } else {
            unregister_code(keycode);
        }

        return false;
    }

    return true;
}
