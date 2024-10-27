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

inline const std::string camera_block = R"(
    layout (std140) uniform CameraBlock
    {
        mat4 u_camera_viewMatrix;
        mat4 u_camera_projectionMatrix;
        vec3 u_camera_eye;
    };
)";

inline const std::string model_block = R"(
    layout (std140) uniform ModelBlock
    {
        mat4 u_model_matrix;
    };
)";

inline const std::string brush_block = R"(
    layout (std140) uniform BrushBlock
    {
        float u_brush_airPressure;
        vec3 u_brush_paintColor;
        float u_brush_nozzleFov;
        mat4 u_brush_viewMatrix;
        mat4 u_brush_projectionMatrix;
        vec3 u_brush_position;
    };
)";

inline const std::string time_block = R"(
    layout (std140) uniform TimeBlock
    {
        float u_time_elapsed_ms;
        float u_time_delta_ms;
    };
)";

}  // namespace shader_source