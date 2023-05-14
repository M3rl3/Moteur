#include "cAnimator.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>

Animator::Animator(Animation* animation)
{
    m_CurrentTime = 0.0f;
    m_CurrentAnimation = animation;

    m_FinalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++)
        m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}

void Animator::UpdateAnimation(float dt)
{
    if (m_CurrentAnimation != nullptr)
    {
        m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
        CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
}

void Animator::PlayAnimation(Animation* pAnimation, bool resetAnimationTime)
{
    if (m_CurrentAnimation != pAnimation) {
        transitionTime = 0.5f;

        if (m_PreviousAnimation != m_CurrentAnimation) {
            m_PreviousAnimation = m_CurrentAnimation;
            m_CurrentTime = 0.0f;
        }
        m_CurrentAnimation = pAnimation;
    }

    if (resetAnimationTime) {
        m_CurrentTime = 0.0f;
    }
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* bone = m_CurrentAnimation->FindBone(nodeName);

    if (bone)
    {
        bone->Update(m_CurrentTime);
        nodeTransform = bone->GetLocalTransform();

        if (m_PreviousAnimation != nullptr && transitionTime > 0.0f)
        {
            Bone* bone2 = m_PreviousAnimation->FindBone(nodeName);
            // Calculate the position of this node.

            glm::vec3 position, scale, temp1;
            glm::vec4 temp2;
            glm::quat rotation;
            glm::decompose(nodeTransform, scale, rotation, position, temp1, temp2);

            glm::mat4 transform2 = bone2->GetLocalTransform();
            glm::vec3 position2, scale2;
            glm::quat rotation2;
            glm::decompose(transform2, scale2, rotation2, position2, temp1, temp2);

            float currRatio = 1.0f - transitionTime;
            float prevRatio = transitionTime;

            position = position * currRatio + position2 * prevRatio;
            scale = scale * currRatio + scale2 * prevRatio;
            rotation = glm::slerp(rotation2, rotation, currRatio);

            // Calculate our transformation matrix
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
            glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

            nodeTransform = translationMatrix * rotationMatrix * scaleMatrix;
        }
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        m_FinalBoneMatrices[index] = globalTransformation * offset;
    }

    /*glm::quat oneEighty = glm::quat(glm::vec3(0.f, glm::radians(180.f), 0.f));
    glm::mat4 rotationMatrix = glm::toMat4(oneEighty);

    {
        std::string boneName = "mixamorig:Head";

        if (boneInfoMap.find(boneName) != boneInfoMap.end()) {
            int boneIndex = boneInfoMap[boneName].id;
            m_FinalBoneMatrices[boneIndex] = m_FinalBoneMatrices[boneIndex] * rotationMatrix;
        }
    }*/

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);
}
