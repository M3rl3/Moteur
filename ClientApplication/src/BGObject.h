#ifndef _BGOBJECT_H_
#define _BGOBJECT_H_

#include <string>
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"

/// <summary>
/// The mesh class
/// </summary>
class cMeshInfo;

/// <summary>
/// The object class
/// </summary>
class BGObject
{
private:
	// List of meshes to be loaded
	cMeshInfo*		m_pMesh;

private:
	// Constructor
	explicit BGObject();
	// Destructor
	~BGObject();

public:
	//Cleanup method
	void Destroy();
	//Update scene at delta time.
	void Update(const float& dt);

public:
	// Set Euler rotation for mesh object
	void SetRotation(glm::vec3 vRot);
	// Set scale for object
	void SetScale(glm::vec3 vScale);

private:
	// Load textures and meshes
	bool Ready(std::string modelName, std::string texName, glm::vec3 position);
public:
	// Load textures and meshes
	static BGObject* Create(std::string modelName, std::string texName, glm::vec3 position);
};

#endif //_BGOBJECT_H_