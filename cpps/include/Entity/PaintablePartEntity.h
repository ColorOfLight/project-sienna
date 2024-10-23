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

#include "./Component/GeometryComponent.h"
#include "./Component/GrFramedTextureComponent.h"
#include "./Component/GrGeometryComponent.h"
#include "./Component/GrPingPongTextureComponent.h"
#include "./Component/GrUniformComponent.h"
#include "./Component/TransformComponent.h"

enum class PaintablePartPreset { CUBE_PART };

class PaintablePartEntity {
 public:
  PaintablePartEntity(PaintablePartPreset preset, glm::vec3 scale,
                      glm::quat rotation, glm::vec3 translation);

  std::unique_ptr<GeometryComponent> geometry_component;
  std::unique_ptr<GrGeometryComponent> gr_geometry_component;
  std::unique_ptr<GrUniformComponent> gr_transform_uniform_component;
  std::unique_ptr<TransformComponent> transform_component;
  std::unique_ptr<GrFramedTextureComponent> gr_painted_framed_texture_component;

  std::unique_ptr<GrFramedTextureComponent> gr_paint_framed_texture_component;
  std::unique_ptr<GrPingPongTextureComponent>
      gr_painted_ping_pong_texture_component;
};
