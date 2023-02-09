#pragma once

#include "OpenGL.h"

struct Window {
	Window(): theWindow(nullptr), width(NULL), height(NULL), title("") {}

	GLFWwindow* theWindow;

	int width;
	int height;
	const char* title;
};