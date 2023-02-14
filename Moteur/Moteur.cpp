#include "Moteur.h"

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "cShaderManager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"
#include "cBasicTextureManager/cBasicTextureManager.h"
#include "cRenderReticle.h"

#include "ParticleAccelerator.h"
#include "DrawBoundingBox.h"
#include "LoadModel.h"

namespace Moteur {

    enum eEditMode
    {
        MOVING_CAMERA,
        MOVING_LIGHT,
        MOVING_SELECTED_OBJECT,
        TAKE_CONTROL,
    };

    // GLFWwindow* window;
    // The window object
    Window* window;

    // Camera object
    Camera* camera;

    // Compiled shaderID
    GLuint shaderID = 0;

    // Manager for all things VAO related
    cVAOManager* VAOMan;

    // Manager for all things texture related
    cBasicTextureManager* TextureMan;

    // Physics particle
    ParticleAccelerator partAcc;

    // The crosshair
    cRenderReticle crosshair;
    bool enableCrosshair = false;

    // Player model
    cMeshInfo* player_mesh;

    // Skybox sphere with inverted normals
    cMeshInfo* skybox_sphere_mesh;

    unsigned int readIndex = 0;

    //float deltaTime = 0.f;

    //float beginTime = 0.f;
    //float currentTime = 0.f;
    //float timeDiff = 0.f;
    //int frameCount = 0;

    // The actual drawing array
    std::vector <cMeshInfo*> meshArray;

    // array for mesh file names
    std::vector <std::string> meshFiles;

    //glm::vec3 cameraEye = glm::vec3(0.f);
    //glm::vec3 cameraTarget = glm::vec3(0.f, 0.f, -1.f);
    
    // Enum for user input
    eEditMode theEditMode = MOVING_CAMERA;

    // Manager for all things animation related
    AnimationManager* animationManager = nullptr;

    // mouse input
    float yaw = 0.f;
    float pitch = 0.f;
    float fov = 45.f;

    // mouse state
    bool firstMouse = true;
    float lastX = 800.f / 2.f;
    float lastY = 600.f / 2.f;

    bool enableMouse = false;
    bool mouseClick = false;

    int object_index = 0;

    // name of the cubemap texture
    std::string cubemapName = "";

    // User defined update method
    void (*Updatecallback)(float dt);

    // Manager for all things related to lighting
    void ManageLights();

    // gen a random float value
    float RandomFloat(float a, float b);

    // Randomize the position of a mesh 
    bool RandomizePositions(cMeshInfo* mesh);

    // Return the index of a mesh from the drawing vector
    void GetIndex(std::vector<cMeshInfo*> v, cMeshInfo* theMesh, int& id);

    //// Callbacks
    // static void ErrorCallback(int error, const char* description);
    // static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    // static void MouseCallBack(GLFWwindow* window, double xposition, double yposition);
    // static void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

