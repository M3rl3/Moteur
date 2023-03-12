#include "RenderSystem.h"

#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "AnimationComponent.h"
#include "BoundingBoxComponent.h"
#include "TextureComponent.h"

#include "DrawBoundingBox.h"

#include "LoadModel.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>

// Global variables
Camera* cam;
AnimationManager* animeMan;

float yaw = 0.f;
float pitch = 0.f;
float fov = 45.f;

// mouse state
bool firstMouse = true;
float lastX = 800.f / 2.f;
float lastY = 600.f / 2.f;

bool enableMouse = false;
bool mouseClick = false;

glm::vec3 targetLoc = glm::vec3(0.f);

// Boolean arrays for handling user input
bool keyPressedID[255];
bool lastKeyPressedID[255];

// Function for managing all the lights in the scene
void ManageLights(unsigned int shaderID);

// Constructor
RenderSystem::RenderSystem()
{
    systemName = "RenderSystem";

    // Initialize all the managers
    vaoManager = new cVAOManager();
    textureManager = new cBasicTextureManager();
    plyFileLoader = new cPlyFileLoader();
    animationManager = new AnimationManager();

    // Initialize all the structs
    window = new Window();
	camera = new Camera();
    cam = camera;
    animeMan = animationManager;

    ambientLight = 1.f;
}

// Destructor
RenderSystem::~RenderSystem() 
{

}

// If something goes wrong
static void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// Keyboard inputs
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

    /*if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) {
        enableMouse = !enableMouse;
    }*/
}

// keyboard callback
void RenderSystem::ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key > 0 && key < 255) {
        if (action == GLFW_PRESS) {
            keyPressedID[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keyPressedID[key] = false;
        }
    }
}

// Callback for panning camera with the mouse
void MouseCallBack(GLFWwindow* window, double xposition, double yposition) {

    if (firstMouse) {
        lastX = xposition;
        lastY = yposition;
        firstMouse = false;
    }

    float xoffset = xposition - lastX;
    float yoffset = lastY - yposition;  // reversed since y coordinates go from bottom to up
    lastX = xposition;
    lastY = yposition;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // prevent perspective from getting flipped by capping it
    if (pitch > 89.f) {
        pitch = 89.f;
    }
    if (pitch < -89.f) {
        pitch = -89.f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    if (enableMouse) {
        cam->target = glm::normalize(front);
    }
}

// Callback for zooming in with the mouse scroll wheel
void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
    if (fov >= 1.f && fov <= 45.f) {
        fov -= yoffset;
    }
    if (fov <= 1.f) {
        fov = 1.f;
    }
    if (fov >= 45.f) {
        fov = 45.f;
    }
}

// Callback for mouse buttons
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {

        // quick cleanup
        if (animeMan != nullptr) {
            animeMan = nullptr;
            delete animeMan;
        }

        float ratio;
        int rWidth, rHeight;

        glfwGetFramebufferSize(window, &rWidth, &rHeight);
        ratio = rWidth / (float)rHeight;

        glm::vec3 upVector = glm::vec3(0.f, 1.f, 0.f);
        glm::mat4x4 projection = glm::perspective(0.6f, ratio, 0.1f, 10000.0f);
        glm::mat4x4 view = glm::lookAt(cam->position, cam->target, upVector);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float x = (float)xpos / width * 2 - 1;
        float y = -((float)ypos / height * 2 - 1);

        glm::vec4 ray_clip(x, y, -1.0, 1.0);

        glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
        ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
        glm::vec3 ray_world = glm::vec3(glm::inverse(view) * ray_eye);

        ray_world = glm::normalize(ray_world);
        float t = -(cam->position.y) / ray_world.y;

        // Get target location
        glm::vec3 targetLocation = cam->position + t * ray_world;
        std::cout << "Mouse left click at position: " << targetLocation.x << ", " << targetLocation.y << ", " << targetLocation.z << std::endl;

        animeMan = new AnimationManager();

        targetLoc = targetLocation;

        /*for (cMeshInfo* meshInfo : meshArray) {
            if (meshInfo->enabled) {
                AnimationData testAnimation;
                testAnimation.PositionKeyFrames.push_back(PositionKeyFrame(meshInfo->position, 0.0f, EaseIn));
                testAnimation.PositionKeyFrames.push_back(PositionKeyFrame(targetLocation, 0.50f, EaseIn));
                testAnimation.Duration = 2.0f;
                meshInfo->animation.IsPlaying = true;
                meshInfo->animation.AnimationTime = 0.0f;

                animationManager->Load("TestAnimation", testAnimation);
            }
        }*/
    }
}

