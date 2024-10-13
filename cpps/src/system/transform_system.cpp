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

#include "./system/transform_system.h"

#include <cmath>
#include <glm/gtc/constants.hpp>

namespace transform_system {

const float rotation_speed = glm::pi<float>() * 2.0f / 1000.0f;

float modulateRotation(float angle);

void transformCamera(
    float delta_ms,
    std::reference_wrapper<const InputComponent> input_component,
    std::reference_wrapper<CameraComponent> camera_component) {
  const auto& pressed_key_map = input_component.get().pressed_key_map;

  bool is_up =
      pressed_key_map.at(InputKey::UP) && !pressed_key_map.at(InputKey::DOWN);
  bool is_down =
      !pressed_key_map.at(InputKey::UP) && pressed_key_map.at(InputKey::DOWN);
  bool is_left = pressed_key_map.at(InputKey::LEFT) &&
                 !pressed_key_map.at(InputKey::RIGHT);
  bool is_right = !pressed_key_map.at(InputKey::LEFT) &&
                  pressed_key_map.at(InputKey::RIGHT);

  if (is_up) {
    camera_component.get().phi -= rotation_speed * delta_ms;
    camera_component.get().needs_update = true;
  }

  if (is_down) {
    camera_component.get().phi += rotation_speed * delta_ms;
    camera_component.get().needs_update = true;
  }

  if (is_left) {
    camera_component.get().theta -= rotation_speed * delta_ms;
    camera_component.get().needs_update = true;
  }

  if (is_right) {
    camera_component.get().theta += rotation_speed * delta_ms;
    camera_component.get().needs_update = true;
  }
}

float modulateRotation(float angle) {
  float range = -glm::two_pi<float>();

  float modValue = std::fmod(angle, range);

  if (modValue > 0.0f) {
    modValue += range;
  }
  return modValue;
}

}  // namespace transform_system
