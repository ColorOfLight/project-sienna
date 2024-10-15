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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISTD::SING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <optional>

inline glm::vec3 getPositionOnSphere(float radius, float phi, float theta) {
  return glm::vec3(radius * std::sin(phi) * std::sin(theta),
                   radius * std::cos(phi),
                   radius * std::sin(phi) * std::cos(theta));
}

inline glm::vec3 getUpOnSphere(float phi, float theta) {
  return glm::vec3(-std::cos(phi) * std::sin(theta), std::sin(phi),
                   -std::cos(phi) * std::cos(theta));
}

inline glm::vec3 getRightOnSphere(float phi, float theta) {
  return glm::vec3(std::cos(theta), 0.0f, -std::sin(theta));
}

inline glm::mat4x4 getTransformMatrix(const glm::vec3& scale,
                                      const glm::quat& rotation,
                                      const glm::vec3& translation) {
  // Order matters: scale -> rotate -> translate
  // Due to right multiplication of matrices
  glm::mat4 model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, translation);
  model_matrix = model_matrix * glm::mat4_cast(rotation);
  model_matrix = glm::scale(model_matrix, scale);

  return model_matrix;
}

struct RayIntersectionResultSet {
  float distance;
  float u;
  float v;
};

typedef std::optional<RayIntersectionResultSet> RayIntersectionResult;

inline RayIntersectionResult getRayIntersectionDistance(
    const glm::vec3& ray_origin, const glm::vec3& ray_direction,
    const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {
  const float EPSILON = 1e-04f;

  // Find vectors for two edges sharing v0
  glm::vec3 edge1 = v1 - v0;
  glm::vec3 edge2 = v2 - v0;

  // Begin calculating determinant - also used to calculate `u` parameter
  glm::vec3 h = glm::cross(ray_direction, edge2);
  float a = glm::dot(edge1, h);

  // If determinant is near zero, ray is parallel to the triangle plane
  if (a > -EPSILON && a < EPSILON) return std::nullopt;

  // Calculate the inverse of the determinant
  float f = 1.0f / a;

  // Calculate distance from v0 to ray origin
  glm::vec3 s = ray_origin - v0;

  // Calculate `u` parameter and test bounds
  float u = f * glm::dot(s, h);
  if (u < 0.0f || u > 1.0f) return std::nullopt;

  // Calculate `v` parameter
  glm::vec3 q = glm::cross(s, edge1);
  float v = f * glm::dot(ray_direction, q);
  if (v < 0.0f || u + v > 1.0f) return std::nullopt;

  // Calculate t to find the intersection point
  float t = f * glm::dot(edge2, q);
  if (t < EPSILON) return std::nullopt;  // No intersection

  return RayIntersectionResultSet{t, u, v};
}

inline glm::vec3 getRayDirectionFromScreen(const glm::vec2& screen_position,
                                           const glm::vec2& canvas_size,
                                           float fovy,
                                           const glm::mat4& view_matrix) {
  float ndcX = (2.0f * screen_position.x) / canvas_size.x - 1.0f;
  float ndcY = 1.0f - (2.0f * screen_position.y) / canvas_size.y;

  float aspect_ratio = static_cast<float>(canvas_size.x) / canvas_size.y;
  float tanHalfFov = tan(fovy / 2.0f);
  float half_height = tanHalfFov;
  float half_width = aspect_ratio * tanHalfFov;

  glm::vec3 ray_eye = glm::vec3(half_width * ndcX, half_height * ndcY, -1.0f);

  glm::vec4 ray_world = glm::inverse(view_matrix) * glm::vec4(ray_eye, 0.0f);
  glm::vec3 ray_direction = glm::normalize(glm::vec3(ray_world));

  return ray_direction;
}
