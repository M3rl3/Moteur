#include "ECSengine.h"
#include "cVAOManager/cVAOManager.h"

#include <iostream>

ECSengine::ECSengine()
{
	entityManager = new EntityManager();
	renderSystem = new RenderSystem();
}

ECSengine::~ECSengine() 
{
}

void ECSengine::Shutdown()
{
	
}

void ECSengine::Update(float dt) {
	for (int i = 0; i < systems.size(); i++) {
		systems[i]->Process(entityManager->GetEntities(), dt);
	}
}

void ECSengine::Render()
{
	//renderSystem->Process(entityManager.GetEntities(), 0.f);
}

unsigned int ECSengine::CreateEntity()
{
	return entityManager->CreateEntity();
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

