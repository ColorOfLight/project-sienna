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

#include "./Component/EventComponent.h"
#include "./Component/GrShaderManagerComponent.h"
#include "./Component/GrUniformComponent.h"
#include "./Component/InputComponent.h"
#include "./Component/RenderConfigComponent.h"

class GameEntity {
 public:
  GameEntity() {
    event_component = std::make_unique<EventComponent>();
    input_component = std::make_unique<InputComponent>();
    gr_shader_manager_component = std::make_unique<GrShaderManagerComponent>();
    render_config_component = std::make_unique<RenderConfigComponent>(
        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    gr_time_uniform_component =
        std::make_unique<GrUniformComponent>("TimeBlock");
  }

  std::unique_ptr<EventComponent> event_component;
  std::unique_ptr<InputComponent> input_component;
  std::unique_ptr<GrShaderManagerComponent> gr_shader_manager_component;
  std::unique_ptr<RenderConfigComponent> render_config_component;
  std::unique_ptr<GrUniformComponent> gr_time_uniform_component;
};
