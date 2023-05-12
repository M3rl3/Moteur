#pragma once

#include <ECS/System.h>
#include <ECS/Entity.h>
#include <vector>

#include "../Components/MeshComponent.h"
#include "../Components/PlayerComponent.h"

#include "../Window/Window.h"

class GUISystem : public System {
public:
	// Constructor
	GUISystem();
	// Destructor
	~GUISystem();

	// Init DearImGui
	void Initialize(Window* window);

	// Update method called every tick
	void Process(const std::vector<Entity*>& entities, float dt);

	// Gracefully close everything down
	void Shutdown();

	// Whether the crosshair is to be drawn or no
	bool drawReticle;

private:

	// name of the system
	std::string systemName;

	// ListBox stuff
	int index = 0;
	bool doOnce = true;

	// store the names of the meshes
	std::vector<std::string> meshNames;

	MeshComponent* GetPlayerMesh(const std::vector<Entity*>& entities);
	PlayerComponent* GetPlayerComponent(const std::vector<Entity*>& entities);

	MeshComponent* GetEnemyMesh(const std::vector<Entity*>& entities, std::string meshName);
};