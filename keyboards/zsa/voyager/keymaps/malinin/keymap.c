/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "os_detection.h"

enum layers { BASE, RU, SYMBOLS, MEDIA_NAV, NUM };

enum custom_keycodes {
    SEL_ALL = SAFE_RANGE,
    UNDO_X,
    CUT_X,
};

enum tap_dance_codes {
    TD_COPY_PASTE,
};

enum keycode_aliases {
    // Aliases for home row mods.
    HRM_C = CTL_T(KC_C),
    HRM_I = LT(SYMBOLS, KC_I),
    HRM_A = ALT_T(KC_A),
    HRM_E = GUI_T(KC_E),
    HRM_H = RGUI_T(KC_H),
    HRM_T = ALGR_T(KC_T),
    HRM_N = LT(SYMBOLS, KC_N),
    HRM_S = RCTL_T(KC_S),

    // Aliases for home row mods on QWERTY
    HRM_QWERTY_A = CTL_T(KC_A),
    HRM_QWERTY_S = LT(SYMBOLS, KC_S),
    HRM_QWERTY_D = ALT_T(KC_D),
    HRM_QWERTY_F = GUI_T(KC_F),
    HRM_QWERTY_J = RGUI_T(KC_J),
    HRM_QWERTY_K = ALGR_T(KC_K),
    HRM_QWERTY_L = LT(SYMBOLS, KC_L),
    HRM_QWERTY_SCLN = RCTL_T(KC_SCLN),

    // One-shot mod keys.
    OSM_LCTL = OSM(MOD_LCTL),
    OSM_LSFT = OSM(MOD_LSFT),
    OSM_LALT = OSM(MOD_LALT),
    OSM_LGUI = OSM(MOD_LGUI),

    EMOJI = LCTL(LGUI(KC_SPC)),
};

#define DUAL_FUNC_0 LT(11, KC_N)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT_voyager(
    KC_NO,  UNDO_X,   CUT_X,   SEL_ALL,  TD(TD_COPY_PASTE),     KC_GRV,            LGUI(LSFT(KC_4)), CW_TOGG, KC_NO, KC_NO, KC_NO, TG(RU),
    KC_ESC, KC_Z,     KC_Y,    KC_O,     KC_U,                  KC_EQL,            KC_Q,             KC_L,    KC_D,  KC_P,  KC_X,  KC_BSPC,
    KC_W,   HRM_C,    HRM_I,   HRM_A,    HRM_E,                 KC_SCLN,           KC_K,             HRM_H,   HRM_T, HRM_N, HRM_S, KC_F,
    KC_NO,  KC_QUOTE, KC_MINS, KC_COMM,  KC_DOT,                KC_SLASH,          KC_J,             KC_M,    KC_G,  KC_B,  KC_V,  KC_ENT,
                                         LT(MEDIA_NAV, KC_SPC), OSM_LSFT,          MEH_T(KC_TAB),    LT(NUM, KC_R)
  ),
  [RU] = LAYOUT_voyager(
    KC_TRNS, KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,          KC_TRNS, KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,         KC_TRNS,
    KC_TRNS, KC_Q,         KC_W,         KC_E,         KC_R,         KC_T,             KC_Y,    KC_U,         KC_I,         KC_O,         KC_P,            KC_TRNS,
    KC_LBRC, HRM_QWERTY_A, HRM_QWERTY_S, HRM_QWERTY_D, HRM_QWERTY_F, KC_G,             KC_H,    HRM_QWERTY_J, HRM_QWERTY_K, HRM_QWERTY_L, HRM_QWERTY_SCLN, KC_QUOT,
    KC_RBRC, KC_Z,         KC_X,         KC_C,         KC_V,         KC_B,             KC_N,    KC_M,         KC_COMM,      KC_DOT,       KC_SLASH,        KC_TRNS,
                                                       KC_TRNS,      KC_TRNS,          KC_TRNS, KC_TRNS
),
  [SYMBOLS] = LAYOUT_voyager(
    KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,   KC_NO,            KC_NO,   EMOJI,   KC_NO,   KC_NO,   KC_NO,    KC_NO,
    KC_TRNS, KC_GRV,  KC_LABK, KC_RABK,  KC_MINS, KC_PIPE,          KC_CIRC, KC_LCBR, KC_RCBR, KC_DLR,  KC_BSLS,  KC_TRNS,
    KC_NO,   KC_EXLM, KC_ASTR, KC_SLSH,  KC_EQL,  KC_AMPR,          KC_HASH, KC_LPRN, KC_RPRN, KC_SCLN, KC_DQUO,  KC_NO,
    KC_NO,   KC_TILD, KC_PLUS, KC_COMM,  KC_DOT,  KC_PERC,          KC_AT,   KC_LBRC, KC_RBRC, KC_COLN, KC_QUOT, KC_TRNS,
                                         KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS
  ),
  [MEDIA_NAV] = LAYOUT_voyager(
    KC_NO,   KC_BRID,  KC_BRIU,  KC_VOLD,  KC_VOLU,  KC_MUTE,          KC_NO,   KC_NO,              KC_NO,   KC_NO,         KC_NO,       KC_NO,
    KC_TRNS, KC_MSTP,  KC_MPRV,  KC_MNXT,  KC_MPLY,  KC_NO,            KC_PGUP, LCTL(LSFT(KC_TAB)), KC_UP,   LCTL(KC_TAB),  KC_HOME,     KC_NO,
    KC_NO,   OSM_LCTL, OSM_LSFT, OSM_LALT, OSM_LGUI, MS_BTN1,          KC_PGDN, KC_LEFT,            KC_DOWN, KC_RIGHT,      KC_END,      KC_NO,
    KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,            KC_MCTL, LGUI(KC_LBRC),      KC_APP,  LGUI(KC_RBRC), DUAL_FUNC_0, KC_NO,
                                           KC_TRNS,  KC_TRNS,          KC_TRNS, KC_TRNS
  ),
  [NUM] = LAYOUT_voyager(
    KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,            KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
    KC_TRNS, KC_7,     KC_5,     KC_3,     KC_1,     KC_9,             KC_8,    KC_0,     KC_2,     KC_4,     KC_6,     KC_TRNS,
    KC_NO,   OSM_LCTL, OSM_LSFT, OSM_LALT, OSM_LGUI, KC_F11,           KC_F10,  OSM_LGUI, OSM_LALT, OSM_LSFT, OSM_LCTL, KC_NO,
    KC_NO,   KC_F7,    KC_F5,    KC_F3,    KC_F1,    KC_F9,            KC_F8,   KC_F12,   KC_F2,    KC_F4,    KC_F6,    KC_TRNS,
                                           KC_TRNS,  KC_TRNS,          KC_TRNS, KC_TRNS
  ),
};

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
  '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*',
  '*', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', '*',
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R',
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', '*',
                      'L', 'L', 'R', 'R'
);

