#pragma once

#include <string>
#include <map>
#include <glm/gtx/easing.hpp>
#include "Animation.h"
#include "cMeshInfo.h"

class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();
	bool Load(std::string name, AnimationData animation);
	void Update(const std::vector<cMeshInfo*>& meshs, float dt);

private:
	int FindPositionKeyFrameIndex(const AnimationData& animation, float time);
	glm::vec3 GetAnimationPosition(const AnimationData& animation, float time);
	std::map<std::string, AnimationData> m_Animations;
};
