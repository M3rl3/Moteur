#ifndef _FACTORY_H_
#define _FACTORY_H_

#include <string>
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"

class BGObject;
class Character;

class Factory
{
private:
	explicit Factory();
	~Factory();

public:
	static BGObject* CreateBGObject(std::string modelPath, std::string modelName, glm::vec3 position);
	static Character* CreateCharacter(std::string modelPath, std::string modelName, glm::vec3 position, glm::vec4 color = glm::vec4(1.f));
};

#endif //_FACTORY_H_