const custom_shift_key_t custom_shift_keys[] = {
  {KC_BSPC, KC_DELETE}, // Shift + Backspace = Delete
};


void keyboard_post_init_user(void) {
  // Set the effect.
  rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_PALETTEFX_FLOW);
  // Set the palette and maximize saturation and brightness.
  uint8_t palette_index = PALETTEFX_POLARIZED;  // Set Carnival palette.
  rgb_matrix_sethsv_noeeprom(RGB_MATRIX_HUE_STEP * palette_index, 255, 255);
  // Set speed to default.
  rgb_matrix_set_speed_noeeprom(128);
  // Make sure RGB Matrix is on.
  rgb_matrix_enable_noeeprom();
}

static void send_os_shortcut(uint16_t mac_shortcut, uint16_t other_shortcut) {
  os_variant_t os = detected_host_os();
  switch (os) {
    case OS_MACOS:
    case OS_IOS:
      tap_code16(mac_shortcut);
      break;
    default:
      tap_code16(other_shortcut);
  }
}

#ifdef TAP_DANCE_ENABLE
void td_copy_paste_finished(tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    send_os_shortcut(LGUI(KC_C), LCTL(KC_C));
  } else {
    send_os_shortcut(LGUI(KC_V), LCTL(KC_V));
  }
}

void td_copy_paste_reset(tap_dance_state_t *state, void *user_data) {
}

tap_dance_action_t tap_dance_actions[] = {
  [TD_COPY_PASTE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_copy_paste_finished, td_copy_paste_reset),
};
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case UNDO_X:
      if (record->event.pressed) {
        send_os_shortcut(LGUI(KC_Z), LCTL(KC_Z));
      }
      return false;
    case CUT_X:
      if (record->event.pressed) {
        send_os_shortcut(LGUI(KC_X), LCTL(KC_X));
      }
      return false;
    case SEL_ALL:
      if (record->event.pressed) {
        send_os_shortcut(LGUI(KC_A), LCTL(KC_A));
      }
      return false;
    case DUAL_FUNC_0:
      if (record->tap.count > 0) {
        if (record->event.pressed) {
          register_code16(KC_Q);
        } else {
          unregister_code16(KC_Q);
        }
      } else {
        if (record->event.pressed) {
          register_code16(KC_H);
        } else {
          unregister_code16(KC_H);
        }
      }
      return false;
  }
  return true;
}
