#pragma once

#include "ECS/Component.h"

#include "Animation.h"

// Struct for holding info on animations
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
private:

	// name of the component
	std::string componentType;
};