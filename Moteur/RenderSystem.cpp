#include "RenderSystem.h"

#include <glm/glm.hpp>
#include <iostream>

RenderSystem::RenderSystem()
{
    window = new Window();
	camera = new Camera();
}

RenderSystem::~RenderSystem() 
{

}

static void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void RenderSystem::CreateWindow(const char* title, const int width, const int height, bool fullScreen) {

    if (!glfwInit()) {
        std::cerr << "GLFW init failed." << std::endl;
        glfwTerminate();
        return;
    }

    if (fullScreen) {
        GLFWmonitor* currentMonitor = glfwGetPrimaryMonitor();

        const GLFWvidmode* mode = glfwGetVideoMode(currentMonitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        window->width = mode->width;
        window->height = mode->height;
        window->title = title;

        window->theWindow = glfwCreateWindow(window->width, window->height, window->title, currentMonitor, NULL);
    }
    else {
        window->width = width;
        window->height = height;
        window->title = title;

        window->theWindow = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
    }

    if (!window->theWindow) {
        std::cerr << "Window creation failed." << std::endl;
        glfwTerminate();
        return;
    }

    glfwSetWindowAspectRatio(window->theWindow, 16, 9);
}

void RenderSystem::Initialize() {

    const char* glsl_version = "#version 420";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // keyboard callback
    glfwSetKeyCallback(window->theWindow, KeyCallback);

    // mouse and scroll callback
    //glfwSetCursorPosCallback(window->theWindow, MouseCallBack);
    //glfwSetScrollCallback(window->theWindow, ScrollCallBack);
    //glfwSetMouseButtonCallback(window->theWindow, MouseButtonCallback);

    // capture mouse input
    glfwSetInputMode(window->theWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetErrorCallback(ErrorCallback);

    glfwMakeContextCurrent(window->theWindow);

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
        std::cerr << "Error: unable to obtain pocess address." << std::endl;
        return;
    }
    glfwSwapInterval(1); //vsync

    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
}

TransformComponent* RenderSystem::GetTransformComponent(const std::vector<Component*>& components)
{
    TransformComponent* component = nullptr;
    for (int j = 0; j < components.size(); j++) {
        component = dynamic_cast<TransformComponent*>(components[j]);

        if (component->GetType() == transformComponent) {
            return component;
        }
    }
    return nullptr;
}

void RenderSystem::Process(const std::vector<Entity*>& entities, float dt)
{
    TransformComponent* transformComponent;
    for (int i = 0; i < entities.size(); i++) {
        Entity* currentEntity = entities[i];

        transformComponent = GetTransformComponent(currentEntity->components);

    }
}

void RenderSystem::Shutdown()
{
    glfwDestroyWindow(window->theWindow);
    glfwTerminate();

    window->theWindow = nullptr;
    delete window->theWindow;

    window = nullptr;
    delete window;

    camera = nullptr;
    delete camera;

    exit(EXIT_SUCCESS);
}
