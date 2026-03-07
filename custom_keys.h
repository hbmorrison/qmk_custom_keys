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

#include QMK_KEYBOARD_H

// Aliases for modified keys and UK symbols (backslash and pipe).

#define A_ST S(KC_TAB)
#define A_BS KC_NUBS
#define A_PP S(KC_NUBS)

// Aliases for cut, copy and paste.

#define A_CX RCTL(KC_X)
#define A_CC RCTL(KC_C)
#define A_CV RCTL(KC_V)

// 1Password shortcut for quick access.

#define P_QA C(S(KC_SPC))
#define P_AT S(KC_ENT)

// Aliases for Windows previous and next tab / window / desktop.

#define W_PT C(S(KC_TAB))
#define W_NT C(KC_TAB)
#define W_PD C(G(KC_LEFT))
#define W_ND C(G(KC_RIGHT))

// These functions need to be called from the equivalent _user() functions in
// the keymap because the QMK module API does not support auto shift or caps
// word. This can be accomplished by adding "SRC += custom_keys_user.c" to the
// rules.mk file in the user keymap, if the auto shift functions do not need
// further customisation.

bool custom_keys_caps_word_press_user(uint16_t);
bool custom_keys_get_custom_auto_shifted_key(uint16_t, keyrecord_t *);
void custom_keys_autoshift_press_user(uint16_t, bool, keyrecord_t *);
void custom_keys_autoshift_release_user(uint16_t, bool, keyrecord_t *);
