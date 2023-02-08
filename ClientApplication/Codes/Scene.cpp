#include "Scene.h"
#include "JsonParser.h"
#include "Factory.h"
#include "BGObject.h"
#include "Character.h"
#include "glm\vec3.hpp"

#include <vector>

using namespace std;
using namespace glm;

Scene::Scene()
	: m_testObj(nullptr), m_testCharacter(nullptr)
{
}

Scene::~Scene()
{
}

void Scene::Update(const float& dt)
{
	if (m_testObj)
		m_testObj->Update(dt);

	if (m_testCharacter)
		m_testCharacter->Update(dt);
}

void Scene::Render()
{
}

void Scene::Destroy()
{
	if (m_testObj)
		m_testObj->Destroy();

	if (m_testCharacter)
		m_testCharacter->Destroy();

	delete this;
}

void Scene::Ready()
{
	string path = "..\\assets\\Json\\";
	string fileName = "mapObjects.json";

	vector<CJsonParser::sSampleData> vecMapObjs;
	CJsonParser::GetInstance()->LoadSampleFunction(path, fileName, vecMapObjs);
	for (int i = 0; i < vecMapObjs.size(); ++i)
	{

	}

	m_testObj = Factory::CreateBGObject(
		"../assets/meshes/steve.ply", 
		"steve", 
		vec3(0.f));

	m_testCharacter = Factory::CreateCharacter(
		"../assets/meshes/steve.ply", 
		"steve", 
		vec3(10.f, 0.f, 10.f));

}
