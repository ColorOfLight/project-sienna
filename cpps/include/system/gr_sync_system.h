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
#include "./Component/GrGeometryComponent.h"
#include "./Component/GrTextureComponent.h"
#include "./Component/GrUniformComponent.h"
#include "./Component/InputComponent.h"
#include "./Component/MaterialComponent.h"
#include "./Component/TransformComponent.h"
#include "./View/TransformUpdatingView.h"

namespace gr_sync_system {

void updateGeometry(
    std::reference_wrapper<GeometryComponent> geometry_component,
    std::reference_wrapper<GrGeometryComponent> gr_geometry_component);

void updateGeometry(
    GeometryPreset geometry_preset,
    std::reference_wrapper<GrGeometryComponent> gr_geometry_component);

void updateCameraUniform(
    std::reference_wrapper<InputComponent> input_component,
    std::reference_wrapper<CameraComponent> camera_component,
    std::reference_wrapper<GrUniformComponent> gr_uniform_component);

void updateTransformUniforms(
    std::reference_wrapper<TransformUpdatingView> transform_updating_view);

void updateBrushUniform(
    std::reference_wrapper<BrushComponent> brush_component,
    std::reference_wrapper<InputComponent> input_component,
    std::reference_wrapper<CameraComponent> camera_component,
    std::reference_wrapper<GrUniformComponent> gr_uniform_component);

void updateTimeUniform(
    float elapsed_ms, float delta_ms,
    std::reference_wrapper<GrUniformComponent> gr_uniform_component);

}  // namespace gr_sync_system
