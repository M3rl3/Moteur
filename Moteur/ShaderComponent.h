#pragma once

#include <ECS/Component.h>
#include <string>

struct ShaderComponent : Component {
public:
	ShaderComponent() : 
		componentType("ShaderComponent"), 
		shaderID(NULL) {};

	~ShaderComponent() {};

private:
	std::string componentType;

	unsigned int shaderID;
};