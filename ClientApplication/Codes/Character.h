#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <string>
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"

class cMeshInfo;

class Character
{
private:
	cMeshInfo*		m_pMesh;
	glm::vec3		m_vRot;

private:
	explicit Character();
	~Character();

public:
	void Destroy();
	void Update(const float& dt);

private:
	bool Ready(std::string modelPath, std::string modelName, glm::vec3 position, glm::vec4 color);
public:
	static Character* Create(std::string modelPath, std::string modelName, glm::vec3 position, glm::vec4 color);
};

#endif //_CHARACTER_H_