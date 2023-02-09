#pragma once

#include <ECS/System.h>

#include "Window.h"
#include "Camera.h"

class RenderSystem : System {
public:

	RenderSystem();
	~RenderSystem();

	void CreateWindow(const char* title, const int width, const int height, bool fullScreen);
	void Initialize();
	void Process(const std::vector<Entity*>& entities, float dt);
	void Shutdown();

	Window* window;

private:

	Camera* camera;
};