// Init window
void RenderSystem::CreateWindow(const char* title, const int width, const int height, bool fullScreen) {

    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW init failed." << std::endl;
        glfwTerminate();
        return;
    }

    // Fullscreen support
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

    // If window creation fails
    if (!window->theWindow) {
        std::cerr << "Window creation failed." << std::endl;
        glfwTerminate();
        return;
    }

    // set aspect ratio
    glfwSetWindowAspectRatio(window->theWindow, 16, 9);
}

void RenderSystem::Initialize(const char* title, const int width, const int height, bool fullScreen) {

    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW init failed." << std::endl;
        glfwTerminate();
        return;
    }

    // Fullscreen support based on current monitor
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
    // Otherwise uses the provied size for window
    else {
        window->width = width;
        window->height = height;
        window->title = title;

        window->theWindow = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);
    }

    // if init window failed
    if (!window->theWindow) {
        std::cerr << "Window creation failed." << std::endl;
        glfwTerminate();
        return;
    }

    // Set aspect ratio
    glfwSetWindowAspectRatio(window->theWindow, 16, 9);

    // GLFW and glsl upper and lower version
    const char* glsl_version = "#version 420";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // keyboard callback
    glfwSetKeyCallback(window->theWindow, ProcessInput);

    // mouse, mouse button and, scroll callback
    glfwSetCursorPosCallback(window->theWindow, MouseCallBack);
    glfwSetScrollCallback(window->theWindow, ScrollCallBack);
    glfwSetMouseButtonCallback(window->theWindow, MouseButtonCallback);

    // capture mouse input
    glfwSetInputMode(window->theWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Error callback
    glfwSetErrorCallback(ErrorCallback);

    // bring the window in front
    glfwMakeContextCurrent(window->theWindow);

    // Get process address for the app
    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
        std::cerr << "Error: unable to obtain pocess address." << std::endl;
        return;
    }
    glfwSwapInterval(1); //vsync

    // Cull back facing triangles
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Depth test
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Update method called every tick
void RenderSystem::Process(const std::vector<Entity*>& entities, float dt)
{
    memcpy(&(lastKeyPressedID[0]), &(keyPressedID[0]), 255);

    glm::mat4x4 model, view, projection;

    // Cull back facing triangles
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Depth test
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Make a copy of all the entity components
    TransformComponent* transformComponent = nullptr;
    ShaderComponent* shaderComponent = nullptr;
    MeshComponent* meshComponent = nullptr;
    AnimationComponent* animationComponent = nullptr;
    BoundingBoxComponent* boundingBoxComponent = nullptr;
    TextureComponent* textureComponent = nullptr;
    
    // Iterate through all entities
    for (int i = 0; i < entities.size(); i++) {

        Entity* currentEntity = entities[i];

        // get the specific instances for all components
        transformComponent = currentEntity->GetComponentByType<TransformComponent>();
        shaderComponent = currentEntity->GetComponentByType<ShaderComponent>();
        meshComponent = currentEntity->GetComponentByType<MeshComponent>();
        animationComponent = currentEntity->GetComponentByType<AnimationComponent>();
        boundingBoxComponent = currentEntity->GetComponentByType<BoundingBoxComponent>();
        textureComponent = currentEntity->GetComponentByType<TextureComponent>();

        // check if the component exists
        if (transformComponent != nullptr && shaderComponent != nullptr)
        {
            //MVP
            glm::vec3 upVector = glm::vec3(0.f, 1.f, 0.f);

            GLint modelLocaction = glGetUniformLocation(shaderComponent->shaderID, "Model");
            GLint viewLocation = glGetUniformLocation(shaderComponent->shaderID, "View");
            GLint projectionLocation = glGetUniformLocation(shaderComponent->shaderID, "Projection");
            GLint modelInverseLocation = glGetUniformLocation(shaderComponent->shaderID, "ModelInverse");         

            // Lighting
            // ManageLights(shaderComponent->shaderID);

            float ratio;
            int width, height;
            glfwGetFramebufferSize(window->theWindow, &width, &height);
            ratio = width / (float)height;
            glViewport(0, 0, width, height);

            // view = glm::lookAt(camera->position, camera->target, upVector);
            // projection = glm::perspective(0.6f, ratio, 0.1f, 10000.f);

            // mouse support
            if (enableMouse) {
                view = glm::lookAt(camera->position, camera->position + camera->target, upVector);
                projection = glm::perspective(glm::radians(fov), ratio, 0.1f, 10000.f);
            }
            else {
                view = glm::lookAt(camera->position, camera->target, upVector);
                projection = glm::perspective(0.6f, ratio, 0.1f, 10000.f);
            }

            // Size of the viewport
            glm::vec4 viewport = glm::vec4(0, 0, width, height);

            // Where the camera is at
            GLint eyeLocationLocation = glGetUniformLocation(shaderComponent->shaderID, "eyeLocation");
            glUniform4f(eyeLocationLocation, camera->position.x, camera->position.y, camera->position.z, 1.f);

            // Set the model matrix based on transformations applied
            model = glm::mat4x4(1.f);

            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), transformComponent->position);
            glm::mat4 scaling = glm::scale(glm::mat4(1.f), transformComponent->scale);
            glm::mat4 rotation = glm::mat4(transformComponent->rotation);

            model *= translationMatrix;
            model *= rotation;
            model *= scaling;

            // Uniform location in the shader
            glUniformMatrix4fv(modelLocaction, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

            glm::mat4 modelInverse = glm::inverse(glm::transpose(model));
            glUniformMatrix4fv(modelInverseLocation, 1, GL_FALSE, glm::value_ptr(modelInverse));

            if (textureComponent != nullptr) {

                GLint useTextureLocation = glGetUniformLocation(shaderComponent->shaderID, "useTexture");
                GLint useRGBAColourLocation = glGetUniformLocation(shaderComponent->shaderID, "useRGBAColour");
                GLint RGBAColourLocation = glGetUniformLocation(shaderComponent->shaderID, "RGBAColour");

                // Check if it uses RGBA color or a texture
                if (textureComponent->useRGBAColor) {

                    glUniform1f(useRGBAColourLocation, (GLfloat)GL_TRUE);

                    glm::vec4 color = textureComponent->rgbaColor;

                    glUniform4f(RGBAColourLocation, color.r, color.g, color.b, color.w);
                }
                else {
                    glUniform1f(useRGBAColourLocation, (GLfloat)GL_FALSE);
                }

                if (textureComponent->useTexture) {

                    glUniform1f(useTextureLocation, (GLfloat)GL_TRUE);

                    // std::string texture = textureComponent->textures[0];
                    // GLuint textureID = textureManager->getTextureIDFromName(texture);

                    GLuint textureID = textureComponent->textureID;
                    GLuint textureUnit = 0;
                    glActiveTexture(textureUnit + GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, textureID);

                    // set uniform locations
                    GLint textureLocation = glGetUniformLocation(shaderComponent->shaderID, "texture0");
                    glUniform1i(textureLocation, textureUnit);

                    GLint texRatio_0_3 = glGetUniformLocation(shaderComponent->shaderID, "texRatio_0_3");
                    glUniform4f(texRatio_0_3,
                        textureComponent->textureRatios[0],
                        textureComponent->textureRatios[1],
                        textureComponent->textureRatios[2],
                        textureComponent->textureRatios[3]);
                }
                else {
                    glUniform1f(useTextureLocation, (GLfloat)GL_FALSE);
                }
            }

            GLint useFBObIsFullScreenQuadLocation = glGetUniformLocation(shaderComponent->shaderID, "bIsFullScreenQuad");
            glUniform1f(useFBObIsFullScreenQuadLocation, (GLfloat)GL_FALSE);

            GLint bIsSkyboxObjectLocation = glGetUniformLocation(shaderComponent->shaderID, "bIsSkyboxObject");

            // Check if object is a skybox mesh
            if (meshComponent->isSkyBox) {

                glUniform1f(bIsSkyboxObjectLocation, (GLfloat)GL_TRUE);

                // std::string cubeMapTextureName = textureComponent->textures[0];
                // GLuint cubeMapTextureID = textureManager->getTextureIDFromName(cubeMapTextureName);

                GLuint cubeMapTextureID = textureComponent->textureID;
                GLuint textureUnit = 30;			// Texture unit go from 0 to 79
                glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
                GLint skyboxTextureLocation = glGetUniformLocation(shaderComponent->shaderID, "skyboxTexture");
                glUniform1i(skyboxTextureLocation, textureUnit);

                transformComponent->position = camera->position;
                transformComponent->scale = glm::vec3(7500.f);
            }
            else {
                glUniform1f(bIsSkyboxObjectLocation, (GLfloat)GL_FALSE);
            }

            // Manage animations
            if (animationComponent != nullptr) {
                if (glfwGetMouseButton(window->theWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)  {
                    AnimationData testAnimation;
                    testAnimation.PositionKeyFrames.push_back(PositionKeyFrame(transformComponent->position, 0.0f, EaseIn));
                    testAnimation.PositionKeyFrames.push_back(PositionKeyFrame(targetLoc, 0.50f, EaseInOut));
                    testAnimation.Duration = 2.0f;

                    //animationComponent->animation = new Animation();
                    /*animationComponent->animation.IsPlaying = true;
                    animationComponent->animation.AnimationTime = 1.0f;*/

                    animationManager->Load("TestAnimation", testAnimation);
                }                
            }

            // Polygon mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // Find the models vertices and indices
            sModelDrawInfo modelInfo;
            std::string meshName = meshComponent->plyModel.meshName;

            if (vaoManager->FindDrawInfoByModelName(meshName, modelInfo)) {

                // Bind and draw
                glBindVertexArray(modelInfo.VAO_ID);
                glDrawElements(GL_TRIANGLES, modelInfo.numberOfIndices, GL_UNSIGNED_INT, (void*)0);
                glBindVertexArray(0);
            }
            else {
                std::cout << "Model " << meshName << " not found in VAO." << std::endl;
            }

            if (boundingBoxComponent != nullptr) {
                boundingBoxComponent->modelMatrix = model;

                if (boundingBoxComponent->drawBBox) {
                    draw_bbox(&modelInfo, shaderComponent->shaderID, boundingBoxComponent->modelMatrix);
                }
            }
        }
    }

    // Set window title
    std::stringstream ss;
    ss << " Camera: " << "(" << camera->position.x << ", " << camera->position.y << ", " << camera->position.z << ")";

    glfwSetWindowTitle(window->theWindow, ss.str().c_str());

    glfwSwapBuffers(window->theWindow);
    glfwPollEvents();
}

