#pragma once

#include <ECS/Component.h>

#include "../cAnimationManager/cAnimator.h"

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
		componentType("AnimationComponent"),
		animator(nullptr),
		animation(nullptr),
		useAnimator(true),
		isPlaying(true)
	{};

	// Destructor
	~AnimationComponent() {};

	// The actual animation
	Animator* animator;
	Animation* animation;

	bool useAnimator;
	bool isPlaying;

private:
	// name of the component
	std::string componentType;
};