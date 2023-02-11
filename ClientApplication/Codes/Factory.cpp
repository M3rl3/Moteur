#include "Factory.h"
#include "BGObject.h"
#include "Character.h"
#include <Animation.h>

using namespace std;
using namespace glm;

Factory::Factory()
{
}

Factory::~Factory()
{
}

BGObject* Factory::CreateBGObject(string modelPath, string modelName, vec3 position)
{
	return BGObject::Create(modelPath, modelName, position);
}

Character* Factory::CreateCharacter(string modelPath, string modelName, vec3 position, Animation animation, vec4 color)
{
	return Character::Create(modelPath, modelName, position, color, animation);
}
