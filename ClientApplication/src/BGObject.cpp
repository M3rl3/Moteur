#include "BGObject.h"
#include "Moteur.h"
#include "cMeshInfo.h"

#include <sstream>

using namespace std;
using namespace glm;

/// <summary>
/// The constructor
/// </summary>
BGObject::BGObject()
	: m_pMesh(nullptr)
{
}

/// <summary>
/// The destructor
/// </summary>
BGObject::~BGObject()
{
}

/// <summary>
/// Cleanup method
/// </summary>
void BGObject::Destroy()
{
	delete this;
}

/// <summary>
/// Update scene at delta time.
/// </summary>
/// <param name="dt">The delta time</param>
void BGObject::Update(const float& dt)
{
}

/// <summary>
/// Set Euler rotation for mesh object
/// </summary>
/// <param name="vRot">Vector3 of new angle</param>
void BGObject::SetRotation(glm::vec3 vRot)
{
	vec3 vEuler = vec3(radians(vRot.x), radians(vRot.y), radians(vRot.z));
	m_pMesh->SetRotationFromEuler(vEuler);
}

/// <summary>
/// Set scale for object
/// </summary>
/// <param name="vScale">Vector3 of new scale</param>
void BGObject::SetScale(glm::vec3 vScale)
{
	m_pMesh->scale = vScale;
}

/// <summary>
/// Load textures and meshes
/// </summary>
/// <param name="modelName">model name</param>
/// <param name="texName"> text name</param>
/// <param name="position">The position where object will be placed</param>
/// <returns></returns>
bool BGObject::Ready(string modelName, string texName, vec3 position)
{
	stringstream ss;
	ss << "../assets/mesh_bg/" + modelName + ".ply";

	int modelID = 0;
	Moteur::Engine_LoadModel(modelID, ss.str().c_str(), modelName.c_str(), false,
		position, vec4(1.f));

	if (-1 == modelID)
		return false;

	m_pMesh = Moteur::Engine_GetMeshObjectFromVector(modelID);
	m_pMesh->textures[0] = texName.c_str();
	m_pMesh->textureRatios[0] = 1.f;
	m_pMesh->hasTexture = true;
	m_pMesh->useRGBAColour = false;
}

/// <summary>
/// Create instance of an object
/// </summary>
/// <param name="modelName">Model name</param>
/// <param name="texName">The text name</param>
/// <param name="position">The position where object will be placed</param>
/// <returns></returns>
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
