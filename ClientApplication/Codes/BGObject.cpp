#include "BGObject.h"
#include "Engine.h"
#include "cMeshInfo.h"

#include <sstream>

using namespace std;
using namespace glm;

BGObject::BGObject()
	: m_pMesh(nullptr)
{
}

BGObject::~BGObject()
{
}

void BGObject::Destroy()
{
	delete this;
}

void BGObject::Update(const float& dt)
{
}

void BGObject::SetRotation(glm::vec3 vRot)
{
	vec3 vEuler = vec3(radians(vRot.x), radians(vRot.y), radians(vRot.z));
	m_pMesh->SetRotationFromEuler(vEuler);
}

void BGObject::SetScale(glm::vec3 vScale)
{
	m_pMesh->scale = vScale;
}

bool BGObject::Ready(string modelName, string texName, vec3 position)
{
	stringstream ss;
	ss << "../assets/mesh_bg/" + modelName + ".ply";

	int modelID = 0;
	Engine::Engine_LoadModel(modelID, ss.str().c_str(), modelName.c_str(), false,
		position, vec4(1.f));

	if (-1 == modelID)
		return false;

	m_pMesh = Engine::Engine_GetMeshObjectFromVector(modelID);
	m_pMesh->textures[0] = texName.c_str();
	m_pMesh->textureRatios[0] = 1.f;
	m_pMesh->hasTexture = true;
	m_pMesh->useRGBAColour = false;
}

BGObject* BGObject::Create(string modelName, string texName, vec3 position)
{
	BGObject* pInstance = new BGObject();
	if (!pInstance->Ready(modelName, texName, position))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}
	return pInstance;
}
