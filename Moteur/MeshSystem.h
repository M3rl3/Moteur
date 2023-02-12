#pragma once

#include <ECS/System.h>

#include "cVAOManager/cVAOManager.h"
#include "LoadModel.h"

class MeshSystem : public System {
public:
	MeshSystem();
	~MeshSystem();

	cVAOManager* GetVAOManager();

	bool LoadMesh(std::string fileName, std::string modelName, sModelDrawInfo& plyModel, unsigned int shaderID);
	void Process(const std::vector<Entity*>& entities, float dt);

private:
	std::string systemName;

	cVAOManager* vaoManager;
};