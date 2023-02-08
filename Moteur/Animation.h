#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

enum EasingType
{
	EaseIn = 0,
	EaseOut = 1,
	EaseInOut = 2,
	None = 3
};

struct PositionKeyFrame
{
	PositionKeyFrame(glm::vec3 value, float time, EasingType type = None)
		: value(value), time(time), type(type) { }
	glm::vec3 value;
	float time;
	EasingType type;
};

struct AnimationData
{
	std::vector<PositionKeyFrame> PositionKeyFrames;
	float Duration;
};

struct Animation
{
	std::string AnimationType;
	float AnimationTime;
	bool IsPlaying;
	bool IsLooping;
	float Speed;
};
