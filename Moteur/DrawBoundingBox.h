#pragma once

#include "cMeshInfo.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "cVAOManager/cVAOManager.h"

void draw_bbox(cMeshInfo* mesh, unsigned int shaderID, glm::mat4 meshTransform);
void draw_bbox(std::vector<glm::vec3>& vertices, unsigned int shaderID, glm::mat4 meshTransform);
void draw_bbox(sModelDrawInfo* model, unsigned int shaderID, glm::mat4 meshTransform);