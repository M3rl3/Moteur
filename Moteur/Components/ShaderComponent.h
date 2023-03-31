#pragma once

#include <ECS/Component.h>
#include <string>

// Struct holding the shader ID
struct ShaderComponent : Component {
public:
	// Constructor
	ShaderComponent() : 
		componentType("ShaderComponent"), 
		shaderID(NULL) 
	{};

	// Destructor
	~ShaderComponent() {};

	// the compiled shaderID
	unsigned int shaderID;

private:

	// name of the component
	std::string componentType;
};