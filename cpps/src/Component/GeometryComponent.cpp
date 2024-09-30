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

#include "./Component/GeometryComponent.h"

#include <glm/glm.hpp>
#include <vector>

std::vector<Vertex> generatePlaneVertices(const glm::vec3& right,
                                          const glm::vec3& up, float half_width,
                                          float half_height, float half_depth,
                                          int width_segments,
                                          int height_segments);
std::vector<unsigned int> generatePlaneIndices(int right_segments,
                                               int up_segments, int offset = 0);

GeometryComponent::GeometryComponent(GeometryPreset preset) {
  if (preset == GeometryPreset::PLANE) {
    vertices = generatePlaneVertices(glm::vec3(1.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f), 0.5f, 0.5f,
                                     0.0f, 1, 1);
    indices = generatePlaneIndices(1, 1);
  } else {
    throw std::invalid_argument("Invalid geometry preset");
  }
}

std::vector<Vertex> generatePlaneVertices(const glm::vec3& right,
                                          const glm::vec3& up, float half_width,
                                          float half_height, float half_depth,
                                          int width_segments,
                                          int height_segments) {
  int vertex_count = (width_segments + 1) * (height_segments + 1);
  std::vector<Vertex> vertices(vertex_count);

  glm::vec3 front =
      glm::cross(right, up);  // Calculate the front direction (normal)

  for (int j = 0; j <= height_segments; ++j) {
    for (int i = 0; i <= width_segments; ++i) {
      float xPos =
          half_width * 2.0f * (static_cast<float>(i) / width_segments - 0.5f);
      float yPos =
          half_height * 2.0f * (static_cast<float>(j) / height_segments - 0.5f);

      glm::vec3 position = right * xPos + up * yPos + front * half_depth;

      Vertex vertex = {
          .position = position,
          .normal = front,
          .tex_coords = glm::vec2(static_cast<float>(i) / width_segments,
                                  static_cast<float>(j) / height_segments),
      };

      vertices[j * (width_segments + 1) + i] = vertex;
    }
  }

  return vertices;
}

std::vector<unsigned int> generatePlaneIndices(int right_segments,
                                               int up_segments, int offset) {
  int index_count = right_segments * up_segments *
                    6;  // Each quad has 6 indices (2 triangles)
  std::vector<unsigned int> indices(index_count);

  int index = 0;
  for (int j = 0; j < up_segments; ++j) {
    for (int i = 0; i < right_segments; ++i) {
      int left_bottom_index = j * (right_segments + 1) + i;
      int right_bottom_index = j * (right_segments + 1) + i + 1;
      int left_top_index = (j + 1) * (right_segments + 1) + i;
      int right_top_index = (j + 1) * (right_segments + 1) + i + 1;

      // First triangle (left_bottom_index, right_bottom_index, left_top_index)
      indices[index++] = left_bottom_index + offset;
      indices[index++] = right_bottom_index + offset;
      indices[index++] = left_top_index + offset;

      // Second triangle (right_bottom_index, right_top_index, left_top_index)
      indices[index++] = right_bottom_index + offset;
      indices[index++] = right_top_index + offset;
      indices[index++] = left_top_index + offset;
    }
  }

  return indices;
}
