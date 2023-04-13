#pragma once

#include <ECS/Component.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

// Struct holding the transform information of a mesh
struct TransformComponent : Component {
public:
	// Constructor
	TransformComponent() :
		componentType("TransfromComponent"),
		position(glm::vec3(0.f)), 
		scale(glm::vec3(0.f)), 
		rotation(glm::quat(glm::vec3(0.f))),
		matModel(glm::mat4(1.f)),
		initMatModel(glm::mat4(1.f)),
		useModel(false),
		doOnce(true)
	{}

	// Destructor
	~TransformComponent() {}

	// Mesh position
	glm::vec3 position;
	// Mesh size
	glm::vec3 scale;
	// Mesh rotation
	glm::quat rotation;

	// Model matrix
	glm::mat4 matModel;
	// the initial transformation of the entity
	glm::mat4 initMatModel;

	// If the model matrix is to be used
	bool useModel;

	// throwaway
	bool doOnce;

private:
	// name of the component
	std::string componentType;
};