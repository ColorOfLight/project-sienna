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

#include "./system/client_sync_system.h"

#include <emscripten/val.h>

#include <glm/glm.hpp>

namespace client_sync_system {

void syncInput(std::reference_wrapper<InputComponent> input_component) {
  emscripten::val client_input_component =
      emscripten::val::global("ClientInputComponent");

  emscripten::val pressed_key_map = client_input_component["pressedKeyMap"];

  input_component.get().pressed_key_map[InputKey::UP] =
      pressed_key_map["KeyW"].as<bool>();
  input_component.get().pressed_key_map[InputKey::LEFT] =
      pressed_key_map["KeyA"].as<bool>();
  input_component.get().pressed_key_map[InputKey::DOWN] =
      pressed_key_map["KeyS"].as<bool>();
  input_component.get().pressed_key_map[InputKey::RIGHT] =
      pressed_key_map["KeyD"].as<bool>();
  input_component.get().pressed_key_map[InputKey::FORWARD] =
      pressed_key_map["KeyR"].as<bool>();
  input_component.get().pressed_key_map[InputKey::BACKWARD] =
      pressed_key_map["KeyF"].as<bool>();

  emscripten::val is_pointer_down = client_input_component["isPointerDown"];

  input_component.get().is_pointer_down = is_pointer_down.as<bool>();

  emscripten::val pointer_position = client_input_component["pointerPosition"];

  input_component.get().pointer_position.x = pointer_position[0].as<float>();
  input_component.get().pointer_position.y = pointer_position[1].as<float>();

  emscripten::val canvas_size = client_input_component["canvasSize"];

  input_component.get().canvas_size.width = canvas_size[0].as<int>();
  input_component.get().canvas_size.height = canvas_size[1].as<int>();

  emscripten::val brush_input = client_input_component["brush"];

  input_component.get().brush_input.air_pressure =
      brush_input["airPressure"].as<float>();
  input_component.get().brush_input.nozzle_fov =
      glm::radians(brush_input["nozzleFov"].as<float>());

  input_component.get().brush_input.paint_color.r =
      brush_input["paintColor"]["r"].as<float>();
  input_component.get().brush_input.paint_color.g =
      brush_input["paintColor"]["g"].as<float>();
  input_component.get().brush_input.paint_color.b =
      brush_input["paintColor"]["b"].as<float>();
}

void consumeEvent(std::reference_wrapper<EventComponent> event_component) {
  emscripten::val client_event_component =
      emscripten::val::global("ClientEventComponent");

  bool is_reset = client_event_component["reset"].as<bool>();
  bool change_canvas_size =
      client_event_component["changeCanvasSize"].as<bool>();

  if (is_reset) {
    event_component.get().reset = true;
    client_event_component.set("reset", false);
  }

  if (change_canvas_size) {
    event_component.get().change_canvas_size = true;
    client_event_component.set("changeCanvasSize", false);
  }
}

}  // namespace client_sync_system
