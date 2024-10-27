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
#include "./Entity/ClientInputEntity.h"
#include "./Entity/ConfigEntity.h"
#include "./Entity/GrGlobalEntity.h"
#include "./Entity/PaintableEntity.h"
#include "./View/GrModelGeometriesView.h"
#include "./View/PaintedTexturesView.h"
#include "./View/RenderItemsView.h"
#include "./View/TransformUpdatingView.h"
#include "./system/render_system.h"

class RootManager {
 public:
  RootManager();

  std::unique_ptr<ConfigEntity> config_entity;
  std::unique_ptr<ClientInputEntity> client_input_entity;
  std::unique_ptr<GrGlobalEntity> gr_global_entity;
  std::unique_ptr<CameraEntity> camera_entity;
  std::unique_ptr<BrushEntity> brush_entity;
  std::unique_ptr<PaintableEntity> paintable_entity;
  std::unique_ptr<RenderItemsView> render_items_view;
  std::unique_ptr<PaintedTexturesView> painted_textures_view;
  std::unique_ptr<TransformUpdatingView> transform_updating_view;
  std::unique_ptr<GrModelGeometriesView> gr_model_geometries_view;
};
