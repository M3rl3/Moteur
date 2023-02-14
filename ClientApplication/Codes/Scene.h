#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

// Represents static objects that can be loaded to seen
class BGObject;

// The game characters
class Character;

// The game world
class Scene
{
private:
	// List of objects to add to scene
	std::vector<BGObject*>	m_vecBGObjs;

	// The game character
	Character*				m_testCharacter;

public:
	// Constructor
	explicit Scene();
private:
	// Destructor
	~Scene();

public:
	// Update scene at delta time.
	void Update(const float& dt);

	// Display scene
	void Render();

	// Delete scene at the end
	void Destroy();

public:
	// Load textures and meshes
	void Ready();
};

#endif //_SCENE_H_