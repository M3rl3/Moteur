#include "Scene.h"
#include "JsonParser/JsonParser.h"
#include "Factory.h"
#include "BGObject.h"
#include "Character.h"
#include "glm\vec3.hpp"
#include "OldEngine/Moteur.h"

#include <vector>

using namespace std;
using namespace glm;

/// <summary>
/// The constructor
/// </summary>
Scene::Scene()
	: m_testCharacter(nullptr)
{
	m_vecBGObjs.clear();
}

/// <summary>
/// The destructor
/// </summary>
Scene::~Scene()
{
}

/// <summary>
/// Update scene at delta time.
/// </summary>
/// <param name="dt"> The delta time</param>
void Scene::Update(const float& dt)
{
	for (int i = 0; i < m_vecBGObjs.size(); ++i)
		m_vecBGObjs[i]->Update(dt);

	if (m_testCharacter)
		m_testCharacter->Update(dt);
}

/// <summary>
/// Display scene
/// </summary>
void Scene::Render()
{
}

/// <summary>
/// Cleanup method
/// </summary>
void Scene::Destroy()
{
	for (int i = 0; i < m_vecBGObjs.size(); ++i)
		m_vecBGObjs[i]->Destroy();
	m_vecBGObjs.clear();

	if (m_testCharacter)
		m_testCharacter->Destroy();

	delete this;
}

/// <summary>
/// Load textures and meshes
/// </summary>
void Scene::Ready()
{
	//Texture
	Moteur::Engine_SetTexturePath("../assets/texture_bg");
	Moteur::Engine_Create2DTextureFromBMPFile("barrel_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("beam_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("ceiling_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("chest_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("chest_B_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("door_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("floor_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("lantern_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("moss.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("mud_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("mushroom_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("pillar_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("pot_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("pot_B_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("rock_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("skull_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("spiderweb.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("table_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("torch_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("wall_A_d.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("alpha.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("albedo.bmp");
	Moteur::Engine_Create2DTextureFromBMPFile("specular.bmp");

	//Add.BGObjects
	string path = "..\\assets\\Json\\";
	string fileName = "mapObjects.json";

	vector<CJsonParser::sSampleData> vecMapObjs;
	CJsonParser::GetInstance()->LoadSampleFunction(path, fileName, vecMapObjs);
	for (int i = 0; i < vecMapObjs.size(); ++i)
	{
		CJsonParser::sSampleData curData = vecMapObjs[i];
		BGObject* newObj = Factory::CreateBGObject(
			curData.MODELNAME, curData.TEXNAME, curData.POSITION);
		if (nullptr != newObj)
		{
			newObj->SetRotation(curData.ROTATION);
			newObj->SetScale(curData.SCALE);
			m_vecBGObjs.push_back(newObj);
		}
	}

	//Add.Character
	Animation animation = Animation();
	//animation.enabled = true;
	animation.AnimationTime = 0.0f;
	animation.IsPlaying = true;
	animation.Speed = 0.2f;
	animation.AnimationType = "TestAnimation";
	m_testCharacter = Factory::CreateCharacter(
		"../assets/meshes/female_warrior.ply",
		"female_warrior",
		vec3(10.f, 0.f, -20.f), 
		animation);

}
