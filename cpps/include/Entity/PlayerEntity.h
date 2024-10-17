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
#include "./Component/CleanerComponent.h"
#include "./Component/GrShaderComponent.h"
#include "./Component/GrUniformComponent.h"

class PlayerEntity {
 public:
  PlayerEntity() {
    camera_component = std::make_unique<CameraComponent>();
    cleaner_component = std::make_unique<CleanerComponent>();
    brush_component = std::make_unique<BrushComponent>();

    gr_camera_uniform_component =
        std::make_unique<GrUniformComponent>("CameraBlock");
    gr_decal_shader_component = std::make_unique<GrShaderComponent>();
    gr_brush_uniform_component =
        std::make_unique<GrUniformComponent>("BrushBlock");
  }

  std::unique_ptr<CameraComponent> camera_component;
  std::unique_ptr<GrUniformComponent> gr_camera_uniform_component;
  std::unique_ptr<BrushComponent> brush_component;
  std::unique_ptr<GrShaderComponent> gr_decal_shader_component;
  std::unique_ptr<GrUniformComponent> gr_brush_uniform_component;

  // TODO: remove later
  std::unique_ptr<CleanerComponent> cleaner_component;
};
