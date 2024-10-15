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

#pragma once

#include "./shader/source.h"

enum class ShaderType { TEXTURE_TEST, PHONG };

inline const char* getVertexShaderSource(ShaderType shader_type) {
  return shader_source::basic_vertex.c_str();
};

inline const char* getFragmentShaderSource(ShaderType shader_type) {
  switch (shader_type) {
    case ShaderType::TEXTURE_TEST:
      return shader_source::texture_test_fragment.c_str();
    case ShaderType::PHONG:
      return shader_source::phong_fragment.c_str();
    default:
      throw std::runtime_error(
          "ERROR::SHADER::FRAGMENT::INVALID_SHADER_TYPE\n");
  }
};
