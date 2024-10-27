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

#include <vector>

#include "./Component/GrGeometryComponent.h"
#include "./Component/GrShaderManagerComponent.h"
#include "./Component/GrTextureComponent.h"
#include "./Component/GrUniformComponent.h"
#include "./shader/core.h"

void drawGrComponents(
    ShaderType shader_type,
    std::reference_wrapper<GrShaderManagerComponent>
        gr_shader_manager_component,
    std::reference_wrapper<GrGeometryComponent> gr_geometry_component,
    const std::vector<std::reference_wrapper<GrUniformComponent>>&
        gr_uniform_components,
    const std::vector<std::reference_wrapper<GrTextureComponent>>&
        gr_texture_components);
