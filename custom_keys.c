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

// Declare internal functions.

bool custom_keys_autoshift_press_us_intl(uint16_t, bool, keyrecord_t *);
bool custom_keys_autoshift_release_us_intl(uint16_t, bool, keyrecord_t *);

// Stores the state of the shift-backspace key override.

static bool ck_shift_backspace_pressed = false;

// Process custom macros.

bool process_record_custom_keys(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    switch (keycode) {

    // Shift-backspace issues delete.

    case KC_BSPC:
      if (record->event.pressed) {
        if (get_oneshot_mods() & MOD_BIT(KC_LSFT)) {
          del_oneshot_mods(MOD_BIT(KC_LSFT));
          register_code(KC_DEL);
          ck_shift_backspace_pressed = true;
          return false;
        }
      } else {
        if (ck_shift_backspace_pressed) {
          unregister_code(KC_DEL);
          ck_shift_backspace_pressed = false;
          return false;
        }
      }
      break;

      // Send the Vim command sequence - esc then colon - and immediately return
      // false to indicate that no further processing is required.

      case CK_VCMD:
        tap_code16(KC_ESC);
        wait_ms(100);
        tap_code16(KC_COLN);
        return false;

      // Send the SSH suspend sequence and immediately return false to indicate
      // that no further processing is required.

      case CK_SSHS:
        tap_code16(KC_ENT);
        tap_code16(KC_PIPE);
        tap_code16(RCTL(KC_Z));
        return false;

      // Send the keyboard shortcuts for 1Password quick access and auto type,
      // and immediately return false to indicate that no further processing is
      // required.

      case CK_OPQA:
        tap_code16(LSFT(LCTL(KC_SPC)));
        return false;
      case CK_OPAT:
        tap_code16(LSFT(KC_ENT));
        return false;
    }
  }

  // Continue processing the key.

  return true;
}

// Only capitalise alpha characters and ignore some symbols and other keys.

bool custom_keys_caps_word_press_user(uint16_t keycode) {
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

// The custom keys version of get_custom_auto_shifted_key().

bool custom_keys_get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {

    // Auto shift space and enter to help with 1Password shortcuts.

    case KC_SPC:
    case KC_ENT:
      return true;

    // Auto shift esc to issue esc + :.

    case KC_ESC:
      return true;

    // Auto shift the symbol keys present in the base layer.

    case KC_COMM:
    case KC_DOT:
    case KC_SLSH:
      return true;

    // Auto shift left and right arrow keys to issue home and end.

    case KC_LEFT:
    case KC_RIGHT:
      return true;

    // Auto shift left brace, parenthesis and curly brace to issue [],() or {}
    // as custom auto shifts.

    case KC_LBRC:
    case KC_LPRN:
    case KC_LCBR:
      return true;

    // Auto shift $ to issue ${}.

    case KC_DLR:
      return true;

    // Use auto shift as a mechanism to decide whether to issue US or
    // international versions of these symbol keycodes.

    case CK_AT:
    case CK_DQUO:
    case CK_GRV:
    case CK_TILD:
    case CK_BSLS:
    case CK_PIPE:
      return true;

    // Auto shift single quotes to issue '' when shifted.

    case KC_QUOT:
      return true;

    // Auto shift equals to issue => when shifted.

    case KC_EQL:
      return true;
  }

  // Auto shift any mod-tap and layer-tap keys so that auto shift will work.

  if (IS_RETRO(keycode))
    return true;

  // No other custom auto shift keys.

  return false;
}

// The custom keys version of autoshift_press_user().

void custom_keys_autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {

  // Process key presses for the custom symbol keys added by this module to
  // issue either US or international symbols.

  if (custom_keys_autoshift_press_us_intl(keycode, shifted, record))
    return;

  // If the custom key is not shifted, register the key press and return. Only
  // register the lower eight bits of the keycode, removing the upper eight bits
  // that mod tap uses, so that retro shift will work correctly.

  if (! shifted) {
    register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    return;
  }

  // Handle custom key behaviour.

  switch (keycode) {

    // Auto shift space to issue ctrl-shift-space.

    case KC_SPC:
      if (shifted)
        add_weak_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LCTL));
      register_code16(KC_SPC);
      break;

    // Auto shift esc to issue esc + :.

    case KC_ESC:
      if (shifted) {
        tap_code16(KC_ESC);
        wait_ms(100);
        tap_code16(KC_COLN);
      } else {
        register_code16(KC_ESC);
      }
      break;

    // Issue home and end when left and right arrow keys are shifted.

    case KC_LEFT:
      if (shifted)
        register_code16(KC_HOME);
      else
        register_code16(KC_LEFT);
      break;
    case KC_RIGHT:
      if (shifted)
        register_code16(KC_END);
      else
        register_code16(KC_RIGHT);
      break;

    // Issue [], () or {} respectively when the left brace, parenthesis or curly
    // brace keys are shifted.

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

    // Issue ${} when the dollar key is shifted.

    case KC_DLR:
      if (shifted) {
        tap_code16(KC_DLR);
        tap_code16(KC_LCBR);
        tap_code16(KC_RCBR);
        tap_code16(KC_LEFT);
      } else {
        register_code16(KC_DLR);
      }
      break;

    // Issue '' when the single quote key is shifted.

    case KC_QUOT:
      if (shifted) {
        tap_code16(KC_QUOT);
        tap_code16(KC_QUOT);
        tap_code16(KC_LEFT);
      } else {
        register_code16(keycode);
      }
      break;

    // Issue => when the equals key is shifted.

    case KC_EQL:
      if (shifted) {
        tap_code16(KC_EQL);
        tap_code16(KC_GT);
        tap_code16(KC_SPC);
      } else {
        register_code16(keycode);
      }
      break;

    // Add a weak shift modifier and remove the higher bits added to the keycode
    // by mod tap and layer tap before issuing the key press, so that auto shift
    // will work.

    default:
      if (shifted)
        add_weak_mods(MOD_BIT(KC_LSFT));
      register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
  }
}

