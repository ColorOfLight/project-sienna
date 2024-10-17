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

#include "./system/paint_system.h"

#include <GLES3/gl3.h>

namespace paint_system {

void paint(
    std::reference_wrapper<GrGeometryComponent> gr_geometry_component,
    std::reference_wrapper<GrShaderComponent> gr_shader_component,
    std::reference_wrapper<GrUniformComponent> gr_brush_uniform_component,
    std::reference_wrapper<GrUniformComponent> gr_model_uniform_component,
    std::reference_wrapper<GrTextureComponent> gr_painted_texture_component,
    std::reference_wrapper<GrFramebufferComponent>
        gr_painted_framebuffer_component) {
  auto shader_program_id = gr_shader_component.get().shader_program_id;

  auto vao_id = gr_geometry_component.get().vao_id;
  auto brush_uniform_block_name =
      gr_brush_uniform_component.get().uniform_block_name.c_str();
  auto model_uniform_block_name =
      gr_model_uniform_component.get().uniform_block_name.c_str();
  auto texture_name = gr_painted_texture_component.get().name.c_str();

  glBindFramebuffer(GL_FRAMEBUFFER,
                    gr_painted_framebuffer_component.get().framebuffer_id);
  glViewport(0, 0, gr_painted_texture_component.get().width,
             gr_painted_texture_component.get().height);

  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ONE);

  glUseProgram(shader_program_id);

  glBindVertexArray(vao_id);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0,
                   gr_brush_uniform_component.get().uniform_buffer_id);
  unsigned int brush_uniform_block_index =
      glGetUniformBlockIndex(shader_program_id, brush_uniform_block_name);
  glUniformBlockBinding(shader_program_id, brush_uniform_block_index, 0);

  glBindBufferBase(GL_UNIFORM_BUFFER, 1,
                   gr_model_uniform_component.get().uniform_buffer_id);
  unsigned int model_uniform_block_index =
      glGetUniformBlockIndex(shader_program_id, model_uniform_block_name);
  glUniformBlockBinding(shader_program_id, model_uniform_block_index, 1);

  glDrawElements(GL_TRIANGLES, gr_geometry_component.get().vertex_count,
                 GL_UNSIGNED_INT, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glUseProgram(0);
  glDisable(GL_BLEND);
}

}  // namespace paint_system
