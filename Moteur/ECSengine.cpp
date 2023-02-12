#include "ECSengine.h"
#include "cVAOManager/cVAOManager.h"

#include <iostream>

ECSengine::ECSengine()
{
	renderSystem = new RenderSystem();
}

ECSengine::~ECSengine() 
{
}

void ECSengine::Initialize() {

	renderSystem->Initialize();
}

void ECSengine::CreateWindow(const char* title, const int width, const int height, bool fullScreen) {

	renderSystem->CreateWindow(title, width, height, fullScreen);
}

void ECSengine::CreateShaderProgramFromFiles(unsigned int& id, const char* vertShader, const char* fragShader)
{
	shaderSystem->CreateShaderProgramFromFiles(id, vertShader, fragShader);
}

void ECSengine::LoadModel(std::string fileName, std::string modelName, unsigned int shaderID)
{
	sModelDrawInfo plyModel;
	meshSystem->LoadMesh(fileName, modelName, plyModel, shaderID);
}

void ECSengine::Shutdown()
{
	renderSystem->Shutdown();
}

void ECSengine::Update(float dt) {
	for (int i = 0; i < systems.size(); i++) {
		systems[i]->Process(entityManager.GetEntities(), dt);
	}
}

void ECSengine::Render()
{
	//renderSystem->Process(entityManager.GetEntities(), 0.f);
}

unsigned int ECSengine::CreateEntity()
{
	return entityManager.CreateEntity();
}

Window* ECSengine::GetWindow()
{
	return renderSystem->window;
}

void ECSengine::AddSystem(System* system)
{
	std::vector<System*>::iterator itFind =
		std::find(systems.begin(), systems.end(), system);	

	if (itFind != systems.end())
	{
		std::cout << "System already exists." << std::endl;
		return;
	}

	systems.push_back(system);
}

void ECSengine::RemoveSystem(System* system)
{
	std::vector<System*>::iterator itFind =
		std::find(systems.begin(), systems.end(), system);

	if (itFind == systems.end())
	{
		std::cout << "System does not exist." << std::endl;
		return;
	}

	systems.erase(itFind);
}

