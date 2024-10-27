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

#include "./Component/GrUniformComponent.h"
#include "./Component/TransformComponent.h"
#include "./Entity/PaintableEntity.h"

struct TransformUpdatingChild {
  std::reference_wrapper<TransformComponent> transform_component;
  std::reference_wrapper<GrUniformComponent> gr_uniform_component;
};

class TransformUpdatingView {
 public:
  TransformUpdatingView(
      std::reference_wrapper<PaintableEntity> paintable_entity)
      : parent_transform_component(
            std::ref(*paintable_entity.get().transform_component)) {
    for (const auto& paintable_part :
         paintable_entity.get().paintable_part_entities) {
      children_transforms.push_back(
          {.transform_component =
               std::ref(*paintable_part->transform_component),
           .gr_uniform_component =
               std::ref(*paintable_part->gr_transform_uniform_component)});
    }
  }

  std::reference_wrapper<TransformComponent> parent_transform_component;
  std::vector<TransformUpdatingChild> children_transforms;
};
