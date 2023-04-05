#pragma once

#include <fstream>
#include <iostream>

#include <string>
#include <vector>

#include <glm/vec3.hpp>

#include "../OpenGL.h"
#include "../cVAOManager/cVAOManager.h"

class cModelFileLoader {
public:
	cModelFileLoader();
	~cModelFileLoader();

	sModelDrawInfo* GetPlyModelByID(unsigned int id);

	void SetBasePath(std::string filePath);
	int LoadModel(std::string fileName, sModelDrawInfo& plyModel);

private:
	std::vector<sModelDrawInfo*> plyModels;
	std::string basePath;
};
