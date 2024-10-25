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

#include <GLES3/gl3.h>
#include <emscripten.h>

#include <memory>
#include <vector>

#include "./Entity/PaintableEntity.h"
#include "./RootManager.h"
#include "./system/client_sync_system.h"
#include "./system/gr_sync_system.h"
#include "./system/input_sync_system.h"
#include "./system/manage_system.h"
#include "./system/paint_system.h"
#include "./system/render_system.h"
#include "./system/transform_system.h"

static std::function<void(float, float)> static_main_loop;
static double start_time = emscripten_get_now();
static double prev_time = start_time;

void renderFrame() {
  double current_time = emscripten_get_now();
  float elapsed_time = static_cast<float>(current_time - start_time);
  float delta_time = static_cast<float>(current_time - prev_time);

  if (static_main_loop) {
    static_main_loop(elapsed_time, delta_time);
  }

  prev_time = current_time;
}

int main() {
  render_system::initContext();

  auto root_manager = std::make_unique<RootManager>();

  gr_sync_system::updateGeometry(
      GeometryPreset::QUAD,
      std::ref(
          *root_manager.get()->gr_global_entity->gr_quad_geometry_component));

  for (const auto& paintable_part :
       root_manager.get()->paintable_entity->paintable_part_entities) {
    gr_sync_system::updateGeometry(
        std::ref(*paintable_part->geometry_component),
        std::ref(*paintable_part->gr_geometry_component));
  }

  auto main_loop = [root_manager = std::ref(*root_manager)](float elapsed_ms,
                                                            float delta_ms) {
    auto config_entity = std::ref(*root_manager.get().config_entity);
    auto client_input_entity =
        std::ref(*root_manager.get().client_input_entity);
    auto gr_global_entity = std::ref(*root_manager.get().gr_global_entity);
    auto camera_entity = std::ref(*root_manager.get().camera_entity);
    auto brush_entity = std::ref(*root_manager.get().brush_entity);
    auto paintable_entity = std::ref(*root_manager.get().paintable_entity);
    auto& paintable_transforms = root_manager.get().paintable_transforms;
    auto& paintable_gr_transform_uniforms =
        root_manager.get().paintable_gr_transform_uniforms;
    auto& paintable_gr_ping_pong_textures =
        root_manager.get().paintable_gr_ping_pong_textures;
    auto& paintable_gr_geometries = root_manager.get().paintable_gr_geometries;
    auto render_items_view = std::ref(*root_manager.get().render_items_view);

    client_sync_system::syncInput(
        std::ref(*client_input_entity.get().input_component));
    client_sync_system::consumeEvent(
        std::ref(*client_input_entity.get().event_component));

    render_system::adjustViewportSize(
        std::ref(*client_input_entity.get().input_component),
        std::ref(*client_input_entity.get().event_component),
        std::ref(*camera_entity.get().camera_component));

    transform_system::transformCamera(
        delta_ms, std::ref(*client_input_entity.get().input_component),
        std::ref(*camera_entity.get().camera_component));
    transform_system::transformPaintable(
        delta_ms, std::ref(*client_input_entity.get().input_component),
        std::ref(*camera_entity.get().camera_component),
        std::ref(*paintable_entity.get().transform_component));

    gr_sync_system::updateCameraUniform(
        std::ref(*client_input_entity.get().input_component),
        std::ref(*camera_entity.get().camera_component),
        std::ref(*camera_entity.get().gr_camera_uniform_component));
    gr_sync_system::updateTimeUniform(
        elapsed_ms, delta_ms,
        std::ref(*gr_global_entity.get().gr_time_uniform_component));

    if (client_input_entity.get().event_component->reset) {
      manage_system::resetPainted(
          std::ref(*client_input_entity.get().event_component),
          std::ref(*config_entity.get().render_config_component),
          paintable_gr_ping_pong_textures);
    }

    if (client_input_entity.get().input_component->is_pointer_down) {
      input_sync_system::syncBrush(
          std::ref(*client_input_entity.get().input_component),
          std::ref(*brush_entity.get().brush_component));

      gr_sync_system::updateBrushUniform(
          std::ref(*brush_entity.get().brush_component),
          std::ref(*client_input_entity.get().input_component),
          std::ref(*camera_entity.get().camera_component),
          std::ref(*brush_entity.get().gr_brush_uniform_component));

      paint_system::updateBrushDepth(
          std::ref(*gr_global_entity.get().gr_shader_manager_component),
          std::ref(*brush_entity.get().gr_brush_uniform_component),
          paintable_gr_geometries, paintable_gr_transform_uniforms,
          std::ref(
              *brush_entity.get().gr_brush_depth_framed_texture_component));

      for (auto& paintable_part :
           paintable_entity.get().paintable_part_entities) {
        paint_system::paint(
            std::ref(*paintable_part->gr_geometry_component),
            std::ref(*gr_global_entity.get().gr_shader_manager_component),
            std::ref(*brush_entity.get().gr_brush_uniform_component),
            std::ref(*paintable_part->gr_transform_uniform_component),
            std::ref(*gr_global_entity.get().gr_time_uniform_component),
            std::ref(
                *brush_entity.get().gr_brush_depth_framed_texture_component),
            std::ref(*paintable_part->gr_paint_framed_texture_component));
        paint_system::updatePaintedMap(
            std::ref(*gr_global_entity.get().gr_quad_geometry_component),
            std::ref(*gr_global_entity.get().gr_shader_manager_component),
            std::ref(*paintable_part->gr_paint_framed_texture_component),
            std::ref(*paintable_part->gr_painted_ping_pong_texture_component));
      }
    }

    gr_sync_system::updateTransformUniforms(
        std::ref(*paintable_entity.get().transform_component),
        paintable_transforms, paintable_gr_transform_uniforms);

    render_system::render(
        std::ref(*client_input_entity.get().input_component),
        std::ref(*config_entity.get().render_config_component),
        std::ref(*paintable_entity.get().material_component),
        std::ref(*gr_global_entity.get().gr_shader_manager_component),
        render_items_view);
  };

  static_main_loop = main_loop;

  emscripten_set_main_loop(renderFrame, 0, 1);

  return 0;
}
