#pragma once

#include "OpenGL.h"

// Simple struct holding the window object and its size
struct Window {
	// Constructor
	Window(): 
		theWindow(nullptr), 
		width(NULL), 
		height(NULL), 
		title("") 
	{}

	// Destructor
	~Window() {}

	// The main window
	GLFWwindow* theWindow;

	// Window width
	int width;

	// Window height
	int height;

	// Window title
	const char* title;
};