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

#include <string>

namespace shader_source {
inline const std::string basic_vertex = R"(#version 300 es
    precision mediump float;

    layout (std140) uniform CameraBlock
    {
        mat4 u_camera_viewMatrix;
        mat4 u_camera_projectionMatrix;
        vec3 u_camera_eye;
    };

    layout (std140) uniform ModelBlock
    {
        mat4 u_model_matrix;
    };

    layout (location = 0) in vec3 a_position;
    layout (location = 1) in vec3 a_normal;
    layout (location = 2) in vec2 a_texCoord;

    out vec3 v_position;
    out vec3 v_normal;
    out vec2 v_texCoord;

    void main()
    {
        gl_Position = u_camera_projectionMatrix * u_camera_viewMatrix * u_model_matrix * vec4(a_position, 1.0);

        vec4 modelPosition = u_model_matrix * vec4(a_position, 1.0);
        v_position = modelPosition.xyz;

        mat3 normalMatrix = transpose(inverse(mat3(u_model_matrix)));
        v_normal = normalize(normalMatrix * a_normal);

        v_texCoord = a_texCoord;
    }
)";

inline const std::string texture_test_fragment = R"(#version 300 es
    precision mediump float;

    out vec4 FragColor;

    in vec2 v_texCoord;

    void main()
    {
        FragColor = vec4(v_texCoord, 0.0, 1.0);
    }
)";
}  // namespace shader_source
