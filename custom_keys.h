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

// These functions need to be called from the equivalent _user() functions in
// the keymap because the QMK module API does not support auto shift or caps
// word. This can be accomplished by adding "SRC += custom_keys_user.c" to the
// rules.mk file in the user keymap, if the auto shift functions do not need
// further customisation.

bool caps_word_press_custom_keys(uint16_t);
bool get_custom_auto_shifted_key_custom_keys(uint16_t, keyrecord_t *);
void autoshift_press_custom_keys(uint16_t, bool, keyrecord_t *);
void autoshift_release_custom_keys(uint16_t, bool, keyrecord_t *);
