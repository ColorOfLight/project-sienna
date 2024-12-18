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

#include <GLES3/gl3.h>

#include "./shader/source.h"

enum class ShaderType {
  TEXTURE_TEST,
  PHONG,
  BRUSH_DECAL,
  BRUSH_DEPTH,
  PAINT_BLEND
};

inline const std::string SHADER_DEFAULT_HEADER = R"(#version 300 es
    precision mediump float;
)";

inline std::string getStringFromSource(
    shader_source::ShaderSourceGroup source_group) {
  std::string source = "";

  for (const auto& block : source_group.blocks) {
    source += block;
  }

  source += source_group.source;

  return source;
}

inline std::string getVertexShaderSource(ShaderType shader_type) {
  std::string shader_source = SHADER_DEFAULT_HEADER;

  switch (shader_type) {
    case ShaderType::TEXTURE_TEST:
    case ShaderType::PHONG:
      shader_source += getStringFromSource(shader_source::basic_vertex);
      break;
    case ShaderType::PAINT_BLEND:
      shader_source += getStringFromSource(shader_source::texture_quad_vertex);
      break;
    case ShaderType::BRUSH_DECAL:
      shader_source += getStringFromSource(shader_source::brush_decal_vertex);
      break;
    case ShaderType::BRUSH_DEPTH:
      shader_source += getStringFromSource(shader_source::brush_depth_vertex);
      break;
    default:
      throw std::runtime_error("ERROR::SHADER::VERTEX::INVALID_SHADER_TYPE\n");
  }

  return shader_source;
};

inline std::string getFragmentShaderSource(ShaderType shader_type) {
  std::string shader_source = SHADER_DEFAULT_HEADER;

  switch (shader_type) {
    case ShaderType::TEXTURE_TEST:
      shader_source +=
          getStringFromSource(shader_source::texture_test_fragment);
      break;
    case ShaderType::PHONG:
      shader_source += getStringFromSource(shader_source::phong_fragment);
      break;
    case ShaderType::BRUSH_DECAL:
      shader_source += getStringFromSource(shader_source::brush_decal_fragment);
      break;
    case ShaderType::BRUSH_DEPTH:
      shader_source += getStringFromSource(shader_source::empty_fragment);
      break;
    case ShaderType::PAINT_BLEND:
      shader_source += getStringFromSource(shader_source::paint_blend_fragment);
      break;
    default:
      throw std::runtime_error(
          "ERROR::SHADER::FRAGMENT::INVALID_SHADER_TYPE\n");
  }

  return shader_source;
};

inline unsigned int generateShaderProgram(ShaderType shader_type) {
  int success;
  char info_log[512];

  std::string vertex_shader_source = getVertexShaderSource(shader_type);
  std::string fragment_shader_source = getFragmentShaderSource(shader_type);

  const char* vertex_shader_source_cstr = vertex_shader_source.c_str();
  const char* fragment_shader_source_cstr = fragment_shader_source.c_str();

  // Compile the vertex shader
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader_id, 1, &vertex_shader_source_cstr, nullptr);
  glCompileShader(vertex_shader_id);

  // Check for vertex shader compile errors
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader_id, 512, nullptr, info_log);
    throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" +
                             std::string(info_log));
  }

  // Compile the fragment shader
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader_id, 1, &fragment_shader_source_cstr, nullptr);
  glCompileShader(fragment_shader_id);

  // Check for fragment shader compile errors
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader_id, 512, nullptr, info_log);
    throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" +
                             std::string(info_log));
  }

  // Link shaders to create a shader program
  GLuint shader_program_id = glCreateProgram();
  glAttachShader(shader_program_id, vertex_shader_id);
  glAttachShader(shader_program_id, fragment_shader_id);
  glLinkProgram(shader_program_id);

  // Check for linking errors
  glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program_id, 512, nullptr, info_log);
    throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" +
                             std::string(info_log));
  }

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  return shader_program_id;
}
