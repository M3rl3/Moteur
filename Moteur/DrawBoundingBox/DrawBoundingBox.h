#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../cVAOManager/cVAOManager.h"

void draw_bbox(sModelDrawInfo* model, unsigned int shaderID, glm::mat4 meshTransform);