#include "BGObject.h"
#include "Engine.h"
#include "cMeshInfo.h"

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

bool BGObject::Ready(string modelPath, string modelName, vec3 position, vec4 color)
{
	int modelID = 0;
	Engine::Engine_LoadModel(modelID, modelPath.c_str(), modelName.c_str(), false,
		position, color);

	if (-1 == modelID)
		return false;

	m_pMesh = Engine::Engine_GetMeshObjectFromVector(modelID);
}

BGObject* BGObject::Create(string modelPath, string modelName, vec3 position, vec4 color)
{
	BGObject* pInstance = new BGObject();
	if (!pInstance->Ready(modelPath, modelName, position, color))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}
	return pInstance;
}
