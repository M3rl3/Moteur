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
		useAnimator(true),
		isPlaying(true),
		sprint(false)
	{
		for (int i = 0; i < 8; i++) {
			animation[i] = nullptr;
		}
	};

	// Destructor
	~AnimationComponent() {};

	// The actual animation
	Animator* animator;
	Animation* animation[8];

	bool useAnimator;
	bool isPlaying;

	bool sprint;

private:
	// name of the component
	std::string componentType;
};