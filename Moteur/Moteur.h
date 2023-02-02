#pragma once

#include "OpenGL.h"
#include "Callbacks.h"
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
	void Engine_Initialize();
	void Engine_Render();
	void Engine_Update();
	void Engine_Shutdown();

	void Engine_LoadModel();
	void Engine_LoadTexture();
}

class Moteur {
public:

	// Life Cycle
	void Initialize();
	void Render();
	void Update();
	void Shutdown();

	// Other Methods
	void ReadFromFile();
	void ManageLights();
	float RandomFloat(float a, float b);
	bool RandomizePositions(cMeshInfo* mesh);

	GLFWwindow* window;

private:
	GLuint shaderID = 0;

	cVAOManager* VAOMan;
	cBasicTextureManager* TextureMan;
	ParticleAccelerator partAcc;
	cRenderReticle crosshair;

	sModelDrawInfo player_obj;

	cMeshInfo* skybox_sphere_mesh;
	
	cMeshInfo* cube_mesh;
	cMeshInfo* bulb_mesh;

	float beginTime = 0.f;
	float currentTime = 0.f;
	float timeDiff = 0.f;
	int frameCount = 0;

	unsigned int readIndex = 0;
	int elapsed_frames = 0;

	bool mouseClick = false;

	glm::vec3 cameraEye;
	glm::vec3 cameraTarget;

	float yaw = 0.f;
	float pitch = 0.f;
	float fov = 45.f;

	// mouse state
	bool firstMouse = true;
	float lastX = 800.f / 2.f;
	float lastY = 600.f / 2.f;

	cMeshInfo* player_mesh;
	bool enableMouse = false;

	eEditMode theEditMode = MOVING_CAMERA;
	int object_index = 0;

	std::vector <std::string> meshFiles;
};