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

#include "./Component/GrShaderManagerComponent.h"

#include <GLES3/gl3.h>

GrShaderManagerComponent::GrShaderManagerComponent() {
  shader_program_ids = std::unordered_map<ShaderType, unsigned int>();
}

GrShaderManagerComponent::~GrShaderManagerComponent() {
  for (auto& shader_program_id : shader_program_ids) {
    glDeleteProgram(shader_program_id.second);
  }
}

unsigned int GrShaderManagerComponent::getShaderProgramId(
    ShaderType shader_type) {
  auto it = shader_program_ids.find(shader_type);

  if (it == shader_program_ids.end()) {
    unsigned int shader_program_id = generateShaderProgram(shader_type);
    shader_program_ids[shader_type] = shader_program_id;
    return shader_program_id;
  }

  return it->second;
}
