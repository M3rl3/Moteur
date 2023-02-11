#include "MeshSystem.h"

MeshSystem::MeshSystem()
{
	vaoManager = new cVAOManager();
}

MeshSystem::~MeshSystem() 
{

}

cVAOManager* MeshSystem::GetVAOManager() 
{
	return vaoManager;
}

void MeshSystem::LoadMesh(std::string fileName, sModelDrawInfo& plyModel)
{
	LoadModel(fileName, plyModel);
}

