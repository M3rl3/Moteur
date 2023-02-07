#pragma once

#include "OpenGL.h"
#include "LoadModel.h"
#include "ParticleAccelerator.h"
#include "DrawBoundingBox.h"

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cShaderManager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"
#include "cBasicTextureManager/cBasicTextureManager.h"
#include "cRenderReticle.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

namespace Engine {

	// Life Cycle
	void Engine_Initialize();
	void Engine_Update();
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
	void Engine_SetDrawingArray(std::vector<cMeshInfo*> vecMesh);
	void Engine_SetPlayerMesh(cMeshInfo* playerMesh);
	void Engine_SetPlayerMesh(unsigned int id);

	cMeshInfo* Engine_GetMeshObjectFromVector(int id);

	GLFWwindow* Engine_GetWindow();
}