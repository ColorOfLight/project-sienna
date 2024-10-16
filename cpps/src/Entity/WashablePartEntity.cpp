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

#include "./Entity/WashablePartEntity.h"

WashablePartEntity::WashablePartEntity(WashablePartPreset preset,
                                       glm::vec3 scale, glm::quat rotation,
                                       glm::vec3 translation) {
  int painted_map_width = 200;
  int painted_map_height = 200;

  clean_mark_component = std::make_unique<CleanMarkComponent>();

  gr_geometry_component = std::make_unique<GrGeometryComponent>();

  gr_painted_texture_component = std::make_unique<GrTextureComponent>(
      TextureType::RGBA, "u_paintedMapTexture", painted_map_width,
      painted_map_height);

  gr_painted_framebuffer_component = std::make_unique<GrFramebufferComponent>(
      gr_painted_texture_component->texture_id);

  gr_transform_uniform_component =
      std::make_unique<GrUniformComponent>("ModelBlock");

  transform_component =
      std::make_unique<TransformComponent>(scale, rotation, translation);

  if (preset == WashablePartPreset::CUBE_PART) {
    geometry_component =
        std::make_unique<GeometryComponent>(GeometryPreset::PLANE);
  } else {
    throw std::invalid_argument("Invalid washable part preset");
  }

  // TODO: remove the belows later
  int dirt_map_width = 200;
  int dirt_map_height = 200;
  dirt_map_component =
      std::make_unique<DirtMapComponent>(dirt_map_width, dirt_map_height);
  gr_dirt_map_texture_component = std::make_unique<GrTextureComponent>(
      TextureType::R8, "u_dirtMapTexture", dirt_map_width, dirt_map_height);
}