    // if something goes wrong
    static void ErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    // keyboard input
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if (key == GLFW_KEY_C && action == GLFW_PRESS)
        {
            theEditMode = MOVING_CAMERA;
        }
        if (key == GLFW_KEY_O && action == GLFW_PRESS)
        {
            theEditMode = MOVING_SELECTED_OBJECT;
        }
        if (key == GLFW_KEY_F && action == GLFW_PRESS)
        {
            theEditMode = TAKE_CONTROL;
            camera->target = player_mesh->position;
            camera->position = player_mesh->position - glm::vec3(20.f, -4.f, 0.f);
        }
        // Wireframe
        if (key == GLFW_KEY_X && action == GLFW_PRESS) {
            for (int i = 0; i < meshArray.size(); i++) {
                meshArray[i]->isWireframe = true;
            }
        }
        if (key == GLFW_KEY_X && action == GLFW_RELEASE) {
            for (int i = 0; i < meshArray.size(); i++) {
                meshArray[i]->isWireframe = false;
            }
        }
        /*if (key == GLFW_KEY_LEFT_ALT) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }*/
        if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
            enableMouse = !enableMouse;
        }

        switch (theEditMode)
        {
        case MOVING_CAMERA:
        {
            const float CAMERA_MOVE_SPEED = 1.f;
            if (key == GLFW_KEY_A)     // Left
            {
                camera->position.x -= CAMERA_MOVE_SPEED;
            }
            if (key == GLFW_KEY_D)     // Right
            {
                camera->position.x += CAMERA_MOVE_SPEED;
            }
            if (key == GLFW_KEY_W)     // Forward
            {
                camera->position.z += CAMERA_MOVE_SPEED;
            }
            if (key == GLFW_KEY_S)     // Backwards
            {
                camera->position.z -= CAMERA_MOVE_SPEED;
            }
            if (key == GLFW_KEY_Q)     // Down
            {
                camera->position.y -= CAMERA_MOVE_SPEED;
            }
            if (key == GLFW_KEY_E)     // Up
            {
                camera->position.y += CAMERA_MOVE_SPEED;
            }
        }
        break;
        case MOVING_SELECTED_OBJECT:
        {
            const float OBJECT_MOVE_SPEED = 1.f;
            if (key == GLFW_KEY_A)     // Left
            {
                meshArray[object_index]->position.x -= OBJECT_MOVE_SPEED;
            }
            if (key == GLFW_KEY_D)     // Right
            {
                meshArray[object_index]->position.x += OBJECT_MOVE_SPEED;
            }
            if (key == GLFW_KEY_W)     // Forward
            {
                meshArray[object_index]->position.z += OBJECT_MOVE_SPEED;
            }
            if (key == GLFW_KEY_S)     // Backwards
            {
                meshArray[object_index]->position.z -= OBJECT_MOVE_SPEED;
            }
            if (key == GLFW_KEY_Q)     // Down
            {
                meshArray[object_index]->position.y -= OBJECT_MOVE_SPEED;
            }
            if (key == GLFW_KEY_E)     // Up
            {
                meshArray[object_index]->position.y += OBJECT_MOVE_SPEED;
            }

            // Cycle through objects in the scene
            if (key == GLFW_KEY_1 && action == GLFW_PRESS)
            {
                object_index++;
                if (object_index > meshArray.size() - 1) {
                    object_index = 0;
                }
                if (!enableMouse) camera->target = meshArray[object_index]->position;
            }
            if (key == GLFW_KEY_2 && action == GLFW_PRESS)
            {
                object_index--;
                if (object_index < 0) {
                    object_index = meshArray.size() - 1;
                }
                if (!enableMouse) camera->target = meshArray[object_index]->position;
            }
        }
        break;
        case TAKE_CONTROL: { // player object
            constexpr float PLAYER_MOVE_SPEED = 1.f;

            // move the player through forces applied
            if (key == GLFW_KEY_W && action == GLFW_PRESS) {
                player_mesh->velocity.x = PLAYER_MOVE_SPEED;
                player_mesh->rotation = glm::quat(glm::vec3(0.f, 67.55f, 0.f));
            }
            if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
                player_mesh->KillAllForces();
            }
            if (key == GLFW_KEY_S && action == GLFW_PRESS) {
                player_mesh->velocity.x = -PLAYER_MOVE_SPEED;
                player_mesh->rotation = glm::quat(glm::vec3(0.f, -67.55f, 0.f));
            }
            if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
                player_mesh->KillAllForces();
            }
            if (key == GLFW_KEY_A && action == GLFW_PRESS) {
                player_mesh->velocity.z = -PLAYER_MOVE_SPEED;
                player_mesh->rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));
            }
            if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
                player_mesh->KillAllForces();
            }
            if (key == GLFW_KEY_D && action == GLFW_PRESS) {
                player_mesh->velocity.z = PLAYER_MOVE_SPEED;
                player_mesh->rotation = glm::quat(glm::vec3(0.f, 135.10f, 0.f));
            }
            if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
                player_mesh->KillAllForces();
            }
            if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
                player_mesh->velocity.y = PLAYER_MOVE_SPEED;
                //player_mesh->rotation = glm::quat(glm::vec3(0.f, 1.f, 0.f));
            }
            if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
                player_mesh->KillAllForces();
            }
            if (key == GLFW_KEY_E && action == GLFW_PRESS) {
                player_mesh->velocity.y = -PLAYER_MOVE_SPEED;
                //player_mesh->rotation = glm::quat(glm::vec3(0.f, -1.f, 0.f));
            }
            if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
                player_mesh->KillAllForces();
            }
        }
        break;
        }
    }

    // Mouse camera pan
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
            camera->target = glm::normalize(front);
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
            if (animationManager != nullptr) {
                delete animationManager;
                animationManager = nullptr;
            }

            float ratio;
            int rWidth, rHeight;

            glfwGetFramebufferSize(window, &rWidth, &rHeight);
            ratio = rWidth / (float)rHeight;

            glm::vec3 upVector = glm::vec3(0.f, 1.f, 0.f);
            glm::mat4x4 projection = glm::perspective(0.6f, ratio, 0.1f, 10000.0f);
            glm::mat4x4 view = glm::lookAt(camera->position, camera->target, upVector);

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
            float t = -(camera->position.y) / ray_world.y;

            // Get target location
            glm::vec3 targetLocation = camera->position + t * ray_world;
            std::cout << "Mouse left click at position: " << targetLocation.x << ", " << targetLocation.y << ", " << targetLocation.z << std::endl;

            animationManager = new AnimationManager();

            for (cMeshInfo* meshInfo : meshArray) {
                if (meshInfo->enabled) {
                    glm::vec3 direction = glm::normalize(targetLocation - meshInfo->position);
                    float targetAngle = glm::degrees(glm::atan(direction.x, direction.z));
                    for (float i = 0.0f; i < targetAngle; i++)
                        meshInfo->AdjustRoationAngleFromEuler(glm::vec3(0, glm::radians(1.0f), 0));

                    // Create an animation based on the positions
                    AnimationData testAnimation;
                    testAnimation.PositionKeyFrames.push_back(PositionKeyFrame(meshInfo->position, 0.0f, EaseIn));
                    testAnimation.PositionKeyFrames.push_back(PositionKeyFrame(targetLocation, 0.50f, EaseIn));
                    testAnimation.Duration = 2.0f;
                    meshInfo->animation.IsPlaying = true;
                    meshInfo->animation.AnimationTime = 0.0f;

                    // Load the animations for them to actually play
                    animationManager->Load("TestAnimation", testAnimation);
                }
            }
        }
    }

    // Init the engine
    void Moteur::Engine_Initialize() {

        const char* glsl_version = "#version 420";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        // keyboard callback
        glfwSetKeyCallback(window->theWindow, KeyCallback);

        // mouse and scroll callback
        glfwSetCursorPosCallback(window->theWindow, MouseCallBack);
        glfwSetScrollCallback(window->theWindow, ScrollCallBack);
        glfwSetMouseButtonCallback(window->theWindow, MouseButtonCallback);

        // capture mouse input
        glfwSetInputMode(window->theWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glfwSetErrorCallback(ErrorCallback);

        glfwMakeContextCurrent(window->theWindow);

        if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
            std::cerr << "Error: unable to obtain pocess address." << std::endl;
            return;
        }
        glfwSwapInterval(1); //vsync

        // VAO Manager
        VAOMan = new cVAOManager();

        // Init Camera
        camera = new Camera();

        // Init imgui for crosshair
        crosshair.Initialize(window->theWindow, glsl_version);

        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
    }

    // Init the window object
    void Moteur::Engine_CreateWindow(const char* title, const int width, const int height, bool fullScreen, bool enableMouse)
    {
        if (!glfwInit()) {
            std::cerr << "GLFW init failed." << std::endl;
            glfwTerminate();
            return;
        }

        // Init Window
        window = new Window();

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

        // Check if window was created
        if (!window) {
            std::cerr << "Window creation failed." << std::endl;
            glfwTerminate();
            return;
        }

        // Set aspect ratio
        glfwSetWindowAspectRatio(window->theWindow, 16, 9);

        Engine_SetEnableMouse(enableMouse);
    }

    // Setter for enabling mouse input
    void Moteur::Engine_SetEnableMouse(bool enableMouse)
    {
        enableMouse = enableMouse;
    }

    // Getter for the window object
    Window* Moteur::Engine_GetWindow()
    {
        return window;
    }

    // Getter for the camera object
    Camera* Engine_GetCameraObject()
    {
        return camera;
    }

    // Compile the shaders provided
    void Moteur::Engine_CreateShaderProgramFromFiles(unsigned int& id, const char* vertShader, const char* fragShader)
    {
        //Shader Manager
        cShaderManager* shadyMan = new cShaderManager();

        cShaderManager::cShader vertexShader;
        cShaderManager::cShader fragmentShader;

        vertexShader.fileName = vertShader;
        fragmentShader.fileName = fragShader;

        // Check if shaders compiled
        if (!shadyMan->createProgramFromFile("ShadyProgram", vertexShader, fragmentShader)) {
            std::cout << "Error: Shader program failed to compile." << std::endl;
            std::cout << shadyMan->getLastError();
            return;
        }
        else {
            std::cout << "Shaders compiled." << std::endl;
        }

        // Use the compiled shader
        shadyMan->useShaderProgram("ShadyProgram");
        id = shadyMan->getIDFromFriendlyName("ShadyProgram");
        glUseProgram(id);

        shaderID = id;
    }

    // Set a readfile for asset names and paths
    void Moteur::Engine_LoadAssetsFromTextFile(const char* path)
    {
        std::ifstream readFile(path);
        if (!readFile.is_open()) {
            std::cerr << "Error: could not load asset names from file." << std::endl;
            return;
        }
        std::string input;

        while (readFile >> input) {
            meshFiles.push_back(input);
            readIndex++;
        }

        readFile.close();
    }

    // Load a mesh model into VAO
    void Moteur::Engine_LoadModel(int& id, const char* filepath, const char* modelName, bool doNotLight, glm::vec3 position, glm::vec4 color)
    {
        sModelDrawInfo model;

        std::string path = filepath;
        LoadModel(path, model);
        if (!VAOMan->LoadModelIntoVAO(modelName, model, shaderID)) {
            std::cerr << "Could not load model into VAO" << std::endl;
        }

        cMeshInfo* mesh = new cMeshInfo();
        mesh->meshName = modelName;
        mesh->position = position;
        mesh->RGBAColour = color;
        mesh->doNotLight = doNotLight;
        mesh->useRGBAColour = true;
        meshArray.push_back(mesh);

        GetIndex(meshArray, mesh, id);

        // Check if model was loaded
        if (id != -1) {
            std::cout << "Model " << modelName << " loaded successfully." << std::endl;
        }
        else {
            std::cout << "Error: model " << modelName << " not found." << std::endl;
        }
    }

    // Set the path where the textures are located
    void Moteur::Engine_SetTexturePath(const char* filePath)
    {
        std::cout << "\nLoading Textures";

        std::string errorString = "";
        TextureMan = new cBasicTextureManager();

        TextureMan->SetBasePath(filePath);
    }

    // Load a 2D texture from bmp file
    void Moteur::Engine_Create2DTextureFromBMPFile(const char* filePath)
    {
        // Check if texture was loaded
        if (TextureMan->Create2DTextureFromBMPFile(filePath))
        {
            std::cout << "Loaded " << filePath << " texture." << std::endl;
        }
        else
        {
            std::cout << "Error: failed to load " << filePath << " texture." << std::endl;
        }
    }

    // Load a cubemap texture from bmp files
    void Moteur::Engine_CreateCubeMapTextureFromBMPFiles(
        std::string cubeMapName,
        std::string posX_fileName, std::string negX_fileName,
        std::string posY_fileName, std::string negY_fileName,
        std::string posZ_fileName, std::string negZ_fileName,
        bool bIsSeamless, std::string& errorString)
    {
        // make a copy of it to use later
        cubemapName = cubeMapName;

        const char* skybox_name = cubeMapName.c_str();

        // Check if the texture was loaded
        if (TextureMan->CreateCubeTextureFromBMPFiles(skybox_name,
            posX_fileName, negX_fileName,
            posY_fileName, negY_fileName,
            posZ_fileName, negZ_fileName,
            bIsSeamless, errorString))
        {
            std::cout << "\nLoaded skybox textures: " << skybox_name << std::endl;
        }
        else
        {
            std::cout << "\nError: failed to load skybox because " << errorString;
        }
    }

    // Manually create an array of objects to be drawn
    void Moteur::Engine_SetDrawingArray(std::vector<cMeshInfo*> vecMesh)
    {
        meshArray = vecMesh;
    }

    // Getter for the array of objects to be drawn
    void Engine_GetDrawingArray(std::vector<cMeshInfo*>& vecMesh)
    {
        // Error check
        if (meshArray.size() != NULL) {
            vecMesh = meshArray;
        }
    }

    // Set a player model
    void Moteur::Engine_SetPlayerMesh(cMeshInfo* playerMesh)
    {
        player_mesh = playerMesh;
    }

    // Set initial position of the camera
    void Moteur::Engine_SetCameraPosition(glm::vec3 cameraEye)
    {
        camera->position = cameraEye;
    }

    // Set the camera lookAt
    void Moteur::Engine_SetCameraTarget(glm::vec3 cameraTarget)
    {
        camera->target = cameraTarget;
    }

    // Toggle if reticle should be visible
    void Moteur::Engine_SetEnableCrosshair(bool enabled)
    {
        enableCrosshair = enabled;
    }

    // Set a player model
    void Moteur::Engine_SetPlayerMesh(unsigned int id)
    {
        player_mesh = meshArray[id];
    }

    // Set a skybox model
    void Moteur::Engine_SetSkyboxMesh(cMeshInfo* skyboxMesh)
    {
        skybox_sphere_mesh = skyboxMesh;
    }

    // Set a skybox model
    void Moteur::Engine_SetSkyboxMesh(unsigned int id)
    {
        skybox_sphere_mesh = meshArray[id];
    }

    //void Engine::Engine_SetDeltaTime(float dt)
    //{
    //    ::deltaTime = dt;
    //}

    // Getter for animation manager
    AnimationManager* Engine_GetAnimationManager()
    {
        return animationManager;
    }

    // Getter for a specific object from the drawing array
    cMeshInfo* Moteur::Engine_GetMeshObjectFromVector(int id)
    {
        return meshArray[id];
    }

    // Update function running every tick
    void Moteur::Engine_Update(const float& dt) {

        //MVP
        glm::mat4x4 model, view, projection;
        glm::vec3 upVector = glm::vec3(0.f, 1.f, 0.f);

        GLint modelLocaction = glGetUniformLocation(shaderID, "Model");
        GLint viewLocation = glGetUniformLocation(shaderID, "View");
        GLint projectionLocation = glGetUniformLocation(shaderID, "Projection");
        GLint modelInverseLocation = glGetUniformLocation(shaderID, "ModelInverse");

        //Lighting
        ManageLights();

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window->theWindow, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);

        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);
        //glEnable(GL_CULL_FACE);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // mouse support
        if (enableMouse) {
            view = glm::lookAt(camera->position, camera->position + camera->target, upVector);
            projection = glm::perspective(glm::radians(fov), ratio, 0.1f, 10000.f);
        }
        else {
            view = glm::lookAt(camera->position, camera->target, upVector);
            projection = glm::perspective(0.6f, ratio, 0.1f, 10000.f);
        }

        glm::vec4 viewport = glm::vec4(0, 0, width, height);

        GLint eyeLocationLocation = glGetUniformLocation(shaderID, "eyeLocation");
        glUniform4f(eyeLocationLocation, camera->position.x, camera->position.y, camera->position.z, 1.f);

        //currentTime = glfwGetTime();
        //timeDiff = currentTime - beginTime;
        //frameCount++;

        if (player_mesh != NULL && theEditMode == TAKE_CONTROL) {
            camera->position = player_mesh->position - glm::vec3(35.f, -4.f, 0.f);
            if (!enableMouse) {
                camera->target = player_mesh->position;
            }
            // last velocity when it wasnt 0
            if (player_mesh->velocity != glm::vec3(0.f)) {
                player_mesh->facingDirection = player_mesh->velocity;
            }
        }

        for (int i = 0; i < meshArray.size(); i++) {

            cMeshInfo* currentMesh = meshArray[i];
            model = glm::mat4x4(1.f);

            if (currentMesh->isVisible == false) {
                continue;
            }

            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), currentMesh->position);
            glm::mat4 scaling = glm::scale(glm::mat4(1.f), currentMesh->scale);

            if (currentMesh->isSkyBoxMesh || currentMesh == skybox_sphere_mesh) {
                model = glm::mat4x4(1.f);
            }

            glm::mat4 rotation = glm::mat4(currentMesh->rotation);

            model *= translationMatrix;
            model *= rotation;
            model *= scaling;

            glUniformMatrix4fv(modelLocaction, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

            glm::mat4 modelInverse = glm::inverse(glm::transpose(model));
            glUniformMatrix4fv(modelInverseLocation, 1, GL_FALSE, glm::value_ptr(modelInverse));

            if (currentMesh->isWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            GLint useIsTerrainMeshLocation = glGetUniformLocation(shaderID, "bIsTerrainMesh");

            if (currentMesh->isTerrainMesh)
            {
                glUniform1f(useIsTerrainMeshLocation, (GLfloat)GL_TRUE);
            }
            else
            {
                glUniform1f(useIsTerrainMeshLocation, (GLfloat)GL_FALSE);
            }

            GLint RGBAColourLocation = glGetUniformLocation(shaderID, "RGBAColour");

            glUniform4f(RGBAColourLocation, currentMesh->RGBAColour.r, currentMesh->RGBAColour.g, currentMesh->RGBAColour.b, currentMesh->RGBAColour.w);

            GLint useRGBAColourLocation = glGetUniformLocation(shaderID, "useRGBAColour");

            if (currentMesh->useRGBAColour)
            {
                glUniform1f(useRGBAColourLocation, (GLfloat)GL_TRUE);
            }
            else
            {
                glUniform1f(useRGBAColourLocation, (GLfloat)GL_FALSE);
            }

            GLint bHasTextureLocation = glGetUniformLocation(shaderID, "bHasTexture");

            if (currentMesh->hasTexture)
            {
                glUniform1f(bHasTextureLocation, (GLfloat)GL_TRUE);

                std::string texture0 = currentMesh->textures[0];    // moon
                //std::string texture1 = currentMesh->textures[1];    // dungeon

                GLuint texture0ID = TextureMan->getTextureIDFromName(texture0);

                GLuint texture0Unit = 0;
                glActiveTexture(texture0Unit + GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture0ID);

                GLint texture0Location = glGetUniformLocation(shaderID, "texture0");
                glUniform1i(texture0Location, texture0Unit);

                GLint texRatio_0_3 = glGetUniformLocation(shaderID, "texRatio_0_3");
                glUniform4f(texRatio_0_3,
                    currentMesh->textureRatios[0],
                    currentMesh->textureRatios[1],
                    currentMesh->textureRatios[2],
                    currentMesh->textureRatios[3]);
            }
            else
            {
                glUniform1f(bHasTextureLocation, (GLfloat)GL_FALSE);
            }

            GLint doNotLightLocation = glGetUniformLocation(shaderID, "doNotLight");

            if (currentMesh->doNotLight)
            {
                glUniform1f(doNotLightLocation, (GLfloat)GL_TRUE);
            }
            else
            {
                glUniform1f(doNotLightLocation, (GLfloat)GL_FALSE);
            }

            currentMesh->TranslateOverTime(dt);

            glm::vec3 cursorPos;

            // Division is expensive
            cursorPos.x = width * 0.5;
            cursorPos.y = height * 0.5;

            glm::vec3 worldSpaceCoordinates = glm::unProject(cursorPos, view, projection, viewport);

            glm::normalize(worldSpaceCoordinates);

            GLint bIsSkyboxObjectLocation = glGetUniformLocation(shaderID, "bIsSkyboxObject");

            if (currentMesh->isSkyBoxMesh || currentMesh == skybox_sphere_mesh) {

                // skybox texture
                // fetches cube map name from the string passed in.
                if (cubemapName.length() != 0) {
                    GLuint cubeMapTextureNumber = TextureMan->getTextureIDFromName(cubemapName);
                    GLuint texture30Unit = 30;			// Texture unit go from 0 to 79
                    glActiveTexture(texture30Unit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
                    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureNumber);
                    GLint skyboxTextureLocation = glGetUniformLocation(shaderID, "skyboxTexture");
                    glUniform1i(skyboxTextureLocation, texture30Unit);

                    glUniform1f(bIsSkyboxObjectLocation, (GLfloat)GL_TRUE);
                    currentMesh->position = camera->position;
                    currentMesh->SetUniformScale(7500.f);
                }
            }
            else {
                glUniform1f(bIsSkyboxObjectLocation, (GLfloat)GL_FALSE);
            }

            sModelDrawInfo modelInfo;
            if (VAOMan->FindDrawInfoByModelName(meshArray[i]->meshName, modelInfo)) {

                glBindVertexArray(modelInfo.VAO_ID);
                glDrawElements(GL_TRIANGLES, modelInfo.numberOfIndices, GL_UNSIGNED_INT, (void*)0);
                glBindVertexArray(0);
            }
            else {
                std::cout << "Model not found." << std::endl;
            }

            if (currentMesh->hasChildMeshes) {

                sModelDrawInfo modelInfo;
                if (VAOMan->FindDrawInfoByModelName(currentMesh->vecChildMeshes[0]->meshName, modelInfo)) {

                    glBindVertexArray(modelInfo.VAO_ID);
                    glDrawElements(GL_TRIANGLES, modelInfo.numberOfIndices, GL_UNSIGNED_INT, (void*)0);
                    glBindVertexArray(0);
                }
                else {
                    std::cout << "Model not found." << std::endl;
                }
            }

            // Only draw bounding box around meshes with this boolean value set to true
            if (currentMesh->drawBBox) {
                draw_bbox(currentMesh, shaderID, model);  /*
                                                           *  pass in the model matrix after drawing
                                                           *  so it doesnt screw with the matrix values
                                                           */
            }
            else {
                currentMesh->drawBBox = false;
            }
        }

        if (enableCrosshair) {
            // Render the crosshair
            crosshair.Update();
        }

        // Set the update callback
        if (Updatecallback != NULL) {
            Updatecallback(dt);
        }

        glfwSwapBuffers(window->theWindow);
        glfwPollEvents();

        //const GLubyte* vendor = glad_glGetString(GL_VENDOR); // Returns the vendor
        const GLubyte* renderer = glad_glGetString(GL_RENDERER); // Returns a hint to the model

        //if (timeDiff >= 1.f / 30.f) {
        //    std::string frameRate = std::to_string((1.f / timeDiff) * frameCount);
        //    std::string frameTime = std::to_string((timeDiff / frameCount) * 1000);

        //    std::stringstream ss;
        //    ss << " Camera: " << "(" << cameraEye.x << ", " << cameraEye.y << ", " << cameraEye.z << ")" /*
        //        << " Target: Index = " << object_index << ", MeshName: " << meshArray[object_index]->friendlyName << ", Position: (" << meshArray[object_index]->position.x << ", " << meshArray[object_index]->position.y << ", " << meshArray[object_index]->position.z << ")" */
        //        << " FPS: " << frameRate << " ms: " << frameTime << " GPU: " << renderer;

        //    glfwSetWindowTitle(window->theWindow, ss.str().c_str());

        //    beginTime = currentTime;
        //    frameCount = 0;
        //}
    }

    // Callback for a user defined update method
    void Engine_UpdateCallback(void(*Callback)(float dt))
    {
        Updatecallback = Callback;
    }

    // Gracefully closes everything down
    void Moteur::Engine_Shutdown() {

        glfwDestroyWindow(window->theWindow);
        glfwTerminate();

        window = nullptr;
        delete window;

        crosshair.Shutdown();

        exit(EXIT_SUCCESS);
    }

    // All lights managed here
    void ManageLights() {

        GLint PositionLocation = glGetUniformLocation(shaderID, "sLightsArray[0].position");
        GLint DiffuseLocation = glGetUniformLocation(shaderID, "sLightsArray[0].diffuse");
        GLint SpecularLocation = glGetUniformLocation(shaderID, "sLightsArray[0].specular");
        GLint AttenLocation = glGetUniformLocation(shaderID, "sLightsArray[0].atten");
        GLint DirectionLocation = glGetUniformLocation(shaderID, "sLightsArray[0].direction");
        GLint Param1Location = glGetUniformLocation(shaderID, "sLightsArray[0].param1");
        GLint Param2Location = glGetUniformLocation(shaderID, "sLightsArray[0].param2");

        //glm::vec3 lightPosition0 = meshArray[1]->position;
        glm::vec3 lightPosition0 = glm::vec3(0.f, 50.f, 0.f);
        glUniform4f(PositionLocation, lightPosition0.x, lightPosition0.y, lightPosition0.z, 1.0f);
        //glUniform4f(PositionLocation, 0.f, 0.f, 0.f, 1.0f);
        glUniform4f(DiffuseLocation, 1.f, 1.f, 1.f, 1.f);
        glUniform4f(SpecularLocation, 1.f, 1.f, 1.f, 1.f);
        glUniform4f(AttenLocation, 0.5f, 0.01f, 0.0f, 1.f);
        glUniform4f(DirectionLocation, 1.f, 1.f, 1.f, 1.f);
        glUniform4f(Param1Location, 0.f, 0.f, 0.f, 1.f); //x = Light Type
        glUniform4f(Param2Location, 1.f, 0.f, 0.f, 1.f); //x = Light on/off
    }

    float RandomFloat(float a, float b) {
        float random = ((float)rand()) / (float)RAND_MAX;
        float diff = b - a;
        float r = random * diff;
        return a + r;
    }

    bool RandomizePositions(cMeshInfo* mesh) {

        int i = 0;
        float x, y, z, w;

        x = RandomFloat(-500, 500);
        y = mesh->position.y;
        z = RandomFloat(-200, 200);

        mesh->position = glm::vec3(x, y, z);

        return true;
    }

    void GetIndex(std::vector<cMeshInfo*> v, cMeshInfo* theMesh, int& id)
    {
        auto it = find(v.begin(), v.end(), theMesh);

        // If element was found
        if (it != v.end())
        {
            // calculating the index of theMesh
            int index = it - v.begin();
            id = index;
        }
        else {
            // If the element is not present in the vector
            id = -1;
        }
    }
}
