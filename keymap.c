/* Copyright 2021 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// clang-format off

enum layers{
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
    LAY,
    L2, // Layer 2, used because takes less space
    MLAY,
    ML2,
};

enum{
    TD_LOOK,
    TD_SPACE,
    TD_FOLD,
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_LOOK] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_F), LCTL(KC_P)),
    [TD_SPACE] = ACTION_TAP_DANCE_TAP_HOLD(KC_SPC, KC_LSFT),
    [TD_FOLD] = ACTION_TAP_DANCE_DOUBLE(KC_FLXP, KC_ENT),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(TD_SPACE):  // list all tap dance keycodes with tap-hold configurations
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
    }
    return true;
}



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_82(
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_NO,    KC_NO,    RM_VALD,  RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_DEL,             KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LOPT,  KC_LCMD,                                KC_SPC,                                 KC_RCMD,  MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_ansi_82(
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),

    [WIN_BASE] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_DEL,             KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
        MO(LAY),  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LCMD,  KC_LALT,                                KC_SPC,                                 MO(WIN_FN),  KC_RCMD, KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_ansi_82(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RM_VALD,  RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,            _______,
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,  _______,            _______,
        RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,  _______,  _______,  _______,  KC_UP,    KC_VOLD,  KC_VOLU,    _______,  _______,            _______,
        _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_RCMD,              _______,            _______,
        _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  KC_ESC,   KC_MPRV,  KC_MPLY,  KC_MNXT,              _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),
    
    [LAY] = LAYOUT_ansi_82(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RM_VALD,  RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_CAPS,            _______,
        LCTL(KC_1),  LCTL(KC_C),  LCTL(KC_V),  LCTL(KC_W),  TD(TD_LOOK),  LCTL(KC_T),  TD(TD_FOLD),  _______,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_VOLD,  KC_VOLU,  _______,            _______,
        LCTL(KC_L),  LCTL(KC_A),  KC_GRV,      KC_UP,   LCTL(KC_PGUP),   KC_HOME,  KC_TASK,  KC_7,     KC_8,     KC_9,     KC_MINS,  KC_COMM,    KC_PGUP,  _______,            _______,
        _______,  KC_ESC,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_END,   KC_SLSH,  KC_4,     KC_5,     KC_6,     KC_PLUS,  KC_DOT,              _______,            _______,
        _______,            KC_LSFT,  MO(L2),   KC_BSPC,  LCTL(KC_PGDN),  LCTL(KC_Z),  KC_ASTR,  KC_1,     KC_2,     KC_3,     KC_0,              KC_PGDN,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),

    [L2] = LAYOUT_ansi_82(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RM_VALD,  RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  LCTL(LALT(KC_DEL)),  _______,  _______,  _______,  _______,  _______,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,  _______,    _______,  _______,            _______,
        _______,  KC_SCRL,  _______,  LCTL(KC_LEFT),  KC_NUM,   _______,  _______,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_EQL,   _______,              _______,            _______,
        _______,            _______,  _______,  LCTL(KC_RGHT),  _______,  _______,  KC_UNDS,  KC_EXLM,  KC_AT,  KC_HASH,  _______,              _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),
        
    [MLAY] = LAYOUT_ansi_82(
        _______,   _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,             _______,
        _______,   _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,   _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,   _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,             _______,
        _______,             _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,   _______,             _______,  _______,
        _______,  _______,  _______,                                _______,                                 _______,  _______, _______,  _______,  _______,  _______),
    
    [ML2] = LAYOUT_ansi_82(
        _______,   _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,             _______,
        _______,   _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,   _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,   _______,  _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,             _______,
        _______,             _______,  _______,    _______,    _______,  _______,  _______,  _______,  _______,  _______,   _______,             _______,  _______,
        _______,  _______,  _______,                                _______,                                 _______,  _______, _______,  _______,  _______,  _______),


};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)},
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)},
    [LAY]   = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT)},
    [L2]   = { ENCODER_CCW_CW(KC_DOWN, KC_UP)},
    [MLAY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [ML2]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)},
};
#endif // ENCODER_MAP_ENABLE
