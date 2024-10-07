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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISTD::SING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include <cmath>
#include <glm/glm.hpp>

inline glm::vec3 getPositionOnSphere(float radius, float phi, float theta) {
  return glm::vec3(radius * std::sin(phi) * std::sin(theta),
                   radius * std::cos(phi),
                   radius * std::sin(phi) * std::cos(theta));
}

inline glm::vec3 getUpOnSphere(float phi, float theta) {
  return glm::vec3(-std::cos(phi) * std::sin(theta), std::sin(phi),
                   -std::cos(phi) * std::cos(theta));
}

inline glm::mat4x4 getTransformMatrix(const glm::vec3& scale,
                                      const glm::quat& rotation,
                                      const glm::vec3& translation) {
  // Order matters: scale -> rotate -> translate
  // Due to right multiplication of matrices
  glm::mat4 model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, translation);
  model_matrix = model_matrix * glm::mat4_cast(rotation);
  model_matrix = glm::scale(model_matrix, scale);

  return model_matrix;
}
