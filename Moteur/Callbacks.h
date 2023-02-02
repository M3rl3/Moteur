#pragma once

#include "OpenGL.h"
#include "GlobalStuff.h"
#include <fstream>

enum eEditMode
{
    MOVING_CAMERA,
    MOVING_LIGHT,
    MOVING_SELECTED_OBJECT,
    TAKE_CONTROL,
};

extern glm::vec3 cameraEye; //loaded from external file
//glm::vec3 cameraTarget = glm::vec3(-75.0f, 2.0f, 0.0f);

// now controlled by mouse!
extern glm::vec3 cameraTarget;

extern float yaw;
extern float pitch;
extern float fov;

// mouse state
extern bool firstMouse;
extern float lastX;
extern float lastY;

extern cMeshInfo* player_mesh;
extern bool enableMouse;

extern eEditMode theEditMode;
extern int object_index;

extern std::vector <cMeshInfo*> meshArray;

void ReadSceneDescription();

void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseCallBack(GLFWwindow* window, double xposition, double yposition);
void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);