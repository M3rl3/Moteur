#pragma once

#include <fstream>
#include <iostream>

#include <string>
#include <vector>

#include <assimp/scene.h>
#include <glm/vec3.hpp>

#include "../OpenGL.h"
#include "../cVAOManager/cVAOManager.h"

class cModelFileLoader {
public:
	cModelFileLoader();
	~cModelFileLoader();

	sModelDrawInfo* GetPlyModelByID(unsigned int id);

	void SetBasePath(std::string filePath);

	int LoadModelPLY(std::string fileName, sModelDrawInfo& plyModel);
	int LoadModelFBX(std::string fileName, sModelDrawInfo& plyModel);

	int TransferColorTextureToVertex(std::string fileName);

	unsigned char* LoadEmbeddedTexture(aiTexture* texture);

private:

	std::vector<sModelDrawInfo*> plyModels;
	std::vector<sModelDrawInfo*> fbxModels;

	std::string basePath;
};
