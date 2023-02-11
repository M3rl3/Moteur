#pragma once

#include <ECS/System.h>

#include "ShaderComponent.h"
#include "TransformComponent.h"
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

	std::string shaderComponent;
	std::string meshComponent;
	std::string transformComponent;

	ShaderComponent* GetShaderComponent(const std::vector<Component*>& components);
	//TransformComponent* GetTransformComponent(const std::vector<Component*>& components);

};