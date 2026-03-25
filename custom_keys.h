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

#pragma once

// Include the standard QMK headers.

#include QMK_KEYBOARD_H

// The CK_SYMBOL_ keycodes are defined to be the correct keycodes for either US or
// international layouts.

#ifdef CUSTOM_KEYS_US_LAYOUT
#  define CK_SYMBOL_TILD KC_TILD
#  define CK_SYMBOL_GRV  KC_GRV
#  define CK_SYMBOL_PIPE KC_PIPE
#  define CK_SYMBOL_BSLS KC_BSLS
#else
#  define CK_SYMBOL_TILD S(KC_NUBS)
#  define CK_SYMBOL_GRV  KC_NUBS
#  define CK_SYMBOL_PIPE S(KC_BSLS)
#  define CK_SYMBOL_BSLS KC_BSLS
#endif

// Because custom auto shift behaviour is attached to the double quote key, the
// CK_AT and CK_DQUO custom keycodes defined by this module must be swapped for
// international layouts, so that the auto shift behaviour is associated with
// the correct key on the keyboard. However, in order to issue the correct
// symbols, the keycodes for the at sign and double quote symbols must also be
// swapped, so CK_DETECT_AT and CK_DETECT_DQUO have the correct custom keycodes
// for detection and CK_CK_ISSUE_AT and CK_CK_ISSUE_DQUO have the correct
// keycodes to issue.
//
// This seems unneccessary but it is needed and it works. It might be better to
// name the custom key codes defined by this module after key positions rather
// than the equivalent US keycode, but that would make the keymap layouts that
// appear in keymap.c less readable: CK_AT appearing in the middle of the top
// row on the left side makes more sense than CK_LEFT_TOP_MIDDLE. The other
// alternative would be to give the at sign the same behaviour, so long presses
// issue @@.

#ifdef CUSTOM_KEYS_US_LAYOUT
#  define CK_DETECT_AT   CK_AT
#  define CK_DETECT_DQUO CK_DQUO
#  define CK_ISSUE_AT    KC_AT
#  define CK_ISSUE_DQUO  KC_DQUO
#else
#  define CK_DETECT_AT   CK_DQUO
#  define CK_DETECT_DQUO CK_AT
#  define CK_ISSUE_AT    S(KC_QUOT)
#  define CK_ISSUE_DQUO  S(KC_2)
#endif

// These functions should be called from the equivalent _user() functions in
// keymap.c because the QMK module API does not support auto shift or caps word.
// If the keymap does not need further caps word or auto shift customisation,
// this can be accomplished by defining CUSTOM_KEYS_INCLUDE_USER_FUNCTIONS in
// the keymap config.h file.

bool custom_keys_caps_word_press_user(uint16_t);
bool custom_keys_get_custom_auto_shifted_key(uint16_t, keyrecord_t *);
void custom_keys_autoshift_press_user(uint16_t, bool, keyrecord_t *);
void custom_keys_autoshift_release_user(uint16_t, bool, keyrecord_t *);
