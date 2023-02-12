#include "RenderSystem.h"

#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

RenderSystem::RenderSystem()
{
    systemName = "RenderSystem";
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

void RenderSystem::Initialize(const char* title, const int width, const int height, bool fullScreen) {

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
    //glfwSetInputMode(window->theWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

void RenderSystem::Process(const std::vector<Entity*>& entities, float dt)
{
    TransformComponent* transformComponent;
    ShaderComponent* shaderComponent;
    //MeshComponent* meshComponent;

    for (int i = 0; i < entities.size(); i++) {
        Entity* currentEntity = entities[i];

        transformComponent = dynamic_cast<TransformComponent*>(currentEntity->GetComponentByType("TransformComponent"));
        shaderComponent = dynamic_cast<ShaderComponent*>(currentEntity->GetComponentByType("ShaderComponent"));
        //meshComponent = dynamic_cast<MeshComponent*>(currentEntity->GetComponentByType("MeshComponent"));
        
        if (transformComponent != nullptr && shaderComponent != nullptr)
        {
            //MVP
            glm::mat4x4 model, view, projection;
            glm::vec3 upVector = glm::vec3(0.f, 1.f, 0.f);

            GLint modelLocaction = glGetUniformLocation(shaderComponent->shaderID, "Model");
            GLint viewLocation = glGetUniformLocation(shaderComponent->shaderID, "View");
            GLint projectionLocation = glGetUniformLocation(shaderComponent->shaderID, "Projection");
            GLint modelInverseLocation = glGetUniformLocation(shaderComponent->shaderID, "ModelInverse");

            float ratio;
            int width, height;
            glfwGetFramebufferSize(window->theWindow, &width, &height);
            ratio = width / (float)height;
            glViewport(0, 0, width, height);

            glEnable(GL_DEPTH_TEST);
            glCullFace(GL_BACK);
            //glEnable(GL_CULL_FACE);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            view = glm::lookAt(camera->position, camera->position + camera->target, upVector);
            projection = glm::perspective(0.6f, ratio, 0.1f, 10000.f);

            glm::vec4 viewport = glm::vec4(0, 0, width, height);

            GLint eyeLocationLocation = glGetUniformLocation(shaderComponent->shaderID, "eyeLocation");
            glUniform4f(eyeLocationLocation, camera->position.x, camera->position.y, camera->position.z, 1.f);

            model = glm::mat4x4(1.f);
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), transformComponent->position);
            glm::mat4 scaling = glm::scale(glm::mat4(1.f), transformComponent->scale);
            glm::mat4 rotation = glm::mat4(transformComponent->rotation);

            model *= translationMatrix;
            model *= rotation;
            model *= scaling;

            glUniformMatrix4fv(modelLocaction, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

            glm::mat4 modelInverse = glm::inverse(glm::transpose(model));
            glUniformMatrix4fv(modelInverseLocation, 1, GL_FALSE, glm::value_ptr(modelInverse));

            glfwSwapBuffers(window->theWindow);
            glfwPollEvents();
        }
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
