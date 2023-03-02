#include "ECSengine.h"
#include "cVAOManager/cVAOManager.h"

#include <iostream>

// Constructor
ECSengine::ECSengine()
{	
	// Init entity manager to add/remove components
	entityManager = new EntityManager();
	UpdateCallBack = NULL;
}

// Destructor
ECSengine::~ECSengine() 
{
}

// Calls update on all systems currently present every tick
void ECSengine::Update(float dt) {

	for (int i = 0; i < systems.size(); i++) {
		System* currentSystem = systems[i];
		currentSystem->Process(entityManager->GetEntities(), dt);
	}
	// Execute the update callback
	if (UpdateCallBack != NULL) {
		UpdateCallBack(dt);
	}
}

void ECSengine::UpdateCallback(void(*Callback)(float dt))
{
	UpdateCallBack = Callback;
}

// Creates an entity and returns its ID
unsigned int ECSengine::CreateEntity()
{
	return entityManager->CreateEntity();
}

// Add a system to the array of systems
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

// Remove a system from the array of systems
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

// Calls shutdown on all added systems
void ECSengine::Shutdown()
{
	entityManager = nullptr;
	delete entityManager;

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->Shutdown();

		systems[i] = nullptr;
		delete systems[i];
	}
}