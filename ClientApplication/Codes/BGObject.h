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

private:
	bool Ready(std::string modelPath, std::string modelName, glm::vec3 position, glm::vec4 color);
public:
	static BGObject* Create(std::string modelPath, std::string modelName, glm::vec3 position, glm::vec4 color);
};

#endif //_BGOBJECT_H_