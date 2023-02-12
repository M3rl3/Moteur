#pragma once

#include <ECS/System.h>

#include "cVAOManager/cVAOManager.h"
#include "cBasicTextureManager/cBasicTextureManager.h"

#include "LoadModel.h"

class MeshSystem : public System {
public:
	MeshSystem();
	~MeshSystem();

	cVAOManager* GetVAOManager();

	bool LoadMesh(std::string fileName, std::string modelName, sModelDrawInfo& plyModel, unsigned int shaderID);

	void SetTexturePath(const char* filePath);
	bool Create2DTextureFromBMPFile(unsigned int& textureID, const char* filePath);
	bool CreateCubeMapTextureFromBMPFiles(
		unsigned int& textureID,
		std::string cubeMapName,
		std::string posX_fileName, std::string negX_fileName,
		std::string posY_fileName, std::string negY_fileName,
		std::string posZ_fileName, std::string negZ_fileName,
		bool bIsSeamless, std::string& errorString);

	void Process(const std::vector<Entity*>& entities, float dt);

private:
	std::string systemName;

	cVAOManager* vaoManager;
	cBasicTextureManager* textureManager;
};