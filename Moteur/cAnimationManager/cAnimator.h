#pragma once

#include "cAnimation.h"

class Animator
{
public:
    Animator(Animation* animation);

    void UpdateAnimation(float dt);
    void PlayAnimation(Animation* pAnimation, bool resetAnimationTime = true);
    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

    const std::vector<glm::mat4>& GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation = nullptr;
    Animation* m_PreviousAnimation = nullptr;
    float transitionTime = 0.f;
    float m_CurrentTime;
};