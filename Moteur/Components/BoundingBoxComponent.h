#pragma once

#include <ECS/Component.h>
#include <glm/mat4x4.hpp>

// Struct holding the model matrix of the model
struct BoundingBoxComponent : Component {
public:
	// Constructor
	BoundingBoxComponent() :
		componentType("BoundingBoxComponent"),
		drawBBox(true),
		modelMatrix(glm::mat4x4(1.f))
	{};

	// Destructor
	~BoundingBoxComponent() {};

	// if the bounding box is to be drawn
	bool drawBBox;

	// copy of the model matrix
	glm::mat4 modelMatrix;

private:
	// name of the component
	std::string componentType;
};