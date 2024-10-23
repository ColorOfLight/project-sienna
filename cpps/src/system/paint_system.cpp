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

#include <vector>

#include "./render_util.h"

namespace paint_system {

void updateBrushDepth(
    std::reference_wrapper<GrShaderManagerComponent>
        gr_shader_manager_component,
    std::reference_wrapper<GrUniformComponent> gr_brush_uniform_component,
    const std::vector<std::reference_wrapper<GrGeometryComponent>>&
        gr_geometry_components,
    const std::vector<std::reference_wrapper<GrUniformComponent>>&
        gr_model_uniform_components,
    std::reference_wrapper<GrFramedTextureComponent>
        gr_brush_depth_framed_texture_component) {
  glBindFramebuffer(
      GL_FRAMEBUFFER,
      gr_brush_depth_framed_texture_component.get().framebuffer_id);
  glViewport(0, 0, gr_brush_depth_framed_texture_component.get().width,
             gr_brush_depth_framed_texture_component.get().height);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < gr_geometry_components.size(); i++) {
    auto gr_uniform_components =
        std::vector<std::reference_wrapper<GrUniformComponent>>{
            gr_brush_uniform_component, gr_model_uniform_components[i]};

    drawGrComponents(ShaderType::BRUSH_DEPTH, gr_shader_manager_component,
                     gr_geometry_components[i], gr_uniform_components, {});
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void paint(
    std::reference_wrapper<GrGeometryComponent> gr_geometry_component,
    std::reference_wrapper<GrShaderManagerComponent>
        gr_shader_manager_component,
    std::reference_wrapper<GrUniformComponent> gr_brush_uniform_component,
    std::reference_wrapper<GrUniformComponent> gr_model_uniform_component,
    std::reference_wrapper<GrUniformComponent> gr_time_uniform_component,
    std::reference_wrapper<GrTextureComponent> gr_brush_depth_texture_component,
    std::reference_wrapper<GrFramedTextureComponent>
        gr_paint_framed_texture_component) {
  auto gr_uniform_components =
      std::vector<std::reference_wrapper<GrUniformComponent>>{
          gr_brush_uniform_component, gr_model_uniform_component,
          gr_time_uniform_component};
  auto gr_texture_components =
      std::vector<std::reference_wrapper<GrTextureComponent>>{
          gr_brush_depth_texture_component};

  glBindFramebuffer(GL_FRAMEBUFFER,
                    gr_paint_framed_texture_component.get().framebuffer_id);
  glViewport(0, 0, gr_paint_framed_texture_component.get().width,
             gr_paint_framed_texture_component.get().height);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glClear(GL_COLOR_BUFFER_BIT);

  drawGrComponents(ShaderType::BRUSH_DECAL, gr_shader_manager_component,
                   gr_geometry_component, gr_uniform_components,
                   gr_texture_components);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void updatePaintedMap(
    std::reference_wrapper<GrGeometryComponent> gr_geometry_component,
    std::reference_wrapper<GrShaderManagerComponent>
        gr_shader_manager_component,
    std::reference_wrapper<GrTextureComponent> gr_paint_texture_component,
    std::reference_wrapper<GrPingPongTextureComponent>
        gr_painted_ping_pong_texture_component) {
  gr_painted_ping_pong_texture_component.get().switchTexture();

  auto prev_framed_texture =
      gr_painted_ping_pong_texture_component.get().getPrevFramedTexture();
  auto current_framed_texture =
      gr_painted_ping_pong_texture_component.get().getCurrentFramedTexture();

  auto gr_texture_components =
      std::vector<std::reference_wrapper<GrTextureComponent>>{
          gr_paint_texture_component, prev_framed_texture};

  glBindFramebuffer(GL_FRAMEBUFFER,
                    current_framed_texture.get().framebuffer_id);
  glViewport(0, 0, current_framed_texture.get().width,
             current_framed_texture.get().height);

  drawGrComponents(ShaderType::PAINT_BLEND, gr_shader_manager_component,
                   gr_geometry_component, {}, gr_texture_components);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}  // namespace paint_system
