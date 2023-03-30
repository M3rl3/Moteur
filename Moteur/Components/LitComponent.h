#pragma once

#include <ECS/Component.h>
#include <glm/mat4x4.hpp>

// Struct holding the model matrix of the model
struct LitComponent : Component {
public:
	// Constructor
	LitComponent() :
		componentType("LitComponent"),
		isLit(true)
	{};

	// Destructor
	~LitComponent() {};

	// if the object is to be influenced by lighting
	bool isLit;

private:
	// name of the component
	std::string componentType;
};