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
#include <vector>

#include "./shader/block.h"

namespace shader_source {

struct ShaderSourceGroup {
  std::vector<std::reference_wrapper<const std::string>> blocks;
  std::string source;
};

inline const ShaderSourceGroup basic_vertex = {
    .blocks = {camera_block, model_block}, .source = R"(
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
)"};

inline const ShaderSourceGroup brush_decal_vertex = {
    .blocks = {brush_block, model_block}, .source = R"(
    layout (location = 0) in vec3 a_position;
    layout (location = 1) in vec3 a_normal;
    layout (location = 2) in vec2 a_texCoord;

    out vec3 v_position;
    out vec3 v_normal;
    out vec3 v_projectedPosition;
    out vec2 v_texCoord;

    void main()
    {
        v_texCoord = a_texCoord;

        vec4 modelPosition = u_model_matrix * vec4(a_position, 1.0);
        v_position = modelPosition.xyz;

        vec4 projectedPosition = u_brush_projectionMatrix * u_brush_viewMatrix * modelPosition;
        v_projectedPosition = projectedPosition.xyz / projectedPosition.w;

        mat3 normalMatrix = transpose(inverse(mat3(u_model_matrix)));
        v_normal = normalize(normalMatrix * a_normal);

        gl_Position = vec4(v_texCoord * 2.0 - 1.0, 0.0, 1.0);
    }
)"};

inline const ShaderSourceGroup texture_quad_vertex = {.source = R"(
    layout (location = 0) in vec3 a_position;
    layout (location = 1) in vec3 a_normal;
    layout (location = 2) in vec2 a_texCoord;

    out vec2 v_texCoord;

    void main()
    {
        gl_Position = vec4(a_position, 1.0);
        v_texCoord = a_texCoord;
    }
)"};

inline const ShaderSourceGroup brush_depth_vertex = {
    .blocks = {brush_block, model_block}, .source = R"(
    layout (location = 0) in vec3 a_position;
    layout (location = 1) in vec3 a_normal;
    layout (location = 2) in vec2 a_texCoord;

    void main()
    {
        vec4 modelPosition = u_model_matrix * vec4(a_position, 1.0);
        gl_Position = u_brush_projectionMatrix * u_brush_viewMatrix * modelPosition;
    }
)"};

inline const ShaderSourceGroup empty_fragment = {.source = R"(
    void main()
    {
    }
)"};

inline const ShaderSourceGroup brush_decal_fragment = {
    .blocks = {time_block, brush_block}, .source = R"(
    uniform sampler2D u_brushDepthTexture;

    out vec4 FragColor;

    in vec3 v_position;
    in vec3 v_normal;
    in vec3 v_projectedPosition;
    in vec2 v_texCoord;

    float g_intensity_coff = 0.05;

    void main()
    {
        float centerDistance = length(v_projectedPosition.xy);

        // Discard fragments outside the unit circle
        if (centerDistance > 1.0)
        {
            discard;
        }

        float brushDepth = texture(u_brushDepthTexture, v_projectedPosition.xy * 0.5 + 0.5).r;
        float normalizedZ = v_projectedPosition.z * 0.5 + 0.5;

        // Discard fragments behind the brush
        if (normalizedZ - brushDepth > 2.0 * 1e-5)
        {
            discard;
        }

        float tanHalfFov = tan(u_brush_nozzleFov / 2.0);
        float distance = length(v_position - u_brush_position);

        float strength_coff = g_intensity_coff * u_brush_airPressure / (tanHalfFov * tanHalfFov * distance * distance);
        float strength = strength_coff * (1.0 - smoothstep(0.0, 1.0, centerDistance));

        float baseIntensity = 2.0 / 1000.0 * u_time_delta_ms;
        float intensity = clamp(strength * baseIntensity, 0.0, 1.0);

        FragColor = vec4(u_brush_paintColor, intensity);
    }
)"};

inline const ShaderSourceGroup paint_blend_fragment = {.source = R"(
    uniform sampler2D u_paintMapTexture;
    uniform sampler2D u_paintedMapTexture;

    out vec4 FragColor;

    in vec2 v_texCoord;

    void main() {
        vec4 prevPaintedColor = texture(u_paintedMapTexture, v_texCoord);
        vec4 paintColor = texture(u_paintMapTexture, v_texCoord);

        float prevIntensity = prevPaintedColor.a;
        float paintIntensity = paintColor.a;

        float newIntensity = prevIntensity + paintIntensity * (1.0 - prevIntensity);
        newIntensity = clamp(newIntensity, 0.0, 1.0);

        float prevColorIntensity = prevIntensity * (1.0 - paintIntensity);

        float blendFactor = paintIntensity / (prevColorIntensity + paintIntensity);

        vec3 newColor = mix(prevPaintedColor.rgb, paintColor.rgb, blendFactor);

        FragColor = vec4(newColor.rgb, newIntensity);
    }
)"};

inline const ShaderSourceGroup texture_test_fragment = {.source = R"(
    out vec4 FragColor;

    in vec2 v_texCoord;

    void main()
    {
        FragColor = vec4(v_texCoord, 0.0, 1.0);
    }
)"};

inline const ShaderSourceGroup phong_fragment = {.blocks = {camera_block},
                                                 .source = R"(
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

    uniform sampler2D u_paintedMapTexture;

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

        vec4 paintColor = texture(u_paintedMapTexture, v_texCoord);
        g_material.color = mix(g_material.color, vec3(paintColor), paintColor.a);

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
        
        vec3 color = diffuseColor + specularColor;
        FragColor = vec4(color, 1.0);
    }
)"};

}  // namespace shader_source
