#include "RenderSystem.h"

#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

#include "LoadModel.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>

Camera* cam;
void ManageLights(unsigned int shaderID);

RenderSystem::RenderSystem()
{
    systemName = "RenderSystem";
    vaoManager = new cVAOManager();

    window = new Window();
	camera = new Camera();
    cam = camera;
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

    const float CAMERA_MOVE_SPEED = 1.f;
    if (key == GLFW_KEY_A)     // Left
    {
        cam->position.x -= CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_D)     // Right
    {
        cam->position.x += CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_W)     // Forward
    {
        cam->position.z += CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_S)     // Backwards
    {
        cam->position.z -= CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_Q)     // Down
    {
        cam->position.y -= CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_E)     // Up
    {
        cam->position.y += CAMERA_MOVE_SPEED;
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
    TransformComponent* transformComponent = nullptr;
    ShaderComponent* shaderComponent = nullptr;
    MeshComponent* meshComponent;
    
    for (int i = 0; i < entities.size(); i++) {
        Entity* currentEntity = entities[i];

        transformComponent = currentEntity->GetComponentByType<TransformComponent>();
        shaderComponent = currentEntity->GetComponentByType<ShaderComponent>();
        meshComponent = currentEntity->GetComponentByType<MeshComponent>();

        if (transformComponent != nullptr && shaderComponent != nullptr)
        {
            //MVP
            glm::mat4x4 model, view, projection;
            glm::vec3 upVector = glm::vec3(0.f, 1.f, 0.f);

            GLint modelLocaction = glGetUniformLocation(shaderComponent->shaderID, "Model");
            GLint viewLocation = glGetUniformLocation(shaderComponent->shaderID, "View");
            GLint projectionLocation = glGetUniformLocation(shaderComponent->shaderID, "Projection");
            GLint modelInverseLocation = glGetUniformLocation(shaderComponent->shaderID, "ModelInverse");

            ManageLights(shaderComponent->shaderID);

            float ratio;
            int width, height;
            glfwGetFramebufferSize(window->theWindow, &width, &height);
            ratio = width / (float)height;
            glViewport(0, 0, width, height);

            glEnable(GL_DEPTH_TEST);
            glCullFace(GL_BACK);
            //glEnable(GL_CULL_FACE);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            view = glm::lookAt(camera->position, camera->target, upVector);
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

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            {
                GLint useRGBAColourLocation = glGetUniformLocation(shaderComponent->shaderID, "useRGBAColour");

                GLint RGBAColourLocation = glGetUniformLocation(shaderComponent->shaderID, "RGBAColour");

                glm::vec4 color = glm::vec4(100.f, 100.f, 100.f, 1.f);

                glUniform4f(RGBAColourLocation, color.r, color.g, color.b, color.w);

                glUniform1f(useRGBAColourLocation, (GLfloat)GL_TRUE);

                GLint useIsTerrainMeshLocation = glGetUniformLocation(shaderComponent->shaderID, "bIsTerrainMesh");
                glUniform1f(useIsTerrainMeshLocation, (GLfloat)GL_FALSE);

                GLint bHasTextureLocation = glGetUniformLocation(shaderComponent->shaderID, "bHasTexture");
                glUniform1f(bHasTextureLocation, (GLfloat)GL_FALSE);

                GLint doNotLightLocation = glGetUniformLocation(shaderComponent->shaderID, "doNotLight");
                glUniform1f(doNotLightLocation, (GLfloat)GL_FALSE);

                GLint bIsSkyboxObjectLocation = glGetUniformLocation(shaderComponent->shaderID, "bIsSkyboxObject");
                glUniform1f(bIsSkyboxObjectLocation, (GLfloat)GL_FALSE);
            }

            std::string meshName = meshComponent->plyModel.meshName;

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            sModelDrawInfo modelInfo;
            if (vaoManager->FindDrawInfoByModelName(meshName, modelInfo)) {

                glBindVertexArray(modelInfo.VAO_ID);
                glDrawElements(GL_TRIANGLES, modelInfo.numberOfIndices, GL_UNSIGNED_INT, (void*)0);
                glBindVertexArray(0);
            }
            else {
                std::cout << "Model " << meshName << " not found in VAO." << std::endl;
            }

            std::stringstream ss;
            ss << " Camera: " << "(" << camera->position.x << ", " << camera->position.y << ", " << camera->position.z << ")";

            glfwSetWindowTitle(window->theWindow, ss.str().c_str());

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

bool RenderSystem::LoadMesh(std::string fileName, std::string modelName, sModelDrawInfo& plyModel, unsigned int shaderID)
{
    LoadModel(fileName, plyModel);

    if (vaoManager->LoadModelIntoVAO(modelName, plyModel, shaderID)) {
        std::cout << "Model " << modelName << " loaded successfully." << std::endl;
        return true;
    }
    else {
        std::cout << "Could not load model " << modelName << " into VAO" << std::endl;
        return false;
    }
}

Window* RenderSystem::GetWindow() {
    return window;
}

Camera* RenderSystem::GetCamera()
{
    return camera;
}

void RenderSystem::SetCameraPosition(glm::vec3 pos)
{
    camera->position = pos;
}

void RenderSystem::SetCameraTarget(glm::vec3 cameraTarget)
{
    camera->target = cameraTarget;
}

void ManageLights(unsigned int shaderID) {

    GLint PositionLocation = glGetUniformLocation(shaderID, "sLightsArray[0].position");
    GLint DiffuseLocation = glGetUniformLocation(shaderID, "sLightsArray[0].diffuse");
    GLint SpecularLocation = glGetUniformLocation(shaderID, "sLightsArray[0].specular");
    GLint AttenLocation = glGetUniformLocation(shaderID, "sLightsArray[0].atten");
    GLint DirectionLocation = glGetUniformLocation(shaderID, "sLightsArray[0].direction");
    GLint Param1Location = glGetUniformLocation(shaderID, "sLightsArray[0].param1");
    GLint Param2Location = glGetUniformLocation(shaderID, "sLightsArray[0].param2");

    //glm::vec3 lightPosition0 = meshArray[1]->position;
    glm::vec3 lightPosition0 = glm::vec3(0.f, 5.f, 0.f);
    glUniform4f(PositionLocation, lightPosition0.x, lightPosition0.y, lightPosition0.z, 1.0f);
    //glUniform4f(PositionLocation, 0.f, 0.f, 0.f, 1.0f);
    glUniform4f(DiffuseLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(SpecularLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(AttenLocation, 0.5f, 0.01f, 0.0f, 1.f);
    glUniform4f(DirectionLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(Param1Location, 0.f, 0.f, 0.f, 1.f); //x = Light Type
    glUniform4f(Param2Location, 1.f, 0.f, 0.f, 1.f); //x = Light on/off
}