#ifndef _SCENE_H_
#define _SCENE_H_

class BGObject;
class Character;

class Scene
{
private:
	BGObject*		m_testObj;
	Character*		m_testCharacter;

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