// Gracefully closes everything down
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

    vaoManager = nullptr;
    delete vaoManager;

    textureManager = nullptr;
    delete textureManager;

    exit(EXIT_SUCCESS);
}

// Loads model from ply file and gets it into the VAO
bool RenderSystem::LoadMesh(std::string fileName, std::string modelName, sModelDrawInfo& plyModel, unsigned int shaderID)
{
    plyFileLoader->LoadModel(fileName, plyModel);

    if (vaoManager->LoadModelIntoVAO(modelName, plyModel, shaderID)) {
        std::cout << "Model " << modelName << " loaded successfully." << std::endl;
        return true;
    }
    else {
        std::cout << "Could not load model " << modelName << " into VAO" << std::endl;
        return false;
    }
}

// Set the path where the textures are located
void RenderSystem::SetTexturePath(const char* filePath)
{
    std::cout << "\nLoading Textures..." << std::endl;

    textureManager->SetBasePath(filePath);
}

// Load a 2D texture
bool RenderSystem::Load2DTexture(unsigned int& textureID, const char* filePath)
{
    // Check if the texture loaded
    if (textureManager->Create2DTextureFromBMPFile(filePath))
    {
        textureID = textureManager->getTextureIDFromName(filePath);

        std::cout << "Loaded " << filePath << " texture." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Error: failed to load " << filePath << " texture." << std::endl;
        return false;
    }
}

