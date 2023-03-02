#pragma once

#include <ECS/System.h>

#include <glm/vec4.hpp>

#include "cLightManager/cLightManager.h"

// System that handles all things shader related
class LightSystem : public System {
public:
	// Constructor
	LightSystem();
	// Destructor
	~LightSystem();

	// Getter for shadermanager
	cLightManager* GetLightManager();

	// Add a light to the scene
	cLight* AddLight(glm::vec4 position);
	

	void GetAmbientLightAmount(float& amount);
	void SetAmbientLightAmount(float& amount);

	// Update method of the system, called every tick
	void Process(const std::vector<Entity*>& entities, float dt);

	// Load uniform locations
	void LoadLightUniformLocations(unsigned int shaderID);

	// Copy light info to the shader
	void CopyLightInformationToShader(unsigned int shaderID);

	// Gracefully closes everything down
	void Shutdown();

private:
	// Pointer to the shader manager
	cLightManager* lightManager;

	// Compiled shader ID
	unsigned int shaderID = 0;
};