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

#include "./render_util.h"

#include <GLES3/gl3.h>

void drawGrComponents(
    ShaderType shader_type,
    std::reference_wrapper<GrShaderManagerComponent>
        gr_shader_manager_component,
    std::reference_wrapper<GrGeometryComponent> gr_geometry_component,
    const std::vector<std::reference_wrapper<GrUniformComponent>>&
        gr_uniform_components,
    const std::vector<std::reference_wrapper<GrTextureComponent>>&
        gr_texture_components) {
  GLuint shader_program_id =
      gr_shader_manager_component.get().getShaderProgramId(shader_type);

  glUseProgram(shader_program_id);

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