// Load a skybox texture
bool RenderSystem::LoadCubeMapTexture(
    unsigned int& textureID,
    std::string cubeMapName,
    std::string posX_fileName, std::string negX_fileName,
    std::string posY_fileName, std::string negY_fileName,
    std::string posZ_fileName, std::string negZ_fileName,
    bool bIsSeamless, std::string& errorString)
{
    // Check if the skybox loaded
    if (textureManager->CreateCubeTextureFromBMPFiles(cubeMapName,
        posX_fileName, negX_fileName,
        posY_fileName, negY_fileName,
        posZ_fileName, negZ_fileName,
        bIsSeamless, errorString))
    {
        textureID = textureManager->getTextureIDFromName(cubeMapName);
        std::cout << "Loaded skybox textures: " << cubeMapName << std::endl;
        return true;
    }
    else
    {
        std::cout << "\nError: failed to load skybox because " << errorString;
        return false;
    }
}

// Returns the instance of the window
Window* RenderSystem::GetWindow() {
    return window;
}

// Returns the instance of the camera
Camera* RenderSystem::GetCamera()
{
    return camera;
}

// Set camera position
void RenderSystem::SetCameraPosition(glm::vec3 pos)
{
    camera->position = pos;
}

// Set camera lookat
void RenderSystem::SetCameraTarget(glm::vec3 cameraTarget)
{
    camera->target = cameraTarget;
}

