#pragma once

#include "../OpenGL.h"
#include "../Window/Window.h"
#include "../Camera/Camera.h"
#include "../OldMeshClass/cMeshInfo.h"

#include "../cAnimationManager/AnimationManager.h"

namespace Moteur {

	// Life Cycle

	// Init the engine
	void Engine_Initialize();

	// Update function running every tick
	void Engine_Update(const float& dt);

	// Callback for a user defined update method
	void Engine_UpdateCallback(void (*Callback)(float dt));

	// Gracefully closes everything down
	void Engine_Shutdown();

	// Init the window object
	void Engine_CreateWindow(const char* title, const int width, const int height, bool fullScreen, bool enableMouse);

	// Setter for enabling mouse input
	void Engine_SetEnableMouse(bool enableMouse);

	// Compile the shaders provided
	void Engine_CreateShaderProgramFromFiles(unsigned int& id, const char* vertexShader, const char* fragmentShader);

	// Set a readfile for asset names and paths
	void Engine_LoadAssetsFromTextFile(const char* path);

	// Load a mesh model into VAO
	void Engine_LoadModel(int& id, const char* filePath, const char* modelName, bool doNotLight, glm::vec3 position, glm::vec4 color);

	// Set the path where the textures are located
	void Engine_SetTexturePath(const char* filePath);

	// Load a 2D texture from bmp file
	void Engine_Create2DTextureFromBMPFile(const char* filePath);

	// Load a cubemap texture from bmp files
	void Engine_CreateCubeMapTextureFromBMPFiles(std::string cubeMapName,
					std::string posX_fileName, std::string negX_fileName,
					std::string posY_fileName, std::string negY_fileName,
					std::string posZ_fileName, std::string negZ_fileName,
					bool bIsSeamless, std::string& errorString);

	// Set initial position of the camera
	void Engine_SetCameraPosition(glm::vec3 cameraEye);

	// Set the camera lookAt
	void Engine_SetCameraTarget(glm::vec3 cameraTarget);

	// Toggle if reticle should be visible
	void Engine_SetEnableCrosshair(bool enabled);

	// Manually create an array of objects to be drawn
	void Engine_SetDrawingArray(std::vector<cMeshInfo*> vecMesh);

	// Getter for the array of objects to be drawn
	void Engine_GetDrawingArray(std::vector<cMeshInfo*> &vecMesh);

	// Set a player model
	void Engine_SetPlayerMesh(cMeshInfo* playerMesh);
	void Engine_SetPlayerMesh(unsigned int id);

	// Set a skybox model
	void Engine_SetSkyboxMesh(cMeshInfo* skyboxMesh);
	void Engine_SetSkyboxMesh(unsigned int id);

	// Getter for keyboard input array
	bool* Engine_GetKeyPressedArray();

	//void Engine_SetDeltaTime(float dt);
	
	// Getter for animation manager
	AnimationManager* Engine_GetAnimationManager();

	// Getter for a specific object from the drawing array
	cMeshInfo* Engine_GetMeshObjectFromVector(int id);

	// Getter for the window object
	Window* Engine_GetWindow();

	// Getter for the camera object
	Camera* Engine_GetCameraObject();
}