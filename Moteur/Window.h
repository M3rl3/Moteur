#pragma once

#include "OpenGL.h"

// Simple struct holding the window object and its size
class Window {
public:
	// Constructor
	Window(): 
		theWindow(nullptr), 
		width(NULL), 
		height(NULL), 
		title("") 
	{
		for (size_t i = 0; i < 100; i++) {
			keyPressedID[i] = false;
		}
	}

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

	// boolean array for handling input
	bool keyPressedID[100];

	bool* GetKeyPressedArray() {
		return keyPressedID;
	}

	// keyboard callback
	static void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods) {

		Window* altWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (key > 0 && key < 100) {
			if (action == GLFW_PRESS) {
				altWindow->keyPressedID[key] = true;
			}
			else if (action == GLFW_RELEASE) {
				altWindow->keyPressedID[key] = false;
			}
		}
	}
};