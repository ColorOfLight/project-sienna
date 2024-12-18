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
#include "./Component/EventComponent.h"
#include "./Component/RenderConfigComponent.h"
#include "./Component/TransformComponent.h"
#include "./RootManager.h"
#include "./View/PaintedTexturesView.h"

namespace manage_system {

inline bool isResetPaintTrue(
    std::reference_wrapper<EventComponent> event_component) {
  return event_component.get().reset_paint.has_value();
}

inline bool isResetPositionTrue(
    std::reference_wrapper<EventComponent> event_component) {
  return event_component.get().reset_position.has_value();
}

inline bool isChangeModel(
    std::reference_wrapper<EventComponent> event_component) {
  return event_component.get().update_model.has_value();
}

void resetPainted(
    std::reference_wrapper<EventComponent> event_component,
    std::reference_wrapper<RenderConfigComponent> render_config_component,
    std::reference_wrapper<PaintedTexturesView> painted_textures_view);

void resetModel(std::reference_wrapper<EventComponent> event_component,
                std::reference_wrapper<RootManager> root_manager);

void resetPosition(
    std::reference_wrapper<EventComponent> event_component,
    std::reference_wrapper<CameraComponent> camera_component,
    std::reference_wrapper<TransformComponent> transform_component);

}  // namespace manage_system
