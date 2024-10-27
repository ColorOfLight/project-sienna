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

#pragma once

#include <glm/glm.hpp>
#include <map>

enum class InputKey {
  UP = 0,
  DOWN = 1,
  LEFT = 2,
  RIGHT = 3,
  FORWARD = 4,
  BACKWARD = 5,
};

struct PointerPosition {
  float x;
  float y;
};

struct CanvasSize {
  int width;
  int height;
};

struct BrushInput {
  float air_pressure;
  float nozzle_fov;
  glm::vec3 paint_color;
};

class InputComponent {
 public:
  InputComponent() {
    pressed_key_map[InputKey::UP] = false;
    pressed_key_map[InputKey::LEFT] = false;
    pressed_key_map[InputKey::DOWN] = false;
    pressed_key_map[InputKey::RIGHT] = false;
    pressed_key_map[InputKey::FORWARD] = false;
    pressed_key_map[InputKey::BACKWARD] = false;
    is_pointer_down = false;
    pointer_position.x = 0;
    pointer_position.y = 0;
    brush_input.air_pressure = 0.5f;
    brush_input.nozzle_fov = glm::radians(15.0f);
    brush_input.paint_color = glm::vec3(1.0f, 0.0f, 0.0f);
  }

  std::unordered_map<InputKey, bool> pressed_key_map;
  bool is_pointer_down;
  PointerPosition pointer_position;
  BrushInput brush_input;
};
