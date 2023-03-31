#include "Factory.h"
#include "BGObject.h"
#include "Character.h"

#include <cAnimationManager/Animation.h>

using namespace std;
using namespace glm;

// The constructor
Factory::Factory()
{
}

// The destructor
Factory::~Factory()
{
}

/// <summary>
/// The create object function
/// </summary>
/// <param name="modelPath">model path</param>
/// <param name="modelName">model name</param>
/// <param name="position">position</param>
/// <returns></returns>
BGObject* Factory::CreateBGObject(string modelPath, string modelName, vec3 position)
{
	return BGObject::Create(modelPath, modelName, position);
}

/// <summary>
/// The create character method
/// </summary>
/// <param name="modelPath">model path</param>
/// <param name="modelName">model name</param>
/// <param name="position">position</param>
/// <param name="animation">character animation</param>
/// <param name="color">the color</param>
/// <returns></returns>
Character* Factory::CreateCharacter(string modelPath, string modelName, vec3 position, Animation animation, vec4 color)
{
	return Character::Create(modelPath, modelName, position, color, animation);
}
