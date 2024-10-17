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

#include "./system/clean_system.h"

#include "./math_util.h"

namespace clean_system {

void markToClean(
    std::reference_wrapper<InputComponent> input_component,
    std::reference_wrapper<CameraComponent> camera_component,
    std::reference_wrapper<CleanerComponent> cleaner_component,
    std::reference_wrapper<TransformComponent> parent_transform_component,
    const std::vector<std::reference_wrapper<GeometryComponent>>&
        geometry_components,
    const std::vector<std::reference_wrapper<TransformComponent>>&
        child_transform_components,
    const std::vector<std::reference_wrapper<CleanMarkComponent>>&
        clean_mark_components) {
  // 1. Get Ray direction
  const auto& canvas_size = input_component.get().canvas_size;
  const auto& pointer_position = input_component.get().pointer_position;

  auto eye_position = getPositionOnSphere(camera_component.get().radius,
                                          camera_component.get().phi,
                                          camera_component.get().theta);

  auto camera_up =
      getUpOnSphere(camera_component.get().phi, camera_component.get().theta);

  auto view_matrix = glm::lookAt(eye_position, glm::vec3(0.0f), camera_up);

  glm::vec3 ray_direction_in_world = getRayDirectionFromScreen(
      glm::vec2(pointer_position.x, pointer_position.y),
      glm::vec2(canvas_size.width, canvas_size.height),
      camera_component.get().fovy, view_matrix);

  // 2. Get Ray origin
  glm::vec3 ray_origin = eye_position;

  // 3. Get shortest distance index from each geometry
  int geometry_count = geometry_components.size();
  float shortest_distance = std::numeric_limits<float>::infinity();
  int shortest_distance_index = -1;
  glm::vec2 texture_coords;

  const auto& parent_transform_matrix =
      getTransformMatrix(parent_transform_component.get().scale,
                         parent_transform_component.get().rotation,
                         parent_transform_component.get().translation);

  for (int i = 0; i < geometry_count; i++) {
    const auto& transform_matrix =
        parent_transform_matrix *
        getTransformMatrix(child_transform_components[i].get().scale,
                           child_transform_components[i].get().rotation,
                           child_transform_components[i].get().translation);

    const auto& vertexes = geometry_components[i].get().vertices;
    const auto& indexes = geometry_components[i].get().indices;

    for (int j = 0; j < indexes.size(); j += 3) {
      auto intersectionResult = getRayIntersectionDistance(
          ray_origin, ray_direction_in_world,
          glm::vec3(transform_matrix *
                    glm::vec4(vertexes[indexes[j]].position, 1.0f)),
          glm::vec3(transform_matrix *
                    glm::vec4(vertexes[indexes[j + 1]].position, 1.0f)),
          glm::vec3(transform_matrix *
                    glm::vec4(vertexes[indexes[j + 2]].position, 1.0f)));

      if (intersectionResult.has_value() &&
          intersectionResult.value().distance < shortest_distance) {
        shortest_distance = intersectionResult.value().distance;
        shortest_distance_index = i;

        auto u = intersectionResult.value().u;
        auto v = intersectionResult.value().v;
        texture_coords = (1 - u - v) * vertexes[indexes[j]].tex_coords +
                         u * vertexes[indexes[j + 1]].tex_coords +
                         v * vertexes[indexes[j + 2]].tex_coords;
      }
    }
  }

  if (shortest_distance_index == -1) {
    return;
  }

  // 4. Mark the shortest distance index
  clean_mark_components[shortest_distance_index].get().clean_marks.push_back(
      {texture_coords, cleaner_component.get().hitting_radius});
}

void clean(std::reference_wrapper<CleanMarkComponent> clean_mark_component,
           std::reference_wrapper<DirtMapComponent> dirt_map_component) {
  auto& clean_marks = clean_mark_component.get().clean_marks;

  if (clean_marks.empty()) {
    return;
  }

  for (const auto& clean_mark : clean_marks) {
    auto texture_width = dirt_map_component.get().width;
    auto texture_height = dirt_map_component.get().height;
    auto& dirt_map = dirt_map_component.get().dirt_map;

    for (int y = 0; y < texture_height; y++) {
      for (int x = 0; x < texture_width; x++) {
        auto dx = (static_cast<float>(x) / texture_width) -
                  clean_mark.texture_coords.x;
        auto dy = (static_cast<float>(y) / texture_height) -
                  clean_mark.texture_coords.y;

        if (dx * dx + dy * dy < clean_mark.radius * clean_mark.radius) {
          auto& target_dirt = dirt_map[y * texture_width + x];
          target_dirt = target_dirt > 4 ? target_dirt - 4 : 0;
        }
      }
    }

    dirt_map_component.get().needs_update = true;
  }

  clean_marks.clear();
}

}  // namespace clean_system