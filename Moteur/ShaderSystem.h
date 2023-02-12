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

	void Shutdown();

private:
	cShaderManager* shaderManager;

	unsigned int shaderID = 0;
};