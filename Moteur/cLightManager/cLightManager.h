#pragma once

#include "cLight.h"
#include <vector>
#include <string>

class cLightManager
{
public:
	cLightManager();
	~cLightManager();

	cLight* AddLight();

	void LoadLightUniformLocations(unsigned int shaderID);

	void CopyLightInformationToShader(unsigned int shaderID);

	unsigned int NUMBER_OF_LIGHTS = 0;
	std::vector<cLight> vecTheLights;
};
