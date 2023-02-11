#pragma once

#include <ECS/System.h>
#include "cShaderManager/cShaderManager.h"

class ShaderSystem : System {
public:
	ShaderSystem();
	~ShaderSystem();

	cShaderManager* GetShaderManager();

	void Process(const std::vector<Entity*>& entities, float dt);
	void CreateShaderProgramFromFiles(unsigned int& id, const char* vertShader, const char* fragShader);

private:
	cShaderManager* shaderManager;
};