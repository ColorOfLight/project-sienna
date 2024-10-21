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

#include "./math_util.h"

namespace transform_system {

const float rotation_speed = glm::pi<float>() * 1.0f / 1000.0f;
const float zoom_speed = 2.0f / 1000.0f;
const float min_radius = 1.0f;

float modulateRotation(float angle);

void transformCamera(float delta_ms,
                     std::reference_wrapper<InputComponent> input_component,
                     std::reference_wrapper<CameraComponent> camera_component) {
  const auto& pressed_key_map = input_component.get().pressed_key_map;

  bool is_forward = pressed_key_map.at(InputKey::FORWARD) &&
                    !pressed_key_map.at(InputKey::BACKWARD);
  bool is_backward = !pressed_key_map.at(InputKey::FORWARD) &&
                     pressed_key_map.at(InputKey::BACKWARD);

  if (is_forward) {
    camera_component.get().radius = std::max(
        min_radius, camera_component.get().radius - zoom_speed * delta_ms);
    camera_component.get().needs_update = true;
  }

  if (is_backward) {
    camera_component.get().radius += zoom_speed * delta_ms;
    camera_component.get().needs_update = true;
  }
}

void transformPaintable(
    float delta_ms, std::reference_wrapper<InputComponent> input_component,
    std::reference_wrapper<CameraComponent> camera_component,
    std::reference_wrapper<TransformComponent> transform_component) {
  const auto& pressed_key_map = input_component.get().pressed_key_map;
  auto& current_rotation = transform_component.get().rotation;

  bool is_up =
      pressed_key_map.at(InputKey::UP) && !pressed_key_map.at(InputKey::DOWN);
  bool is_down =
      !pressed_key_map.at(InputKey::UP) && pressed_key_map.at(InputKey::DOWN);
  bool is_left = pressed_key_map.at(InputKey::LEFT) &&
                 !pressed_key_map.at(InputKey::RIGHT);
  bool is_right = !pressed_key_map.at(InputKey::LEFT) &&
                  pressed_key_map.at(InputKey::RIGHT);

  auto camera_up =
      getUpOnSphere(camera_component.get().phi, camera_component.get().theta);
  auto camera_right = getRightOnSphere(camera_component.get().phi,
                                       camera_component.get().theta);

  if (is_up) {
    auto new_rotation_quat =
        glm::angleAxis(rotation_speed * delta_ms, camera_right);
    current_rotation = new_rotation_quat * current_rotation;
    transform_component.get().needs_update = true;
  }

  if (is_down) {
    auto new_rotation_quat =
        glm::angleAxis(-rotation_speed * delta_ms, camera_right);
    current_rotation = new_rotation_quat * current_rotation;
    transform_component.get().needs_update = true;
  }

  if (is_left) {
    auto new_rotation_quat =
        glm::angleAxis(rotation_speed * delta_ms, camera_up);
    current_rotation = new_rotation_quat * current_rotation;
    transform_component.get().needs_update = true;
  }

  if (is_right) {
    auto new_rotation_quat =
        glm::angleAxis(-rotation_speed * delta_ms, camera_up);
    current_rotation = new_rotation_quat * current_rotation;
    transform_component.get().needs_update = true;
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
