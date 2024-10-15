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

inline const std::string brush_decal_vertex = R"(#version 300 es
    precision mediump float;

    layout (std140) uniform BrushBlock
    {
        float u_brush_airPressure;
        vec3 u_brush_paintColor;
        float u_brush_paintViscosity;
        float u_brush_nozzleFov;
        mat4 u_brush_viewMatrix;
        mat4 u_brush_projectionMatrix;
        vec3 u_brush_position;
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
    out vec3 v_projectedPosition;

    void main()
    {
        float positionX = a_texCoord.x * 2.0 - 1.0;
        float positionY = a_texCoord.y * 2.0 - 1.0;
        float positionZ = 1.0; // TODO: calculate z value
        gl_Position = vec4(positionX, positionY, positionZ, 1.0);

        vec4 modelPosition = u_model_matrix * vec4(a_position, 1.0);
        v_position = modelPosition.xyz;

        vec4 projectedPosition = u_brush_projectionMatrix * u_brush_viewMatrix * modelPosition;
        v_projectedPosition = projectedPosition.xyz;

        mat3 normalMatrix = transpose(inverse(mat3(u_model_matrix)));
        v_normal = normalize(normalMatrix * a_normal);
    }
)";

inline const std::string brush_decal_fragment = R"(#version 300 es
    precision mediump float;

    layout (std140) uniform BrushBlock
    {
        float u_brush_airPressure;
        vec3 u_brush_paintColor;
        float u_brush_paintViscosity;
        float u_brush_nozzleFov;
        mat4 u_brush_viewMatrix;
        mat4 u_brush_projectionMatrix;
        vec3 u_brush_position;
    };

    out vec4 FragColor;

    in vec3 v_position;
    in vec3 v_normal;
    in vec3 v_projectedPosition;

    void main()
    {
        float k = 1.0;
        float distance = length(v_projectedPosition - u_brush_position);
        float distanceFactor = exp(-distance * u_brush_paintViscosity);

        float epsilon = 1e-6;
        float tanFov = tan(u_brush_nozzleFov / 2.0) + epsilon;
        float fovFactor = 1.0 / pow(tanFov, 2.0);

        float intensity = k * u_brush_airPressure * distanceFactor * fovFactor;
        FragColor = vec4(u_brush_paintColor, min(intensity, 1.0));
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

    struct AmbientLight
    {
        vec3 color;
        float intensity;
    };

    struct DirectionalLight
    {
        vec3 color;
        vec3 direction;
        float intensity;
    };

    struct PointLight
    {
        vec3 color;
        vec3 position;
        float intensity;
        float quadraticAttenuation;
    };

    struct Material
    {
        vec3 color;
        float diffuse;
        float specular;
        float alpha;
    };

    vec3 getAmbientDiffuse(AmbientLight ambient_light, Material material)
    {
        return material.color * ambient_light.color * ambient_light.intensity * material.diffuse;
    }

    vec3 getDirectionalDiffuse(DirectionalLight directional_light, Material material, vec3 normal)
    {
        vec3 lightVector = normalize(-directional_light.direction);
        return material.diffuse * max(dot(normal, lightVector), 0.0) * material.color * directional_light.color * directional_light.intensity;
    }

    vec3 getDirectionalSpecular(DirectionalLight directional_light, Material material, vec3 normal, vec3 viewVector)
    {
        vec3 lightVector = normalize(-directional_light.direction);
        vec3 reflection = reflect(-lightVector, normal);
        return material.specular * pow(max(0.0, dot(reflection, viewVector)), material.alpha) * material.color * directional_light.color * directional_light.intensity;
    }

    vec3 getPointDiffuse(PointLight point_light, Material material, vec3 normal, vec3 position)
    {
        vec3 lightVector = normalize(point_light.position - position);
        float distance = length(point_light.position - position);
        float attenuation = 1.0 / (1.0 + point_light.quadraticAttenuation * distance * distance);
        return material.diffuse * max(dot(normal, lightVector), 0.0) * material.color * point_light.color * point_light.intensity * attenuation;
    }

    vec3 getPointSpecular(PointLight point_light, Material material, vec3 normal, vec3 position, vec3 viewVector)
    {
        vec3 lightVector = normalize(point_light.position - position);
        vec3 reflection = reflect(-lightVector, normal);
        float distance = length(point_light.position - position);
        float attenuation = 1.0 / (1.0 + point_light.quadraticAttenuation * distance * distance);
        return material.specular * pow(max(0.0, dot(reflection, viewVector)), material.alpha) * material.color * point_light.color * point_light.intensity * attenuation;
    }

    uniform sampler2D u_dirtMapTexture;

    in vec3 v_normal;
    in vec3 v_position;
    in vec2 v_texCoord;

    out vec4 FragColor;

    void main()
    {
        AmbientLight g_ambient_light;
        g_ambient_light.color = vec3(0.8, 0.8, 0.75); // warm room light
        g_ambient_light.intensity = 0.3;

        DirectionalLight g_ceiling_light;
        g_ceiling_light.color = vec3(1.0, 1.0, 0.95); // warm ceiling light
        g_ceiling_light.direction = vec3(0.0, -1.0, 0.0);
        g_ceiling_light.intensity = 0.8;

        PointLight g_fill_light;
        g_fill_light.color = vec3(1.0, 0.85, 0.7); // warm white as from a lamp
        g_fill_light.position = vec3(2.0, 3.0, 2.0);
        g_fill_light.intensity = 0.6;
        g_fill_light.quadraticAttenuation = 0.20;

        PointLight g_rim_light;
        g_rim_light.color = vec3(0.8, 0.85, 1.0); // cool white as from a window
        g_rim_light.position = vec3(0.0, 3.0, -3.0);
        g_rim_light.intensity = 0.4;
        g_rim_light.quadraticAttenuation = 0.20;

        Material g_material;
        g_material.color = vec3(1.0, 1.0, 1.0);
        g_material.diffuse = 0.5;
        g_material.specular = 0.5;
        g_material.alpha = 64.0;

        float dirt = texture(u_dirtMapTexture, v_texCoord).r;
        g_material.color = mix(g_material.color, vec3(0.40, 0.26, 0.13), dirt);

        vec3 normal = normalize(v_normal);
        vec3 viewVector = normalize(u_camera_eye - v_position);

        vec3 ambientDiffuse = getAmbientDiffuse(g_ambient_light, g_material);

        vec3 ceilingDiffuse = getDirectionalDiffuse(g_ceiling_light, g_material, normal);
        vec3 ceilingSpecular = getDirectionalSpecular(g_ceiling_light, g_material, normal, viewVector);

        vec3 fillDiffuse = getPointDiffuse(g_fill_light, g_material, normal, v_position);
        vec3 fillSpecular = getPointSpecular(g_fill_light, g_material, normal, v_position, viewVector);

        vec3 rimDiffuse = getPointDiffuse(g_rim_light, g_material, normal, v_position);
        vec3 rimSpecular = getPointSpecular(g_rim_light, g_material, normal, v_position, viewVector);

        vec3 diffuseColor = ambientDiffuse + ceilingDiffuse + fillDiffuse + rimDiffuse;
        vec3 specularColor = ceilingSpecular + fillSpecular + rimSpecular;
        
        vec3 color = diffuseColor + specularColor * (1.0 - dirt);
        FragColor = vec4(color, 1.0);
    }
)";

}  // namespace shader_source
