#ifndef _FACTORY_H_
#define _FACTORY_H_

#include <string>
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"

// The static object class
class BGObject;
// The character class
class Character;
// The animation class
class Animation;

/// <summary>
/// Factor class for loading meshes
/// </summary>
class Factory
{
private:
	// Constructor
	explicit Factory();
	//destructor
	~Factory();

public:
	// The create object method
	static BGObject* CreateBGObject(std::string modelPath, std::string modelName, glm::vec3 position);
	// The create character method
	static Character* CreateCharacter(
		std::string modelPath, 
		std::string modelName, 
		glm::vec3 position, 
		Animation animation,
		glm::vec4 color = glm::vec4(1.f));
};

#endif //_FACTORY_H_