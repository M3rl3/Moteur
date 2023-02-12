#pragma once

#include <ECS/System.h>

#include "Camera.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "cShaderManager/cShaderManager.h"

class ShaderSystem : public System {
public:
	ShaderSystem();
	~ShaderSystem();

	cShaderManager* GetShaderManager();

	void Process(const std::vector<Entity*>& entities, float dt);
	void CreateShaderProgramFromFiles(unsigned int& id, const char* vertShader, const char* fragShader);

	Camera* GetCamera();
	void SetCameraPosition(glm::vec3 cameraEye);
	void SetCameraTarget(glm::vec3 cameraTarget);

private:
	cShaderManager* shaderManager;

	Camera* camera;

	unsigned int shaderID = 0;
};