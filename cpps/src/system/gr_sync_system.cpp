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

#include "./system/gr_sync_system.h"

#include <GLES3/gl3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "./math_util.h"
#include "./shader/util.h"

namespace gr_sync_system {

void updateGeometry(
    std::reference_wrapper<GeometryComponent> geometry_component,
    std::reference_wrapper<GrGeometryComponent> gr_geometry_component) {
  auto& vertices = geometry_component.get().vertices;
  auto& indices = geometry_component.get().indices;

  gr_geometry_component.get().vertex_count = indices.size();

  GLuint vao_id = gr_geometry_component.get().vao_id;
  GLuint vbo_id = gr_geometry_component.get().vbo_id;
  GLuint ebo_id = gr_geometry_component.get().ebo_id;

  std::vector<float> raw_vertices = std::vector<float>(vertices.size() * 8);
  for (int i = 0; i < vertices.size(); i++) {
    raw_vertices[i * 8] = vertices[i].position.x;
    raw_vertices[i * 8 + 1] = vertices[i].position.y;
    raw_vertices[i * 8 + 2] = vertices[i].position.z;
    raw_vertices[i * 8 + 3] = vertices[i].normal.x;
    raw_vertices[i * 8 + 4] = vertices[i].normal.y;
    raw_vertices[i * 8 + 5] = vertices[i].normal.z;
    raw_vertices[i * 8 + 6] = vertices[i].tex_coords.x;
    raw_vertices[i * 8 + 7] = vertices[i].tex_coords.y;
  }

  // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(vao_id);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
  glBufferData(GL_ARRAY_BUFFER, raw_vertices.size() * 4, raw_vertices.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(),
               GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(4 * 3));
  glEnableVertexAttribArray(1);

  // Texture Coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(4 * (3 + 3)));
  glEnableVertexAttribArray(2);

  // Unbind buffers safely
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void updateTransformUniforms(
    std::reference_wrapper<TransformComponent> parent_transform_component,
    const std::vector<std::reference_wrapper<TransformComponent>>&
        child_transform_components,
    const std::vector<std::reference_wrapper<GrUniformComponent>>&
        gr_uniform_components) {
  auto parent_needs_update = parent_transform_component.get().needs_update;
  const auto& parent_model_matrix =
      getTransformMatrix(parent_transform_component.get().scale,
                         parent_transform_component.get().rotation,
                         parent_transform_component.get().translation);

  for (int i = 0; i < child_transform_components.size(); i++) {
    auto& child_transform_component = child_transform_components[i];

    if (!parent_transform_component.get().needs_update &&
        !child_transform_component.get().needs_update) {
      continue;
    }

    const auto& model_matrix =
        parent_model_matrix *
        getTransformMatrix(child_transform_component.get().scale,
                           child_transform_component.get().rotation,
                           child_transform_component.get().translation);

    auto& gr_uniform_component = gr_uniform_components[i];

    glBindBuffer(GL_UNIFORM_BUFFER,
                 gr_uniform_component.get().uniform_buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(model_matrix), &model_matrix,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    child_transform_component.get().needs_update = false;
  }
}

void updateCameraUniform(
    std::reference_wrapper<InputComponent> input_component,
    std::reference_wrapper<CameraComponent> camera_component,
    std::reference_wrapper<GrUniformComponent> gr_uniform_component) {
  if (!camera_component.get().needs_update) {
    return;
  }

  struct CameraUniformData {
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    alignas(16) glm::vec3 eye;
  };

  float radius = camera_component.get().radius;
  float phi = camera_component.get().phi;
  float theta = camera_component.get().theta;

  const auto& position = getPositionOnSphere(radius, phi, theta);
  const auto& up = getUpOnSphere(phi, theta);

  const auto& front = glm::normalize(-position);  // target is origin

  const auto fovy = camera_component.get().fovy;
  float aspect_ratio =
      static_cast<float>(input_component.get().canvas_size.width) /
      input_component.get().canvas_size.height;

  CameraUniformData camera_uniform_data = {
      .view_matrix = glm::lookAt(position, position + front, up),
      .projection_matrix = glm::perspective(fovy, aspect_ratio, 0.1f, 100.0f),
      .eye = position,
  };

  glBindBuffer(GL_UNIFORM_BUFFER, gr_uniform_component.get().uniform_buffer_id);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(camera_uniform_data),
               &camera_uniform_data, GL_STATIC_DRAW);

  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  camera_component.get().needs_update = false;
}

void updateBrushUniform(
    std::reference_wrapper<BrushComponent> brush_component,
    std::reference_wrapper<InputComponent> input_component,
    std::reference_wrapper<CameraComponent> camera_component,
    std::reference_wrapper<GrUniformComponent> gr_uniform_component) {
  const auto& pointer_position = input_component.get().pointer_position;
  const auto& canvas_size = input_component.get().canvas_size;

  struct BrushUniformData {
    float air_pressure;
    alignas(16) glm::vec3 paint_color;
    float paint_viscosity;
    float nozzle_fov;
    alignas(16) glm::mat4 view_matrix;
    alignas(16) glm::mat4 projection_matrix;
    alignas(16) glm::vec3 position;
  };

  glm::mat4 projection_matrix =
      glm::perspective(brush_component.get().nozzle_fov, 1.0f, 0.01f, 100.0f);

  auto eye_position = getPositionOnSphere(camera_component.get().radius,
                                          camera_component.get().phi,
                                          camera_component.get().theta);

  auto camera_up =
      getUpOnSphere(camera_component.get().phi, camera_component.get().theta);

  auto camera_view_matrix =
      glm::lookAt(eye_position, glm::vec3(0.0f), camera_up);

  auto ray_direction = getRayDirectionFromScreen(
      glm::vec2(pointer_position.x, pointer_position.y),
      glm::vec2(canvas_size.width, canvas_size.height),
      camera_component.get().fovy, camera_view_matrix);

  auto brush_view_matrix =
      getRayViewMatrix(eye_position, camera_up, ray_direction);

  BrushUniformData brush_uniform_data = {
      .air_pressure = brush_component.get().air_pressure,
      .paint_color = brush_component.get().paint_color,
      .paint_viscosity = brush_component.get().paint_viscosity,
      .nozzle_fov = brush_component.get().nozzle_fov,
      .view_matrix = brush_view_matrix,
      .projection_matrix = projection_matrix,
      .position = eye_position + ray_direction * glm::vec3(0.2),
  };

  glBindBuffer(GL_UNIFORM_BUFFER, gr_uniform_component.get().uniform_buffer_id);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(brush_uniform_data),
               &brush_uniform_data, GL_STATIC_DRAW);

  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

}  // namespace gr_sync_system
