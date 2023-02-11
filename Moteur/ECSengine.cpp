#include "ECSengine.h"

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
	renderSystem->Process(entityManager.GetEntities(), 0.f);
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
		printf("System already added!\n");
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
		printf("System is not added!\n");
		return;
	}

	systems.erase(itFind);
}

