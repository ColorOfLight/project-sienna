/*
 * MIT License
 *
 * Copyright (c) 2024 Seongho Park
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "./System/ClientSyncSystem.h"

#include <emscripten/val.h>

void ClientSyncSystem::syncInput(
    std::reference_wrapper<InputComponent> input_component) {
  emscripten::val client_input_component =
      emscripten::val::global("ClientInputComponent");

  emscripten::val pressed_key_map = client_input_component["pressedKeyMap"];

  input_component.get().pressed_key_map[InputKey::W] =
      pressed_key_map["W"].as<bool>();
  input_component.get().pressed_key_map[InputKey::A] =
      pressed_key_map["A"].as<bool>();
  input_component.get().pressed_key_map[InputKey::S] =
      pressed_key_map["S"].as<bool>();
  input_component.get().pressed_key_map[InputKey::D] =
      pressed_key_map["D"].as<bool>();

  emscripten::val is_pointer_down = client_input_component["isPointerDown"];

  input_component.get().is_pointer_down = is_pointer_down.as<bool>();

  emscripten::val pointer_position = client_input_component["pointerPosition"];

  input_component.get().pointer_position.x = pointer_position[0].as<float>();
  input_component.get().pointer_position.y = pointer_position[1].as<float>();
}
