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

#include "./Component/GrGeometryComponent.h"
#include "./Component/GrPingPongTextureComponent.h"
#include "./Component/GrTextureComponent.h"
#include "./Component/GrUniformComponent.h"
#include "./Entity/CameraEntity.h"
#include "./Entity/PaintableEntity.h"

struct RenderItem {
  std::reference_wrapper<GrGeometryComponent> gr_geometry_component;
  std::vector<std::reference_wrapper<GrUniformComponent>> gr_uniform_components;
  std::vector<std::reference_wrapper<GrTextureComponent>> gr_texture_components;
  std::vector<std::reference_wrapper<GrPingPongTextureComponent>>
      gr_ping_pong_texture_components;
};

class RenderItemsView {
 public:
  RenderItemsView(std::reference_wrapper<PaintableEntity> paintable_entity,
                  std::reference_wrapper<CameraEntity> camera_entity) {
    render_items = {};

    for (const auto& paintable_part :
         paintable_entity.get().paintable_part_entities) {
      render_items.push_back({
          .gr_geometry_component =
              std::ref(*paintable_part->gr_geometry_component),
          .gr_uniform_components =
              std::vector<std::reference_wrapper<GrUniformComponent>>({
                  std::ref(*camera_entity.get().gr_camera_uniform_component),
                  std::ref(*paintable_part->gr_transform_uniform_component),
              }),
          .gr_texture_components = {},
          .gr_ping_pong_texture_components =
              std::vector<std::reference_wrapper<GrPingPongTextureComponent>>({
                  std::ref(
                      *paintable_part->gr_painted_ping_pong_texture_component),
              }),
      });
    }
  }

  std::vector<RenderItem> render_items;
};
