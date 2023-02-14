#pragma once

#include <ECS/System.h>

#include "Camera.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "cShaderManager/cShaderManager.h"

// System that handles all things shader related
class ShaderSystem : public System {
public:
	// Constructor
	ShaderSystem();
	// Destructor
	~ShaderSystem();

	// Getter for shadermanager
	cShaderManager* GetShaderManager();

	// Update method of the system, called every tick
	void Process(const std::vector<Entity*>& entities, float dt);

	// Compile the shaders provided
	void CreateShaderProgramFromFiles(unsigned int& id, const char* vertShader, const char* fragShader);

	// Gracefully closes everything down
	void Shutdown();

private:
	// Pointer to the shader manager
	cShaderManager* shaderManager;

	// Compiled shader ID
	unsigned int shaderID = 0;
};