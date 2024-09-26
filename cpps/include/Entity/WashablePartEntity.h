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

#include "./Component/CleanMarkComponent.h"
#include "./Component/DirtMapComponent.h"
#include "./Component/GrGeometryComponent.h"
#include "./Component/GrTextureComponent.h"
#include "./Component/GrUniformComponent.h"
#include "./Component/TransformComponent.h"

class WashablePartEntity {
 public:
  WashablePartEntity() {
    clean_mark_component = std::make_unique<CleanMarkComponent>();
    dirt_map_component = std::make_unique<DirtMapComponent>();
    gr_geometry_component = std::make_unique<GrGeometryComponent>();
    gr_texture_component = std::make_unique<GrTextureComponent>();
    gr_uniform_component = std::make_unique<GrUniformComponent>();
    transform_component = std::make_unique<TransformComponent>();
  }

  std::unique_ptr<CleanMarkComponent> clean_mark_component;
  std::unique_ptr<DirtMapComponent> dirt_map_component;
  std::unique_ptr<GrGeometryComponent> gr_geometry_component;
  std::unique_ptr<GrTextureComponent> gr_texture_component;
  std::unique_ptr<GrUniformComponent> gr_uniform_component;
  std::unique_ptr<TransformComponent> transform_component;
};