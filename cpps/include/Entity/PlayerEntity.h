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

#include "./Component/BrushComponent.h"
#include "./Component/CameraComponent.h"
#include "./Component/GeometryComponent.h"
#include "./Component/GrFramedTextureComponent.h"
#include "./Component/GrGeometryComponent.h"
#include "./Component/GrUniformComponent.h"

class PlayerEntity {
 public:
  PlayerEntity() {
    camera_component = std::make_unique<CameraComponent>();
    brush_component = std::make_unique<BrushComponent>();

    gr_camera_uniform_component =
        std::make_unique<GrUniformComponent>("CameraBlock");
    gr_brush_uniform_component =
        std::make_unique<GrUniformComponent>("BrushBlock");
    gr_brush_depth_framed_texture_component =
        std::make_unique<GrFramedTextureComponent>(
            TextureType::DEPTH, "u_brushDepthTexture", 1024, 1024);
    brush_quad_geometry_component =
        std::make_unique<GeometryComponent>(GeometryPreset::QUAD);
    gr_brush_quad_geometry_component = std::make_unique<GrGeometryComponent>();
  }

  std::unique_ptr<CameraComponent> camera_component;
  std::unique_ptr<GrUniformComponent> gr_camera_uniform_component;
  std::unique_ptr<BrushComponent> brush_component;
  std::unique_ptr<GrUniformComponent> gr_brush_uniform_component;
  std::unique_ptr<GrFramedTextureComponent>
      gr_brush_depth_framed_texture_component;

  // TODO: Integrate to global gr manager
  std::unique_ptr<GeometryComponent> brush_quad_geometry_component;
  std::unique_ptr<GrGeometryComponent> gr_brush_quad_geometry_component;
};
