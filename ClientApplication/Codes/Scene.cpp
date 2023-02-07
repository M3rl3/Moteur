#include "Scene.h"
#include "JsonParser.h"

#include <vector>


using namespace std;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Update(const float& dt)
{
}

void Scene::Render()
{
}

void Scene::Destroy()
{
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
		// Layer?
		// list - GameObject - MapObject
	}
}
