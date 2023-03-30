#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <string>
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"

#include <cAnimationManager/Animation.h>

// The mesh class
class cMeshInfo;

/// <summary>
/// The character class
/// </summary>
class Character
{
private:
	// List of meshes
	cMeshInfo*		m_pMesh;
	// Rotation
	glm::vec3		m_vRot;

private:
	// The constructor
	explicit Character();
	// The destructor
	~Character();

public:
	// The cleanup method
	void Destroy();
	// Update scene at delta time.
	void Update(const float& dt);

private:
	// Load meshe character
	bool Ready(std::string modelPath, std::string modelName, glm::vec3 position, glm::vec4 color, Animation animation);
public:
	// Create new character from mesh
	static Character* Create(std::string modelPath, std::string modelName, glm::vec3 position, glm::vec4 color, Animation animation);
};

#endif //_CHARACTER_H_