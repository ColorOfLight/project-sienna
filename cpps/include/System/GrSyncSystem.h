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

#include "./Component/CameraComponent.h"
#include "./Component/DirtMapComponent.h"
#include "./Component/GeometryComponent.h"
#include "./Component/GrGeometryComponent.h"
#include "./Component/GrMaterialComponent.h"
#include "./Component/GrTextureComponent.h"
#include "./Component/GrUniformComponent.h"
#include "./Component/MaterialComponent.h"
#include "./Component/TransformComponent.h"

class GrSyncSystem {
 public:
  static void updateGeometry(
      std::reference_wrapper<const GeometryComponent> geometry_component,
      std::reference_wrapper<GrGeometryComponent> gr_geometry_component);

  static void updateMaterial(
      std::reference_wrapper<const MaterialComponent> material_component,
      std::reference_wrapper<GrMaterialComponent> gr_material_component);

  static void updateCameraUniform(
      std::reference_wrapper<const CameraComponent> camera_component,
      std::reference_wrapper<GrUniformComponent> gr_uniform_component) {
    // TODO: implement in source file
  }

  static void updateTransformUniform(
      std::reference_wrapper<const TransformComponent> transform_component,
      std::reference_wrapper<GrUniformComponent> gr_uniform_component) {
    // TODO: implement in source file
  }

  static void updateDirtTexture(
      std::reference_wrapper<const DirtMapComponent> dirt_map_component,
      std::reference_wrapper<GrTextureComponent> gr_texture_component) {
    // TODO: implement in source file
  }
};
