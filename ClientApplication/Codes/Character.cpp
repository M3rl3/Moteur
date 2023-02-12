#include "Character.h"
#include "Moteur.h"
#include <iostream>

using namespace std;
using namespace glm;

Character::Character()
	: m_pMesh(nullptr), m_vRot(vec3(0.f))
{
}

Character::~Character()
{
}

void Character::Destroy()
{
	delete this;
}

void Character::Update(const float& dt)
{
	/*m_vRot.y += dt * 100.f;

	if (m_pMesh)
	{
		vec3 vEuler = vec3(radians(m_vRot.x), radians(m_vRot.y), radians(m_vRot.z));
		m_pMesh->SetRotationFromEuler(vEuler);
	}*/
}

bool Character::Ready(string modelPath, string modelName, vec3 position, vec4 color, Animation animation)
{
	int modelID = 0;
	Moteur::Engine_LoadModel(modelID, modelPath.c_str(), modelName.c_str(), false,
		position, color);

	if (-1 == modelID)
		return false;

	m_pMesh = Moteur::Engine_GetMeshObjectFromVector(modelID);
	m_pMesh->enabled = true;
	m_pMesh->animation.AnimationTime = animation.AnimationTime;
	m_pMesh->animation.IsPlaying = animation.IsPlaying;
	m_pMesh->animation.Speed = animation.Speed;
	m_pMesh->animation.AnimationType = animation.AnimationType;

}

Character* Character::Create(string modelPath, string modelName, vec3 position, vec4 color, Animation animation)
{
	Character* pInstance = new Character();
	if (!pInstance->Ready(modelPath, modelName, position, color, animation))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}
	return pInstance;
}