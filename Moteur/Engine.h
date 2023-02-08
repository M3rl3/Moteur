#pragma once

#include "OpenGL.h"
#include "Window.h"
#include "Camera.h"
#include "cMeshInfo.h"

#include "AnimationManager.h"

namespace Engine {

	// Life Cycle
	void Engine_Initialize();
	void Engine_Update(const float& dt);
	void Engine_UpdateCallback(void (*Callback)(float dt));
	void Engine_Shutdown();

	void Engine_CreateWindow(const char* title, const int width, const int height, bool fullScreen, bool enableMouse);
	void Engine_SetEnableMouse(bool enableMouse);
	void Engine_CreateShaderProgramFromFiles(unsigned int& id, const char* vertexShader, const char* fragmentShader);
	void Engine_LoadAssetsFromTextFile(const char* path);
	void Engine_LoadModel(int& id, const char* filePath, const char* modelName, bool doNotLight, glm::vec3 position, glm::vec4 color);
	void Engine_SetTexturePath(const char* filePath);
	void Engine_Create2DTextureFromBMPFile(const char* filePath);
	void Engine_CreateCubeMapTextureFromBMPFiles(std::string cubeMapName,
					std::string posX_fileName, std::string negX_fileName,
					std::string posY_fileName, std::string negY_fileName,
					std::string posZ_fileName, std::string negZ_fileName,
					bool bIsSeamless, std::string& errorString);

	void Engine_SetCameraPosition(glm::vec3 cameraEye);
	void Engine_SetCameraTarget(glm::vec3 cameraTarget);
	void Engine_SetEnableCrosshair(bool enabled);

	void Engine_SetDrawingArray(std::vector<cMeshInfo*> vecMesh);
	void Engine_GetDrawingArray(std::vector<cMeshInfo*> &vecMesh);

	void Engine_SetPlayerMesh(cMeshInfo* playerMesh);
	void Engine_SetPlayerMesh(unsigned int id);
	void Engine_SetSkyboxMesh(cMeshInfo* skyboxMesh);
	void Engine_SetSkyboxMesh(unsigned int id);

	//void Engine_SetDeltaTime(float dt);
	AnimationManager* Engine_GetAnimationManager();
	cMeshInfo* Engine_GetMeshObjectFromVector(int id);
	Window* Engine_GetWindow();
	Camera* Engine_GetCameraObject();
}