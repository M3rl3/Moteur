#pragma once

#include <ECS/Component.h>

#include "../cAnimationManager/Animation.h"

#include <glm/glm.hpp>

struct EntityBoneData {

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::quat Rotation;
	glm::mat4 ModelMatrix;
};

// Struct holding data on animations
struct AnimationComponent : Component {
public:
	// Constructor
	AnimationComponent() : 
		componentType("AnimationComponent")
	{};

	// Destructor
	~AnimationComponent() {};

	// The actual animation
	Animation animation;

	// Character animation
	std::vector<glm::mat4> BoneModelMatrices;
	std::vector<glm::mat4> GlobalTransformations;
	glm::mat4 BoneRotationMatrices[66];

	bool isCharacterAnimation;

private:

	// name of the component
	std::string componentType;
};