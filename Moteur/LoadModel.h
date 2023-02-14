#pragma once

#include <fstream>
#include <iostream>

#include "cVAOManager/cVAOManager.h"
 
// vertex format for the ply file
struct vertexLayout {

    float x, y, z;
    float nx, ny, nz;
    float r, g, b, a;
    float texture_u, texture_v;
};

// the triangle layout
struct triangleLayout {

    unsigned int triangleIndices[3];
};

// Function for loading the vertices, indices, color, uv coordinates from ply files
void LoadModel(std::string fileName, sModelDrawInfo& plyModel);