#pragma once

#include <ECS/System.h>

#include "TransformComponent.h"

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

	TransformComponent* GetTransformComponent(const std::vector<Component*>& components);

	Camera* camera;

	std::string transformComponent = "TransformComponent";
};