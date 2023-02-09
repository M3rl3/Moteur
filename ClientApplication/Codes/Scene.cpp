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
	: m_testCharacter(nullptr)
{
	m_vecBGObjs.clear();
}

Scene::~Scene()
{
}

void Scene::Update(const float& dt)
{
	for (int i = 0; i < m_vecBGObjs.size(); ++i)
		m_vecBGObjs[i]->Update(dt);

	if (m_testCharacter)
		m_testCharacter->Update(dt);
}

void Scene::Render()
{
}

void Scene::Destroy()
{
	for (int i = 0; i < m_vecBGObjs.size(); ++i)
		m_vecBGObjs[i]->Destroy();
	m_vecBGObjs.clear();

	if (m_testCharacter)
		m_testCharacter->Destroy();

	delete this;
}

void Scene::Ready()
{
	//Add.BGObjects
	string path = "..\\assets\\Json\\";
	string fileName = "mapObjects.json";

	vector<CJsonParser::sSampleData> vecMapObjs;
	CJsonParser::GetInstance()->LoadSampleFunction(path, fileName, vecMapObjs);
	for (int i = 0; i < vecMapObjs.size(); ++i)
	{
		CJsonParser::sSampleData curData = vecMapObjs[i];
		BGObject* newObj = Factory::CreateBGObject(
			curData.MODELPATH, curData.MODELNAME, curData.POSITION, curData.COLOR);
		if (nullptr != newObj)
			m_vecBGObjs.push_back(newObj);
	}

	//Add.Character
	m_testCharacter = Factory::CreateCharacter(
		"../assets/meshes/steve.ply", 
		"steve", 
		vec3(0.f, 0.f, 0.f));

}
