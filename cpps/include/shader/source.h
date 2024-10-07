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

inline const std::string phong_fragment = R"(#version 300 es
    precision mediump float;

    layout (std140) uniform CameraBlock
    {
        mat4 u_camera_viewMatrix;
        mat4 u_camera_projectionMatrix;
        vec3 u_camera_eye;
    };

    uniform sampler2D u_dirtMapTexture;

    vec3 g_material_color = vec3(1.0, 0.0, 0.0);
    float g_material_diffuse = 0.5;
    float g_material_specular = 0.5;
    float g_material_alpha = 64.0;

    vec3 g_ambient_color = vec3(1.0, 1.0, 1.0);
    float g_ambient_intensity = 0.1;

    vec3 g_directional_color = vec3(1.0, 1.0, 1.0);
    vec3 g_directional_direction = vec3(-1.0, -2.0, -1.0);
    float g_directional_intensity = 1.0;

    in vec3 v_normal;
    in vec3 v_position;
    in vec2 v_texCoord;

    out vec4 FragColor;

    void main()
    {
        float dirt = texture(u_dirtMapTexture, v_texCoord).r;
        vec3 material_color = mix(g_material_color, vec3(0.5, 0.5, 0.5), dirt);

        vec3 ambientColor = material_color * g_ambient_color * g_ambient_intensity * g_material_diffuse;

        vec3 normal = normalize(v_normal);
        vec3 lightVector = normalize(-g_directional_direction);
        vec3 directionalDiffuseColor = g_material_diffuse * max(dot(normal, lightVector), 0.0) * material_color * g_directional_color * g_directional_intensity;

        vec3 viewVector = normalize(u_camera_eye - v_position);
        vec3 reflection = reflect(-lightVector, normal);
        vec3 directionalSpecularColor = g_material_specular * pow(max(0.0, dot(reflection, viewVector)), g_material_alpha) * g_directional_color * g_directional_intensity;
        
        vec3 color = ambientColor + directionalDiffuseColor + directionalSpecularColor * (1.0 - dirt);
        FragColor = vec4(color, 1.0);
    }
)";

}  // namespace shader_source
