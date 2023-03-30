#pragma once

#include <GLFW/glfw3.h>

class cRenderReticle {
public:

	// Constructor
	cRenderReticle();

	// Destructor
	~cRenderReticle();

	// Init DearImGui
	void Initialize(GLFWwindow* window, const char* glsl_version);

	// Update method that runs every tick
	void Update();

	// Gracefully close everything down
	void Shutdown();
};