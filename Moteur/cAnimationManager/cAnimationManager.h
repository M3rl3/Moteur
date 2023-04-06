#pragma once

#include <string>
#include <map>

#include "Animation.h"

class cAnimationManager {
public:
	cAnimationManager();
	~cAnimationManager();

	bool LoadAnimation(const std::string& name, AnimationData animation);
	// void Update(const std::vector<cMeshInfo*>& gameObjects, float dt);

private:
	int FindPositionKeyFrameIndex(const AnimationData& animation, float time);
	int FindScaleKeyFrameIndex(const AnimationData& animation, float time);
	int FindRotationKeyFrameIndex(const AnimationData& animation, float time);
	int FindNullKeyFrameIndex(const AnimationData& animation, float time);

	glm::vec3 GetAnimationPosition(const AnimationData& animation, float time);
	glm::vec3 GetAnimationScale(const AnimationData& animation, float time);
	glm::quat GetAnimationRotation(const AnimationData& animation, float time);
	std::string GetAnimationNullFrame(const AnimationData& animation, float time);
	EasingType GetAnimationEasing(const AnimationData& animation, float time, int value);

	std::map<std::string, AnimationData> m_Animations;
	glm::mat4 m_GlobalInverseTransform;
};