// Copyright 2026 Hannah Blythe Morrison
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "custom_keys.h"

// Process custom macros.

bool process_record_custom_keys(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    switch (keycode) {

      // Send the Vim command sequence - esc then colon - and immediately return
      // false to indicate that no further processing is required.

      case CK_VC:
        tap_code16(KC_ESC);
        wait_ms(100);
        tap_code16(KC_COLN);
        return false;

      // Send the SSH suspend sequence and immediately return false to indicate
      // that no further processing is required.

      case CK_SS:
        tap_code16(KC_ENT);
        tap_code16(KC_PIPE);
        tap_code16(RCTL(KC_Z));
        return false;
    }
  }

  // Continue processing the key.

  return true;
}

// Only capitalise alpha characters and ignore some symbols and other keys.

bool caps_word_press_custom_keys(uint16_t keycode) {
  switch (keycode) {

    // Alpha keys on the left side get capitalised.

    case KC_Q:
    case KC_W:
    case KC_F:
    case KC_P:
    case KC_B:
    case KC_A:
    case KC_R:
    case KC_S:
    case KC_T:
    case KC_G:
    case KC_Z:
    case KC_X:
    case KC_C:
    case KC_D:
    case KC_V:
      add_weak_mods(MOD_BIT(KC_RSFT));
      return true;

    // Alpha keys on the right side get capitalised.

    case KC_J:
    case KC_L:
    case KC_U:
    case KC_Y:
    case KC_M:
    case KC_N:
    case KC_E:
    case KC_I:
    case KC_O:
    case KC_K:
    case KC_H:
      add_weak_mods(MOD_BIT(KC_LSFT));
      return true;

    // Number keys, underscore, backspace and del continue caps word but are not
    // shifted themselves.

    case KC_1 ... KC_0:
    case KC_UNDS:
    case KC_BSPC:
    case KC_DEL:
      return true;

    // Allow tab to continue caps word for shell completion of variable names.

    case KC_TAB:
      return true;
  }

  // Some keys in higher layers also continue caps word so layer-tap keys must
  // also continue caps word.

  if (IS_QK_LAYER_TAP(keycode))
    return true;

  // Everything else stops caps word.

  return false;
}

bool get_custom_auto_shifted_key_custom_keys(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {

    // Auto shift the symbol keys present in the base layer.

    case KC_COMM:
    case KC_DOT:
    case KC_SLSH:
      return true;

    // Auto shift space and enter keys to help with 1Password shortcuts.

    case KC_SPC:
    case KC_ENT:
      return true;

    // Auto shift left brace, parenthesis and curly brace to issue [],() or {}
    // as custom auto shifts.

    case KC_LBRC:
    case KC_LPRN:
    case KC_LCBR:
      return true;
  }

  // Auto shift any layer-tap keys.

  if (IS_QK_LAYER_TAP(keycode))
    return true;

  // Do not auto shift other keys.

  return false;
}

void autoshift_press_custom_keys(uint16_t keycode, bool shifted, keyrecord_t *record) {
  switch (keycode) {

    // When the left brace, parenthesis and curly brace is shifted, a pair of
    // open and closed keycodes are issued, followed by a left keycode to put
    // the cursor between them.

    case KC_LBRC:
    case KC_LPRN:
    case KC_LCBR:
      if (shifted) {
        tap_code16(keycode);
        tap_code16(keycode + 1);
        tap_code16(KC_LEFT);
      } else {
        register_code16(keycode);
      }
      break;

    // By default, add a weak shift modifier and only register the lower eight
    // bits of the keycode if the key is retro shifted - removing the upper
    // eight bits that mod tap uses - so that retro shift will work correctly.

    default:
      if (shifted)
        add_weak_mods(MOD_BIT(KC_LSFT));
      register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
  }
}

void autoshift_release_custom_keys(uint16_t keycode, bool shifted, keyrecord_t *record) {
  switch (keycode) {

    // Release the key if these special cases were not shifted.

    case KC_LBRC:
    case KC_LPRN:
    case KC_LCBR:
      if (! shifted)
        unregister_code16(keycode);
      break;

    // Only unregister the lower eight bits of the keycode if the key is retro
    // shifted - removing the upper eight bits that mod tap uses - so that retro
    // shift will work correctly.

    default:
      unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
  }
}
