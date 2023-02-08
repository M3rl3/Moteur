#include "AnimationManager.h"

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
}

bool AnimationManager::Load(std::string name, AnimationData animation)
{
	std::map<std::string, AnimationData>::iterator it = m_Animations.find(name);
	if (it != m_Animations.end()) {
		printf("Sorry Animation already exist!\n");
		return false;
	}
	m_Animations.insert(std::pair<std::string, AnimationData>(name, animation));

	return true;
}

void AnimationManager::Update(const std::vector<cMeshInfo*>& meshs, float dt)
{
	for (int i = 0; i < meshs.size(); i++)
	{
		cMeshInfo* go = meshs[i];

		if (!go->enabled)
			continue;

		if (go->animation.AnimationType.length() != 0)
		{
			Animation& animation = go->animation;
			std::map<std::string, AnimationData>::iterator itFind = m_Animations.find(go->animation.AnimationType);
			const AnimationData& animationData = itFind->second;

			if (itFind != m_Animations.end())
			{
				if (animation.IsPlaying && animation.Speed != 0.0f)
				{
					animation.AnimationTime += dt * animation.Speed;
					if (animation.AnimationTime > animationData.Duration)
					{
						if (animation.IsLooping)
						{
							if (animation.Speed > 0)
							{
								animation.AnimationTime = 0.0f;
							}
							else
							{
								animation.AnimationTime = animationData.Duration;
							}
						}
						else
						{
							animation.AnimationTime = animationData.Duration;
							animation.IsPlaying = false;
						}

					}
					else if (animation.AnimationTime < 0.f)
					{
						if (animation.IsLooping)
						{
							if (animation.Speed < 0)
							{
								animation.AnimationTime = animationData.Duration;
							}
							else
							{
								animation.AnimationTime = 0.f;
							}
						}
						else
						{
							animation.AnimationTime = 0.f;
							animation.IsPlaying = false;
						}
					}
				}

				go->position = GetAnimationPosition(itFind->second, animation.AnimationTime);
			}
		}
	}
}

int AnimationManager::FindPositionKeyFrameIndex(const AnimationData& animation, float time)
{
	for (int i = 0; i < animation.PositionKeyFrames.size(); i++)
	{
		if (animation.PositionKeyFrames[i].time > time)
			return i - 1;
	}

	return animation.PositionKeyFrames.size() - 1;
}

glm::vec3 AnimationManager::GetAnimationPosition(const AnimationData& animation, float time)
{
	if (animation.PositionKeyFrames.size() == 1)
		return animation.PositionKeyFrames[0].value;

	int positionKeyFrameIndex = FindPositionKeyFrameIndex(animation, time);

	if (animation.PositionKeyFrames.size() - 1 == positionKeyFrameIndex)
		return animation.PositionKeyFrames[positionKeyFrameIndex].value;

	int nextPositionKeyFrameIndex = positionKeyFrameIndex + 1;
	PositionKeyFrame positionKeyFrame = animation.PositionKeyFrames[positionKeyFrameIndex];
	PositionKeyFrame nextPositionKeyFrame = animation.PositionKeyFrames[nextPositionKeyFrameIndex];
	float difference = nextPositionKeyFrame.time - positionKeyFrame.time;
	float ratio = (time - positionKeyFrame.time) / difference;


	switch (positionKeyFrame.type)
	{
	case EaseIn:
		ratio = glm::sineEaseIn(ratio);
		break;

	case EaseOut:
		ratio = glm::sineEaseOut(ratio);
		break;

	case EaseInOut:
		ratio = glm::sineEaseInOut(ratio);
		break;

	case None:
	default:
		break;
	}
	glm::vec3 result = glm::mix(positionKeyFrame.value, nextPositionKeyFrame.value, ratio);

	return result;
}
