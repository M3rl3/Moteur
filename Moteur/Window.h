#pragma once

#include "OpenGL.h"

struct Window {

	GLFWwindow* theWindow;

	int width;
	int height;
	const char* title;
};