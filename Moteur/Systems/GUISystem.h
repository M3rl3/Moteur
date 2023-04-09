#pragma once

#include <ECS/System.h>
#include <vector>

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
};