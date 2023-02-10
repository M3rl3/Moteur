#pragma once

#include <fstream>
#include <iostream>

#include "cVAOManager/cVAOManager.h"
 
struct vertexLayout {

    float x, y, z;
    float nx, ny, nz;
    float r, g, b, a;
    float texture_u, texture_v;
};

struct triangleLayout {

    unsigned int triangleIndices[3];
};

void LoadModel(std::string fileName, sModelDrawInfo& plyModel);