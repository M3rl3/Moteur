#pragma once

#include <ECS/Component.h>
#include <glm/mat4x4.hpp>

// Struct holding the model matrix of the model
struct LitComponent : Component {
public:
	// Constructor
	LitComponent() :
		componentType("LitComponent"),
		doNotLight(true)
	{};

	// Destructor
	~LitComponent() {};

	// if the object is to be influenced by lighting
	bool doNotLight;

private:
	// name of the component
	std::string componentType;
};