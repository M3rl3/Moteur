#ifndef _BGOBJECT_H_
#define _BGOBJECT_H_

#include <string>
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"

class cMeshInfo;

class BGObject
{
private:
	cMeshInfo*		m_pMesh;

private:
	explicit BGObject();
	~BGObject();

public:
	void Destroy();
	void Update(const float& dt);

public:
	void SetRotation(glm::vec3 vRot);
	void SetScale(glm::vec3 vScale);

private:
	bool Ready(std::string modelName, std::string texName, glm::vec3 position);
public:
	static BGObject* Create(std::string modelName, std::string texName, glm::vec3 position);
};

#endif //_BGOBJECT_H_