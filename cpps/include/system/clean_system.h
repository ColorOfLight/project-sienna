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

#include "./Component/CameraComponent.h"
#include "./Component/CleanMarkComponent.h"
#include "./Component/CleanerComponent.h"
#include "./Component/DirtMapComponent.h"
#include "./Component/GeometryComponent.h"
#include "./Component/InputComponent.h"
#include "./Component/TransformComponent.h"

namespace clean_system {

void markToClean(
    std::reference_wrapper<const InputComponent> input_component,
    std::reference_wrapper<const CameraComponent> camera_component,
    std::reference_wrapper<const CleanerComponent> cleaner_component,
    std::reference_wrapper<TransformComponent> parent_transform_component,
    std::vector<std::reference_wrapper<const GeometryComponent>>
        geometry_components,
    std::vector<std::reference_wrapper<TransformComponent>>
        child_transform_components,
    std::vector<std::reference_wrapper<CleanMarkComponent>>
        clean_mark_components);

void clean(std::reference_wrapper<CleanMarkComponent> clean_mark_component,
           std::reference_wrapper<DirtMapComponent> dirt_map_component);

}  // namespace clean_system
