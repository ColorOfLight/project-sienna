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
#include <glm/gtc/constants.hpp>

class CameraComponent {
 public:
  CameraComponent() {
    radius = 3.0f;
    phi = glm::pi<float>() / 3.0f;
    theta = glm::pi<float>() / 4.0f;

    fovy = glm::radians(55.0f);
    needs_update = true;
  }

  CameraComponent(float fovy) {
    // TODO: integrate with the default constructor
    radius = 3.0f;
    phi = glm::pi<float>() / 3.0f;
    theta = glm::pi<float>() / 4.0f;

    this->fovy = fovy;
    needs_update = true;
  }

  float radius;
  float phi;    // Angle in radians from the positive Y-axis to positive Z-axis
  float theta;  // Angle in radians from the positive Z-axis to positive X-axis

  float fovy;
  bool needs_update;
};
