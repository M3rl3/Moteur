#pragma once

#include <ECS/Component.h>
#include <string>

struct ShaderComponent : Component {
public:
	ShaderComponent() : 
		componentType("ShaderComponent"), 
		shaderID(NULL) {};

	~ShaderComponent() {};

	unsigned int shaderID;

private:
	std::string componentType;
};