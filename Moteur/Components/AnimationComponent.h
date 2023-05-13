#pragma once

#include <ECS/Component.h>

#include "../cAnimationManager/cAnimator.h"

#include <glm/glm.hpp>

enum AnimationState {
	BREATHING_IDLE,
	WALK,
	SPRINT
};

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
		sprint(false),
		animationState(AnimationState::BREATHING_IDLE)
	{
		for (int i = 0; i < 8; i++) {
			animation[i] = nullptr;
		}
	};

	// Destructor
	~AnimationComponent() {};

	// The animator instance
	Animator* animator;

	// The actual animations
	Animation* animation[8];

	// State of the animation
	AnimationState animationState;

	// If the animator is to be used
	bool useAnimator;

	// If the animation is supposed to be playing
	bool isPlaying;

	// If the character is walking/sprinting
	bool sprint;

private:
	// name of the component
	std::string componentType;
};