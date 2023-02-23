#pragma once

#include "cLight.h"

#include <glm/vec4.hpp>

#include <vector>
#include <string>

class cLightManager
{
public:
	cLightManager();
	~cLightManager();

	cLight* AddLight(glm::vec4 position);

	void LoadLightUniformLocations(unsigned int shaderID);

	void CopyLightInformationToShader(unsigned int shaderID);

private:
	unsigned int numLights = 0;
	int numLightsUniformLocation = -1;

	std::vector<cLight*> vecTheLights;
};
