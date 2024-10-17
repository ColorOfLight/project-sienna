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

#include "./system/render_system.h"

#include <GLES3/gl3.h>  // OpenGL ES 3.0 for WebGL 2.0
#include <emscripten/html5.h>

namespace render_system {

void initContext() {
  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes(&attr);
  attr.majorVersion = 2;  // WebGL 2.0
  attr.minorVersion = 0;

  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context =
      emscripten_webgl_create_context("#canvas", &attr);
  if (!context) {
    throw std::runtime_error("Failed to create WebGL context!");
  }

  emscripten_webgl_make_context_current(context);

  glEnable(GL_DEPTH_TEST);

  // TODO: replace with actual clear color
  glClearColor(0.1, 0.1, 0.1, 1);
}

void adjustViewportSize(
    std::reference_wrapper<InputComponent> input_component,
    std::reference_wrapper<EventComponent> event_component,
    std::reference_wrapper<CameraComponent> camera_component) {
  if (event_component.get().change_canvas_size) {
    int canvas_width = input_component.get().canvas_size.width;
    int canvas_height = input_component.get().canvas_size.height;

    emscripten_set_canvas_element_size("#canvas", canvas_width, canvas_height);
    glViewport(0, 0, canvas_width, canvas_height);

    camera_component.get().needs_update = true;

    event_component.get().change_canvas_size = false;
  }
}

void render(std::reference_wrapper<InputComponent> input_component,
            std::reference_wrapper<MaterialComponent> material_component,
            std::reference_wrapper<GrShaderManagerComponent>
                gr_shader_manager_component,
            const std::vector<RenderItem>& render_items) {
  auto shader_type = material_component.get().shader_type;
  GLuint shader_program_id =
      gr_shader_manager_component.get().getShaderProgramId(shader_type);

  glViewport(0, 0, input_component.get().canvas_size.width,
             input_component.get().canvas_size.height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shader_program_id);

  for (const auto& render_item : render_items) {
    const auto& gr_geometry_component = render_item.gr_geometry_component;
    const auto& gr_uniform_components = render_item.gr_uniform_components;
    const auto& gr_texture_components = render_item.gr_texture_components;

    glBindVertexArray(gr_geometry_component.get().vao_id);

    int uniform_binging_point = 0;
    for (const auto& gr_uniform_component : gr_uniform_components) {
      glBindBufferBase(GL_UNIFORM_BUFFER, uniform_binging_point,
                       gr_uniform_component.get().uniform_buffer_id);
      unsigned int block_index = glGetUniformBlockIndex(
          shader_program_id,
          gr_uniform_component.get().uniform_block_name.c_str());
      glUniformBlockBinding(shader_program_id, block_index,
                            uniform_binging_point);
      uniform_binging_point++;
    }

    int texture_unit = 0;
    for (const auto& gr_texture_component : gr_texture_components) {
      glUniform1i(glGetUniformLocation(shader_program_id,
                                       gr_texture_component.get().name.c_str()),
                  texture_unit);
      glActiveTexture(GL_TEXTURE0 + texture_unit);
      glBindTexture(GL_TEXTURE_2D, gr_texture_component.get().texture_id);
      texture_unit++;
    }

    glDrawElements(GL_TRIANGLES, gr_geometry_component.get().vertex_count,
                   GL_UNSIGNED_INT, 0);
  }
}

}  // namespace render_system
