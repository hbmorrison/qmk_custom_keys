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

#include QMK_KEYBOARD_H

#include "custom_keys.h"

// The following _user() functions call the equivalent _custom_keys() functions
// since auto shift and caps word are not part of the QMK module API. If further
// auto shift or caps word customisation is not needed, this file can be added
// to a keymap rules.mk file with "SRC += custom_keys_user.c", to take care of
// the required function calls.

bool caps_word_press_user(uint16_t keycode) {
  return caps_word_press_custom_keys(keycode);
}

bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
  return get_custom_auto_shifted_key_custom_keys(keycode, record);
}

void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
  autoshift_press_custom_keys(keycode, shifted, record);
}

void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
  autoshift_release_custom_keys(keycode, shifted, record);
}
