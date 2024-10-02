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

#include "./System/GrSyncSystem.h"

#include <GLES3/gl3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "./shader/utils.h"

void GrSyncSystem::updateGeometry(
    std::reference_wrapper<const GeometryComponent> geometry_component,
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

void GrSyncSystem::updateMaterial(
    std::reference_wrapper<const MaterialComponent> material_component,
    std::reference_wrapper<GrMaterialComponent> gr_material_component) {
  int success;
  char info_log[512];

  auto material_type = material_component.get().material_type;

  const char* vertex_shader_source = getVertexShaderSource(material_type);
  const char* fragment_shader_source = getFragmentShaderSource(material_type);

  // Compile the vertex shader
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader_id, 1, &vertex_shader_source, nullptr);
  glCompileShader(vertex_shader_id);

  // Check for vertex shader compile errors
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader_id, 512, nullptr, info_log);
    throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" +
                             std::string(info_log));
  }

  // Compile the fragment shader
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader_id, 1, &fragment_shader_source, nullptr);
  glCompileShader(fragment_shader_id);

  // Check for fragment shader compile errors
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader_id, 512, nullptr, info_log);
    throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" +
                             std::string(info_log));
  }

  // Link shaders to create a shader program
  GLuint shader_program_id = glCreateProgram();
  glAttachShader(shader_program_id, vertex_shader_id);
  glAttachShader(shader_program_id, fragment_shader_id);
  glLinkProgram(shader_program_id);

  // Check for linking errors
  glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program_id, 512, nullptr, info_log);
    throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" +
                             std::string(info_log));
  }

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  gr_material_component.get().shader_program_id = shader_program_id;
}

void GrSyncSystem::updateTransformUniform(
    std::reference_wrapper<TransformComponent> transform_component,
    std::reference_wrapper<GrUniformComponent> gr_uniform_component) {
  if (!transform_component.get().needs_update) {
    return;
  }

  const auto& scale = transform_component.get().scale;
  const auto& rotation = transform_component.get().rotation;
  const auto& translation = transform_component.get().translation;

  // Order matters: scale -> rotate -> translate
  // Due to right multiplication of matrices
  glm::mat4 model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, translation);
  model_matrix = model_matrix * glm::mat4_cast(rotation);
  model_matrix = glm::scale(model_matrix, scale);

  glBindBuffer(GL_UNIFORM_BUFFER, gr_uniform_component.get().uniform_buffer_id);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(model_matrix), &model_matrix,
               GL_STATIC_DRAW);

  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  transform_component.get().needs_update = false;
}

void GrSyncSystem::updateCameraUniform(
    std::reference_wrapper<const InputComponent> input_component,
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

  const auto& front = camera_component.get().front;
  const auto& up = camera_component.get().up;
  const auto& position = camera_component.get().position;
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
