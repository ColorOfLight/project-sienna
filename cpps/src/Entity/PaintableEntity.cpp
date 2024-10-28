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

#include "./Entity/PaintableEntity.h"

#include "./shader/core.h"

PaintableEntity::PaintableEntity(PaintablePreset preset) {
  material_component = std::make_unique<MaterialComponent>(ShaderType::PHONG);
  transform_component = std::make_unique<TransformComponent>();

  if (preset == PaintablePreset::CUBE) {
    // front face
    paintable_part_entities.push_back(std::make_unique<PaintablePartEntity>(
        PaintablePartPreset::PLANE, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.0f, 0.0f, 0.5f)));

    // back face
    paintable_part_entities.push_back(std::make_unique<PaintablePartEntity>(
        PaintablePartPreset::PLANE, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f)),
        glm::vec3(0.0f, 0.0f, -0.5f)));

    // left face
    paintable_part_entities.push_back(std::make_unique<PaintablePartEntity>(
        PaintablePartPreset::PLANE, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::quat(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f)),
        glm::vec3(-0.5f, 0.0f, 0.0f)));

    // right face
    paintable_part_entities.push_back(std::make_unique<PaintablePartEntity>(
        PaintablePartPreset::PLANE, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::quat(glm::vec3(0.0f, glm::radians(90.0f), 0.0f)),
        glm::vec3(0.5f, 0.0f, 0.0f)));

    // top face
    paintable_part_entities.push_back(std::make_unique<PaintablePartEntity>(
        PaintablePartPreset::PLANE, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f)),
        glm::vec3(0.0f, 0.5f, 0.0f)));

    // bottom face
    paintable_part_entities.push_back(std::make_unique<PaintablePartEntity>(
        PaintablePartPreset::PLANE, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::quat(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f)),
        glm::vec3(0.0f, -0.5f, 0.0f)));
  } else if (preset == PaintablePreset::PLANE) {
    paintable_part_entities.push_back(std::make_unique<PaintablePartEntity>(
        PaintablePartPreset::PLANE, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f)),
        glm::vec3(0.0f, 0.0f, 0.0f)));
    paintable_part_entities.push_back(std::make_unique<PaintablePartEntity>(
        PaintablePartPreset::PLANE, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::quat(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f)),
        glm::vec3(0.0f, -0.0001f, 0.0f)));
  } else if (preset == PaintablePreset::SPHERE) {
    paintable_part_entities.push_back(std::make_unique<PaintablePartEntity>(
        PaintablePartPreset::SPHERE, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.0f, 0.0f, 0.0f)));
  } else {
    throw std::invalid_argument("Invalid paintable preset");
  }
}
