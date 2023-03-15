#pragma once

#include <ECS/System.h>

#include "cVAOManager/cVAOManager.h"
#include "cTextureManager/cTextureManager.h"

#include "LoadModel.h"

// System that handles all things mesh related
class MeshSystem : public System {
public:

	// Constructor
	MeshSystem();

	// Destructor
	~MeshSystem();

	// Getter for the VAO manager
	cVAOManager* GetVAOManager();

	// Load mesh model from ply file into VAO
	bool LoadMesh(std::string fileName, std::string modelName, sModelDrawInfo& plyModel, unsigned int shaderID);

	// Set the path where the textures are located
	void SetTexturePath(const char* filePath);

	// Load a 2D texture
	bool Load2DTexture(unsigned int& textureID, const char* filePath);

	// Load a skybox texture
	bool LoadCubeMapTexture(
		unsigned int& textureID,
		std::string cubeMapName,
		std::string posX_fileName, std::string negX_fileName,
		std::string posY_fileName, std::string negY_fileName,
		std::string posZ_fileName, std::string negZ_fileName,
		bool bIsSeamless, std::string& errorString);

	// The update method called every tick
	void Process(const std::vector<Entity*>& entities, float dt);

	// Gracefully closes everything down
	void Shutdown();

private:

	// name of the system
	std::string systemName;

	// pointer to the VAO manager object
	cVAOManager* vaoManager;

	// pointer to the texture manager object
	cTextureManager* textureManager;
};