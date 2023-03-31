#pragma once

#include <fstream>
#include <iostream>

#include <string>
#include <vector>

#include "../cVAOManager/cVAOManager.h"

class cPlyFileLoader {
public:

    struct vertexLayout {

        float x, y, z;
        float nx, ny, nz;
        float r, g, b, a;
        float texture_u, texture_v;
    };

    struct triangleLayout {

        unsigned int triangleIndices[3];
    };

    cPlyFileLoader();
    ~cPlyFileLoader();

    sModelDrawInfo* GetPlyModelByID(unsigned int id);

    void SetBasePath(std::string basePath);

    int LoadModel(std::string fileName, sModelDrawInfo& plyModel);

private:
    std::vector<sModelDrawInfo*> plyModels;

    std::string basePath;
};
