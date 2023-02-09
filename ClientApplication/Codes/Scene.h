#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

class BGObject;
class Character;

class Scene
{
private:
	std::vector<BGObject*>	m_vecBGObjs;

	Character*				m_testCharacter;

public:
	explicit Scene();
private:
	~Scene();

public:
	void Update(const float& dt);
	void Render();
	void Destroy();

public:
	void Ready();
};

#endif //_SCENE_H_