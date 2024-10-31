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

#include "./system/manage_system.h"

#include <GLES3/gl3.h>

namespace manage_system {

void resetPainted(
    std::reference_wrapper<EventComponent> event_component,
    std::reference_wrapper<RenderConfigComponent> render_config_component,
    std::reference_wrapper<PaintedTexturesView> painted_textures_view) {
  for (const auto& gr_painted_component :
       painted_textures_view.get().paintable_gr_ping_pong_textures) {
    const auto& gr_painted_texture =
        gr_painted_component.get().getCurrentFramedTexture();

    glBindFramebuffer(GL_FRAMEBUFFER, gr_painted_texture.get().framebuffer_id);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(render_config_component.get().clear_color.r,
                 render_config_component.get().clear_color.g,
                 render_config_component.get().clear_color.b, 1.0f);
  }

  event_component.get().reset_paint = std::nullopt;
}

void resetModel(std::reference_wrapper<EventComponent> event_component,
                std::reference_wrapper<RootManager> root_manager) {
  auto model_preset = event_component.get().update_model.value();

  switch (model_preset) {
    case ModelOptions::CUBE:
      root_manager.get().resetPaintable(PaintablePreset::CUBE);
      break;
    case ModelOptions::PLANE:
      root_manager.get().resetPaintable(PaintablePreset::PLANE);
      break;
    case ModelOptions::SPHERE:
      root_manager.get().resetPaintable(PaintablePreset::SPHERE);
      break;
    default:
      std::runtime_error("Invalid model preset");
  }

  event_component.get().update_model = std::nullopt;
}

void resetPosition(
    std::reference_wrapper<EventComponent> event_component,
    std::reference_wrapper<CameraComponent> camera_component,
    std::reference_wrapper<TransformComponent> transform_component) {
  camera_component.get().reset();
  transform_component.get().reset();

  event_component.get().reset_position = std::nullopt;
}

}  // namespace manage_system