// The custom keys version of autoshift_release_user(). Should be called from
// autoshift_release_user() in keymap.c, or by defining 

void custom_keys_autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {

  // Process key presses for the custom symbol keys added by this module to
  // issue either US or international symbols.

  if (custom_keys_autoshift_release_us_intl(keycode, shifted, record))
    return;

  // Process custom symbol key releases.

  switch (keycode) {

    // Space needs to be unregistered whether shifted or not.

    case KC_SPC:
      unregister_code16(keycode);
      break;

    // Issue the unshifted esc key release.

    case KC_ESC:
      if (! shifted)
        unregister_code16(KC_ESC);
      break;

    // Issue the key releases for left and right arrow keys.

    case KC_LEFT:
      if (shifted)
        unregister_code16(KC_HOME);
      else
        unregister_code16(KC_LEFT);
      break;
    case KC_RIGHT:
      if (shifted)
        unregister_code16(KC_END);
      else
        unregister_code16(KC_RIGHT);
      break;

    // Issue the key release for the keys with custom auto shifts if there was
    // no auto shift behaviour.

    case KC_LBRC:
    case KC_LPRN:
    case KC_LCBR:
    case KC_DLR:
    case KC_QUOT:
    case KC_EQL:
      if (! shifted)
        unregister_code16(keycode);
      break;

    // Remove the higher bits added to the keycode by mod tap and layer tap
    // before issuing the key release, so that auto shift will work.

    default:
      unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
  }
}

// Translates the custom symbol keycodes added by this module into either US or
// international symbol keycodes when the key is pressed. The double quote key also
// has a custom auto shift, so that it behave in the same way as the single
// quote key.

bool custom_keys_autoshift_press_us_intl(uint16_t keycode, bool shifted, keyrecord_t *record) {
  switch (keycode) {
    case CK_DETECT_AT:
      register_code16(CK_ISSUE_AT);
      return true;
    case CK_DETECT_DQUO:
      if (shifted)
        tap_code16(CK_ISSUE_DQUO);
      register_code16(CK_ISSUE_DQUO);
      return true;
    case CK_GRV:
      register_code16(CK_SYMBOL_GRV);
      return true;
    case CK_TILD:
      register_code16(CK_SYMBOL_TILD);
      return true;
    case CK_BSLS:
      register_code16(CK_SYMBOL_BSLS);
      return true;
    case CK_PIPE:
      register_code16(CK_SYMBOL_PIPE);
      return true;
  }

  // Indicate that the key press has not been handled.

  return false;
}

// this translates the custom symbol keycodes created by this module into either
// us or international symbol keycodes when a custom key is released. double
// quotes also have a custom auto shift.

bool custom_keys_autoshift_release_us_intl(uint16_t keycode, bool shifted, keyrecord_t *record) {
  switch (keycode) {
    case CK_DETECT_AT:
      unregister_code16(CK_ISSUE_AT);
      return true;
    case CK_DETECT_DQUO:
      unregister_code16(CK_ISSUE_DQUO);
      if (shifted)
        tap_code16(KC_LEFT);
      return true;
    case CK_GRV:
      unregister_code16(CK_SYMBOL_GRV);
      return true;
    case CK_TILD:
      unregister_code16(CK_SYMBOL_TILD);
      return true;
    case CK_BSLS:
      unregister_code16(CK_SYMBOL_BSLS);
      return true;
    case CK_PIPE:
      unregister_code16(CK_SYMBOL_PIPE);
      return true;
  }

  // Indicate that the key release has not been handled.

  return false;
}

// The following functions are usually defined within keymap.c to add custom
// behavious. They appear here because auto shift and caps word are not part of
// the QMK module API. Define CUSTOM_KEYS_INCLUDE_USER_FUNCTIONS in the keymap
// config.h if no further auto shift or caps word customisation is required. If
// the keymap uses any of these functions for customisation, add calls to the
// custom_keys_...() functions from all four user functions defined in keymap.c
// to maintain the custom keys functionality.

#ifdef CUSTOM_KEYS_INCLUDE_USER_FUNCTIONS
bool caps_word_press_user(uint16_t keycode) {
  return custom_keys_caps_word_press_user(keycode);
}

bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
  return custom_keys_get_custom_auto_shifted_key(keycode, record);
}

void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
  custom_keys_autoshift_press_user(keycode, shifted, record);
}

void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
  custom_keys_autoshift_release_user(keycode, shifted, record);
}
#endif
