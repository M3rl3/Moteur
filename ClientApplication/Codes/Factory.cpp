#include "Factory.h"
#include "BGObject.h"
#include "Character.h"

using namespace std;
using namespace glm;

Factory::Factory()
{
}

Factory::~Factory()
{
}

BGObject* Factory::CreateBGObject(string modelPath, string modelName, vec3 position, vec4 color)
{
	return BGObject::Create(modelPath, modelName, position, color);
}

Character* Factory::CreateCharacter(string modelPath, string modelName, vec3 position, vec4 color)
{
	return Character::Create(modelPath, modelName, position, color);
}
