#pragma once

#include <ECS/System.h>

#include "MeshComponent.h"

#include "cVAOManager/cVAOManager.h"
#include "LoadModel.h"

class MeshSystem : System {
public:
	MeshSystem();
	~MeshSystem();

	cVAOManager* GetVAOManager();

	void LoadMesh(std::string fileName, sModelDrawInfo& plyModel);

private:
	cVAOManager* vaoManager;
};