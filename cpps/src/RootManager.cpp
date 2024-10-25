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
  game_entity = std::make_unique<GameEntity>();
  player_entity = std::make_unique<PlayerEntity>();
  paintable_entity = std::make_unique<PaintableEntity>(PaintablePreset::CUBE);

  for (const auto& paintable_part : paintable_entity->paintable_part_entities) {
    paintable_geometries.push_back(
        std::ref(*paintable_part->geometry_component));
    paintable_gr_geometries.push_back(
        std::ref(*paintable_part->gr_geometry_component));
    paintable_transforms.push_back(
        std::ref(*paintable_part->transform_component));
    paintable_gr_transform_uniforms.push_back(
        std::ref(*paintable_part->gr_transform_uniform_component));
    paintable_gr_ping_pong_textures.push_back(
        std::ref(*paintable_part->gr_painted_ping_pong_texture_component));
  }

  for (const auto& paintable_part : paintable_entity->paintable_part_entities) {
    render_items.push_back({
        .gr_geometry_component =
            std::ref(*paintable_part->gr_geometry_component),
        .gr_uniform_components =
            std::vector<std::reference_wrapper<GrUniformComponent>>({
                std::ref(*player_entity->gr_camera_uniform_component),
                std::ref(*paintable_part->gr_transform_uniform_component),
            }),
        .gr_ping_pong_texture_components =
            std::vector<std::reference_wrapper<GrPingPongTextureComponent>>({
                std::ref(
                    *paintable_part->gr_painted_ping_pong_texture_component),
            }),
    });
  }
}