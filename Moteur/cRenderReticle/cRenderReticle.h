#pragma once

#include <GLFW/glfw3.h>
#include <map>

class cRenderReticle {
public:

	// Constructor
	cRenderReticle();

	// Destructor
	~cRenderReticle();

	// Init DearImGui
	void Initialize(GLFWwindow* window, const char* glsl_version);

	// Draw high score
	void DrawHighScoresWindow(bool* p_open, std::map<int32_t, int32_t>& highScores);

	// Update method that runs every tick
	void Update();

	// Gracefully close everything down
	void Shutdown();
};