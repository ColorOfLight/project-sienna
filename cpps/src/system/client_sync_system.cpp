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
      emscripten::val::global("clientInputComponent");

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
      emscripten::val::global("clientEventComponent");

  bool change_canvas_size =
      client_event_component["changeCanvasSize"].as<bool>();

  if (client_event_component["updateCanvasSize"] !=
      emscripten::val::undefined()) {
    event_component.get().update_canvas_size =
        glm::ivec2(client_event_component["updateCanvasSize"][0].as<int>(),
                   client_event_component["updateCanvasSize"][1].as<int>());
    client_event_component.set("updateCanvasSize",
                               emscripten::val::undefined());
  }

  if (client_event_component["changeModel"] != emscripten::val::undefined()) {
    event_component.get().update_model = static_cast<ModelOptions>(
        client_event_component["changeModel"].as<int>());
    client_event_component.set("changeModel", emscripten::val::undefined());
  }

  if (client_event_component["resetPaint"] != emscripten::val::undefined()) {
    event_component.get().reset_paint = std::monostate();
    client_event_component.set("resetPaint", emscripten::val::undefined());
  }

  if (client_event_component["resetPosition"] != emscripten::val::undefined()) {
    event_component.get().reset_position = std::monostate();
    client_event_component.set("resetPosition", emscripten::val::undefined());
  }
}

}  // namespace client_sync_system
