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

#include "./Entity/GameEntity.h"
#include "./Entity/PlayerEntity.h"
#include "./Entity/WashableEntity.h"
#include "./system/client_sync_system.h"
#include "./system/gr_sync_system.h"
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

  auto game_entity = std::make_unique<GameEntity>();
  auto player_entity = std::make_unique<PlayerEntity>();
  auto washable_entity = std::make_unique<WashableEntity>(WashablePreset::CUBE);

  auto washable_geometries =
      std::vector<std::reference_wrapper<GeometryComponent>>();
  auto washable_transforms =
      std::vector<std::reference_wrapper<TransformComponent>>();
  auto washable_clean_marks =
      std::vector<std::reference_wrapper<CleanMarkComponent>>();
  auto washable_dirt_maps =
      std::vector<std::reference_wrapper<DirtMapComponent>>();
  auto washable_gr_transform_uniforms =
      std::vector<std::reference_wrapper<GrUniformComponent>>();

  for (const auto& washable_part : washable_entity->washable_part_entities) {
    washable_geometries.push_back(std::ref(*washable_part->geometry_component));
    washable_transforms.push_back(
        std::ref(*washable_part->transform_component));
    washable_clean_marks.push_back(
        std::ref(*washable_part->clean_mark_component));
    washable_dirt_maps.push_back(std::ref(*washable_part->dirt_map_component));
    washable_gr_transform_uniforms.push_back(
        std::ref(*washable_part->gr_transform_uniform_component));
  }

  gr_sync_system::updateMaterial(
      std::ref(*washable_entity->material_component),
      std::ref(*washable_entity->gr_shader_component));

  gr_sync_system::updateDecalShader(
      std::ref(*player_entity->gr_decal_shader_component));

  auto render_items = std::vector<RenderItem>();
  for (const auto& washable_part : washable_entity->washable_part_entities) {
    gr_sync_system::updateGeometry(
        std::ref(*washable_part->geometry_component),
        std::ref(*washable_part->gr_geometry_component));

    render_items.push_back({
        .gr_geometry_component =
            std::ref(*washable_part->gr_geometry_component),
        .gr_uniform_components =
            std::vector<std::reference_wrapper<GrUniformComponent>>({
                std::ref(*player_entity->gr_camera_uniform_component),
                std::ref(*washable_part->gr_transform_uniform_component),
            }),
        .gr_texture_components =
            std::vector<std::reference_wrapper<GrTextureComponent>>({
                std::ref(*washable_part->gr_painted_texture_component),
            }),
    });
  }

  auto main_loop = [game_entity = std::ref(*game_entity),
                    player_entity = std::ref(*player_entity),
                    washable_entity = std::ref(*washable_entity), render_items,
                    washable_geometries, washable_transforms,
                    washable_clean_marks, washable_dirt_maps,
                    washable_gr_transform_uniforms](float elapsed_ms,
                                                    float delta_ms) {
    client_sync_system::syncInput(std::ref(*game_entity.get().input_component));
    client_sync_system::consumeEvent(
        std::ref(*game_entity.get().event_component));

    render_system::adjustViewportSize(
        std::ref(*game_entity.get().input_component),
        std::ref(*game_entity.get().event_component),
        std::ref(*player_entity.get().camera_component));

    transform_system::transformCamera(
        delta_ms, std::ref(*game_entity.get().input_component),
        std::ref(*player_entity.get().camera_component));
    transform_system::transformWashable(
        delta_ms, std::ref(*game_entity.get().input_component),
        std::ref(*washable_entity.get().transform_component));

    gr_sync_system::updateCameraUniform(
        std::ref(*game_entity.get().input_component),
        std::ref(*player_entity.get().camera_component),
        std::ref(*player_entity.get().gr_camera_uniform_component));

    if (game_entity.get().input_component->is_pointer_down) {
      gr_sync_system::updateBrushUniform(
          std::ref(*player_entity.get().brush_component),
          std::ref(*game_entity.get().input_component),
          std::ref(*player_entity.get().camera_component),
          std::ref(*player_entity.get().gr_brush_uniform_component));

      for (auto& washable_part : washable_entity.get().washable_part_entities) {
        paint_system::paint(
            std::ref(*washable_part->gr_geometry_component),
            std::ref(*player_entity.get().gr_decal_shader_component),
            std::ref(*player_entity.get().gr_brush_uniform_component),
            std::ref(*washable_part->gr_transform_uniform_component),
            std::ref(*washable_part->gr_painted_texture_component),
            std::ref(*washable_part->gr_painted_framebuffer_component));
      }
    }

    if (game_entity.get().event_component->reset) {
      manage_system::resetDirtMap(std::ref(*game_entity.get().event_component),
                                  washable_dirt_maps);
    }

    gr_sync_system::updateTransformUniforms(
        std::ref(*washable_entity.get().transform_component),
        washable_transforms, washable_gr_transform_uniforms);

    render_system::render(std::ref(*game_entity.get().input_component),
                          std::ref(*washable_entity.get().gr_shader_component),
                          render_items);
  };

  static_main_loop = main_loop;

  emscripten_set_main_loop(renderFrame, 0, 1);

  return 0;
}