bool* RenderSystem::GetKeyPressedArray()
{
    return keyPressedID;
}

bool RenderSystem::IsKeyHeldDown(unsigned char key)
{
    return lastKeyPressedID[key] && keyPressedID[key];
}

bool RenderSystem::IsKeyPressed(unsigned char key)
{
    return !lastKeyPressedID[key] && keyPressedID[key];
}

bool RenderSystem::IsKeyReleased(unsigned char key)
{
    return lastKeyPressedID[key] && !keyPressedID[key];
}

// Manage all lighting
void ManageLights(unsigned int shaderID) {

    GLint PositionLocation = glGetUniformLocation(shaderID, "sLightsArray[0].position");
    GLint DiffuseLocation = glGetUniformLocation(shaderID, "sLightsArray[0].diffuse");
    GLint SpecularLocation = glGetUniformLocation(shaderID, "sLightsArray[0].specular");
    GLint AttenLocation = glGetUniformLocation(shaderID, "sLightsArray[0].atten");
    GLint DirectionLocation = glGetUniformLocation(shaderID, "sLightsArray[0].direction");
    GLint Param1Location = glGetUniformLocation(shaderID, "sLightsArray[0].param1");
    GLint Param2Location = glGetUniformLocation(shaderID, "sLightsArray[0].param2");

    //glm::vec3 lightPosition0 = meshArray[1]->position;
    glm::vec3 lightPosition0 = glm::vec3(0.f, 25.f, 0.f);
    glUniform4f(PositionLocation, lightPosition0.x, lightPosition0.y, lightPosition0.z, 1.0f);
    //glUniform4f(PositionLocation, 0.f, 0.f, 0.f, 1.0f);
    glUniform4f(DiffuseLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(SpecularLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(AttenLocation, 0.5f, 0.01f, 0.0f, 1.f);
    glUniform4f(DirectionLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(Param1Location, 0.f, 0.f, 0.f, 1.f); //x = Light Type
    glUniform4f(Param2Location, 1.f, 0.f, 0.f, 1.f); //x = Light on/off
}