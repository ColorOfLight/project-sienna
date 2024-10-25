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

#include "./Entity/BrushEntity.h"
#include "./Entity/CameraEntity.h"
#include "./Entity/GameEntity.h"
#include "./Entity/PaintableEntity.h"
#include "./Entity/PlayerEntity.h"
#include "./system/render_system.h"

class RootManager {
 public:
  RootManager();

  std::unique_ptr<GameEntity> game_entity;
  std::unique_ptr<CameraEntity> camera_entity;
  std::unique_ptr<BrushEntity> brush_entity;
  std::unique_ptr<PlayerEntity> player_entity;
  std::unique_ptr<PaintableEntity> paintable_entity;
  std::vector<std::reference_wrapper<GeometryComponent>> paintable_geometries;
  std::vector<std::reference_wrapper<GrGeometryComponent>>
      paintable_gr_geometries;
  std::vector<std::reference_wrapper<TransformComponent>> paintable_transforms;
  std::vector<std::reference_wrapper<GrUniformComponent>>
      paintable_gr_transform_uniforms;
  std::vector<std::reference_wrapper<GrPingPongTextureComponent>>
      paintable_gr_ping_pong_textures;
  std::vector<RenderItem> render_items;
};
