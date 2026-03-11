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

// These functions should be called from the equivalent _user() functions in
// keymap.c because the QMK module API does not support auto shift or caps word.
// If the keymap odes not need further caps word or auto shift cusomisation,
// this can be accomplished by defining CUSTOM_KEYS_INCLUDE_USER_FUNCTIONS in
// the keymap config.h file.

bool custom_keys_caps_word_press_user(uint16_t);
bool custom_keys_get_custom_auto_shifted_key(uint16_t, keyrecord_t *);
void custom_keys_autoshift_press_user(uint16_t, bool, keyrecord_t *);
void custom_keys_autoshift_release_user(uint16_t, bool, keyrecord_t *);
