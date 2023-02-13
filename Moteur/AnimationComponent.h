#pragma once

#include "ECS/Component.h"

#include "Animation.h"

struct AnimationComponent : Component {
public:
	AnimationComponent() : 
		componentType("AnimationComponent")
	{};
	~AnimationComponent() {};

	Animation animation;
private:

	std::string componentType;
};