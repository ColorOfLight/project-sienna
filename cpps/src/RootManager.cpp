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

#include "./RootManager.h"

RootManager::RootManager() {
  config_entity = std::make_unique<ConfigEntity>();
  client_input_entity = std::make_unique<ClientInputEntity>();
  gr_global_entity = std::make_unique<GrGlobalEntity>();
  camera_entity = std::make_unique<CameraEntity>();
  brush_entity = std::make_unique<BrushEntity>();
  paintable_entity = std::make_unique<PaintableEntity>(PaintablePreset::CUBE);

  render_items_view = std::make_unique<RenderItemsView>(
      std::ref(*paintable_entity), std::ref(*camera_entity));
  painted_textures_view =
      std::make_unique<PaintedTexturesView>(std::ref(*paintable_entity));
  transform_updating_view =
      std::make_unique<TransformUpdatingView>(std::ref(*paintable_entity));
  gr_model_geometries_view =
      std::make_unique<GrModelGeometriesView>(std::ref(*paintable_entity));

  for (const auto& paintable_part : paintable_entity->paintable_part_entities) {
    paintable_gr_geometries.push_back(
        std::ref(*paintable_part->gr_geometry_component));
    paintable_transforms.push_back(
        std::ref(*paintable_part->transform_component));
    paintable_gr_transform_uniforms.push_back(
        std::ref(*paintable_part->gr_transform_uniform_component));
    paintable_gr_ping_pong_textures.push_back(
        std::ref(*paintable_part->gr_painted_ping_pong_texture_component));
  }
}
