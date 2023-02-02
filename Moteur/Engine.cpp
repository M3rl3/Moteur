#include "Engine.h"

GLFWwindow* window;

GLuint shaderID = 0;

cVAOManager* VAOMan;
cBasicTextureManager* TextureMan;
ParticleAccelerator partAcc;
cRenderReticle crosshair;

cMeshInfo* skybox_sphere_mesh;

cMeshInfo* bulb_mesh;

unsigned int readIndex = 0;
int elapsed_frames = 0;

float beginTime = 0.f;
float currentTime = 0.f;
float timeDiff = 0.f;
int frameCount = 0;

std::vector <cMeshInfo*> meshArray;
std::vector <std::string> meshFiles;

enum eEditMode
{
    MOVING_CAMERA,
    MOVING_LIGHT,
    MOVING_SELECTED_OBJECT,
    TAKE_CONTROL,
};

glm::vec3 cameraEye;
// controlled by mouse
glm::vec3 cameraTarget = glm::vec3(0.f, 0.f, -1.f);
eEditMode theEditMode = MOVING_CAMERA;

cMeshInfo* player_mesh;

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

void ReadSceneDescription();
void ManageLights();
float RandomFloat(float a, float b);
bool RandomizePositions(cMeshInfo* mesh);
void getIndex(std::vector<cMeshInfo*> v, cMeshInfo* theMesh, int& id);

//// Callbacks
// static void ErrorCallback(int error, const char* description);
// static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
// static void MouseCallBack(GLFWwindow* window, double xposition, double yposition);
// static void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

static void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

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
        cameraTarget = player_mesh->position;
        cameraEye = player_mesh->position - glm::vec3(20.f, -4.f, 0.f);
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
    if (key == GLFW_KEY_LEFT_ALT) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    /*
    *    updates translation of all objects in the scene based on changes made to scene
    *    description files, resets everything if no changes were made
    */
    if (key == GLFW_KEY_U && action == GLFW_PRESS) {
        ReadSceneDescription();
        player_mesh->particle->position = player_mesh->position;
    }
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
            ::cameraEye.x -= CAMERA_MOVE_SPEED;
        }
        if (key == GLFW_KEY_D)     // Right
        {
            ::cameraEye.x += CAMERA_MOVE_SPEED;
        }
        if (key == GLFW_KEY_W)     // Forward
        {
            ::cameraEye.z += CAMERA_MOVE_SPEED;
        }
        if (key == GLFW_KEY_S)     // Backwards
        {
            ::cameraEye.z -= CAMERA_MOVE_SPEED;
        }
        if (key == GLFW_KEY_Q)     // Down
        {
            ::cameraEye.y -= CAMERA_MOVE_SPEED;
        }
        if (key == GLFW_KEY_E)     // Up
        {
            ::cameraEye.y += CAMERA_MOVE_SPEED;
        }

        if (key == GLFW_KEY_1)
        {
            ::cameraEye = glm::vec3(0.f, 0.f, -5.f);
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
            if (!enableMouse) cameraTarget = glm::vec3(0.f, 0.f, 0.f);
        }
        if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        {
            object_index++;
            if (object_index > meshArray.size() - 1) {
                object_index = 0;
            }
            if (!enableMouse) cameraTarget = meshArray[object_index]->position;
        }
        if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        {
            object_index--;
            if (object_index < 0) {
                object_index = meshArray.size() - 1;
            }
            if (!enableMouse) cameraTarget = meshArray[object_index]->position;
        }
    }
    break;
    case TAKE_CONTROL: {
        if (key == GLFW_KEY_W) {
            player_mesh->velocity.x += 0.5f;
            player_mesh->rotation = glm::quat(glm::vec3(0.f, 67.55f, 0.f));
        }
        if (key == GLFW_KEY_S) {
            player_mesh->velocity.x -= 0.5f;
            player_mesh->rotation = glm::quat(glm::vec3(0.f, -67.55f, 0.f));
        }
        if (key == GLFW_KEY_A) {
            player_mesh->velocity.z -= 0.5f;
            player_mesh->rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));
        }
        if (key == GLFW_KEY_D) {
            player_mesh->velocity.z += 0.5f;
            player_mesh->rotation = glm::quat(glm::vec3(0.f, 135.10f, 0.f));
        }
        if (key == GLFW_KEY_Q) {
            player_mesh->velocity.y += 0.5f;
        }
        if (key == GLFW_KEY_E) {
            player_mesh->velocity.y -= 0.5f;
        }
    }
    break;
    }
}

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
        cameraTarget = glm::normalize(front);
    }
}

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

void Engine::Engine_Initialize() {

    if (!glfwInit()) {
        std::cerr << "GLFW init failed." << std::endl;
        glfwTerminate();
        return;
    }

    const char* glsl_version = "#version 420";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    // keyboard callback
    glfwSetKeyCallback(window, KeyCallback);

    // mouse and scroll callback
    glfwSetCursorPosCallback(window, MouseCallBack);
    glfwSetScrollCallback(window, ScrollCallBack);

    // capture mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetErrorCallback(ErrorCallback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
        std::cerr << "Error: unable to obtain pocess address." << std::endl;
        return;
    }
    glfwSwapInterval(1); //vsync

    // VAO Manager
    VAOMan = new cVAOManager();

    // Init imgui for crosshair
    crosshair.Initialize(window, glsl_version);

    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
}

void Engine::Engine_CreateWindow(const char* title, const int width, const int height, bool fullScreen, bool enableMouse)
{
    if (fullScreen) {
        GLFWmonitor* currentMonitor = glfwGetPrimaryMonitor();

        const GLFWvidmode* mode = glfwGetVideoMode(currentMonitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        window = glfwCreateWindow(mode->height, mode->width, title, currentMonitor, NULL);
    }
    else {
        window = glfwCreateWindow(width, height, title, NULL, NULL);
    }

    if (!window) {
        std::cerr << "Window creation failed." << std::endl;
        glfwTerminate();
        return;
    }

    glfwSetWindowAspectRatio(window, 16, 9);

    Engine_SetEnableMouse(enableMouse);
}

void Engine::Engine_SetEnableMouse(bool enableMouse)
{
    ::enableMouse = enableMouse;
}

void Engine::Engine_CreateShaderProgramFromFiles(unsigned int& id, const char* vertexShader, const char* fragmentShader)
{
    //Shader Manager
    cShaderManager* shadyMan = new cShaderManager();

    cShaderManager::cShader v_Shader;
    cShaderManager::cShader f_Shader;

    v_Shader.fileName = vertexShader;
    f_Shader.fileName = fragmentShader;

    if (!shadyMan->createProgramFromFile("ShadyProgram", v_Shader, f_Shader)) {
        std::cout << "Error: Shader program failed to compile." << std::endl;
        std::cout << shadyMan->getLastError();
        return;
    }
    else {
        std::cout << "Shaders compiled." << std::endl;
    }

    shadyMan->useShaderProgram("ShadyProgram");
    id = shadyMan->getIDFromFriendlyName("ShadyProgram");
    glUseProgram(id);

    shaderID = id;
}

void Engine::Engine_LoadAssetsFromTextFile(const char* path)
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

void Engine::Engine_LoadModel(int& id, const char* filepath, const char* modelName, bool doNotLight, glm::vec4 color)
{
    sModelDrawInfo model;

    std::string path = filepath;
    LoadModel(path, model);
    if (!VAOMan->LoadModelIntoVAO(modelName, model, shaderID)) {
        std::cerr << "Could not load model into VAO" << std::endl;
    }

    cMeshInfo* mesh = new cMeshInfo();
    mesh->meshName = modelName;
    mesh->RGBAColour = color;
    mesh->doNotLight = doNotLight;
    mesh->useRGBAColour = true;
    meshArray.push_back(mesh);

    getIndex(meshArray, mesh, id);
    if (id != -1) {
        std::cout << "Model " << modelName << "loaded successfully";
    }
    else {
        std::cout << "Error: model " << modelName << "not found.";
    }
}

void Engine::Engine_SetTexturePath(const char* filePath)
{
    std::cout << "\nLoading Textures";

    std::string errorString = "";
    TextureMan = new cBasicTextureManager();

    TextureMan->SetBasePath(filePath);
}

void Engine::Engine_Create2DTextureFromBMPFile(const char* filePath)
{
    if (TextureMan->Create2DTextureFromBMPFile(filePath))
    {
        std::cout << "Loaded " << filePath << " texture." << std::endl;
    }
    else
    {
        std::cout << "Error: failed to load " << filePath << " texture.";
    }
}

void Engine::Engine_CreateCubeMapTextureFromBMPFiles(
    std::string cubeMapName, 
    std::string posX_fileName, std::string negX_fileName, 
    std::string posY_fileName, std::string negY_fileName, 
    std::string posZ_fileName, std::string negZ_fileName, 
    bool bIsSeamless, std::string& errorString)
{
    const char* skybox_name = "NightSky";
    if (TextureMan->CreateCubeTextureFromBMPFiles("NightSky",
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

void Engine::Engine_SetDrawingArray(std::vector<cMeshInfo*> vecMesh)
{
    ::meshArray = vecMesh;
}

void Engine::Engine_SetPlayerMesh(cMeshInfo* playerMesh)
{
    ::player_mesh = playerMesh;
}

void Engine::Engine_Update() {

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
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float)height;
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // mouse support
    if (::enableMouse) {
        view = glm::lookAt(::cameraEye, ::cameraEye + ::cameraTarget, upVector);
        projection = glm::perspective(glm::radians(fov), ratio, 0.1f, 10000.f);
    }
    else {
        view = glm::lookAt(::cameraEye, ::cameraTarget, upVector);
        projection = glm::perspective(0.6f, ratio, 0.1f, 10000.f);
    }

    glm::vec4 viewport = glm::vec4(0, 0, width, height);

    GLint eyeLocationLocation = glGetUniformLocation(shaderID, "eyeLocation");
    glUniform4f(eyeLocationLocation, ::cameraEye.x, ::cameraEye.y, ::cameraEye.z, 1.f);

    currentTime = glfwGetTime();
    timeDiff = currentTime - beginTime;
    frameCount++;

    //if (theEditMode == TAKE_CONTROL) {
    //    cameraEye = player_mesh->position - glm::vec3(35.f, -4.f, 0.f);
    //    if (!enableMouse) {
    //        cameraTarget = player_mesh->position;
    //    }
    //    // last velocity when it wasnt 0
    //    if (player_mesh->velocity != glm::vec3(0.f)) {
    //        player_mesh->facingDirection = player_mesh->velocity;
    //    }
    //}

    //bulb_mesh->position = player_mesh->position - glm::vec3(75.f, -25.f, 0.f);

    /*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        mouseClick = true;
    }
    else mouseClick = false;*/

    for (int i = 0; i < meshArray.size(); i++) {

        cMeshInfo* currentMesh = meshArray[i];
        model = glm::mat4x4(1.f);

        if (currentMesh->isVisible == false) {
            continue;
        }

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), currentMesh->position);
        glm::mat4 scaling = glm::scale(glm::mat4(1.f), currentMesh->scale);

        /*glm::mat4 scaling = glm::scale(glm::mat4(1.f), glm::vec3(currentMesh->scale.x,
                                                                 currentMesh->scale.y,
                                                                 currentMesh->scale.z));*/
        if (currentMesh->isSkyBoxMesh) {
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

        // Uncomment to:
        // Randomize the positions of ALL the objects
        // in the scene post every x amount of frames
        // Cause why not?

        //elapsed_frames++;
        //if (elapsed_frames > 100) {
        //    //for (int j = 0; j < meshArray.size(); j++) {
        //    //    cMeshInfo* theMesh = meshArray[j];
        //    //    RandomizePositions(theMesh);
        //    //}
        //    player_mesh->KillAllForces();
        //    elapsed_frames = 0;
        //}

        // adds the model's velocity to its current position
        // currentMesh->TranslateOverTime(0.5f);

        glm::vec3 cursorPos;

        // Division is expensive
        cursorPos.x = width * 0.5;
        cursorPos.y = height * 0.5;

        glm::vec3 worldSpaceCoordinates = glm::unProject(cursorPos, view, projection, viewport);

        glm::normalize(worldSpaceCoordinates);

        // if (mouseClick) {}

        GLint bIsSkyboxObjectLocation = glGetUniformLocation(shaderID, "bIsSkyboxObject");

        if (currentMesh->isSkyBoxMesh) {

            //skybox texture
            GLuint cubeMapTextureNumber = TextureMan->getTextureIDFromName("NightSky");
            GLuint texture30Unit = 30;			// Texture unit go from 0 to 79
            glActiveTexture(texture30Unit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureNumber);
            GLint skyboxTextureLocation = glGetUniformLocation(shaderID, "skyboxTexture");
            glUniform1i(skyboxTextureLocation, texture30Unit);

            glUniform1f(bIsSkyboxObjectLocation, (GLfloat)GL_TRUE);
            currentMesh->position = cameraEye;
            currentMesh->SetUniformScale(7500.f);
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

    // Render the crosshair
    crosshair.Update();

    glfwSwapBuffers(window);
    glfwPollEvents();

    //const GLubyte* vendor = glad_glGetString(GL_VENDOR); // Returns the vendor
    const GLubyte* renderer = glad_glGetString(GL_RENDERER); // Returns a hint to the model

    if (timeDiff >= 1.f / 30.f) {
        std::string frameRate = std::to_string((1.f / timeDiff) * frameCount);
        std::string frameTime = std::to_string((timeDiff / frameCount) * 1000);

        std::stringstream ss;
        ss << " Camera: " << "(" << cameraEye.x << ", " << cameraEye.y << ", " << cameraEye.z << ")" /*
            << " Target: Index = " << object_index << ", MeshName: " << meshArray[object_index]->friendlyName << ", Position: (" << meshArray[object_index]->position.x << ", " << meshArray[object_index]->position.y << ", " << meshArray[object_index]->position.z << ")" */
            << " FPS: " << frameRate << " ms: " << frameTime << " GPU: " << renderer;

        glfwSetWindowTitle(window, ss.str().c_str());

        beginTime = currentTime;
        frameCount = 0;
    }
}

void Engine::Engine_Shutdown() {

    glfwDestroyWindow(window);
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
    glm::vec3 lightPosition0 = glm::vec3(0.f);
    glUniform4f(PositionLocation, lightPosition0.x, lightPosition0.y, lightPosition0.z, 1.0f);
    //glUniform4f(PositionLocation, 0.f, 0.f, 0.f, 1.0f);
    glUniform4f(DiffuseLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(SpecularLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(AttenLocation, 0.1f, 0.5f, 0.0f, 1.f);
    glUniform4f(DirectionLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(Param1Location, 0.f, 0.f, 0.f, 1.f); //x = Light Type
    glUniform4f(Param2Location, 1.f, 0.f, 0.f, 1.f); //x = Light on/off
}

//read scene description files
void ReadSceneDescription() {
    std::ifstream File("sceneDescription.txt");
    if (!File.is_open()) {
        std::cerr << "Could not load file." << std::endl;
        return;
    }

    int number = 0;
    std::string input0;
    std::string input1;
    std::string input2;
    std::string input3;

    std::string temp;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    File >> number;

    for (int i = 0; i < number; i++) {
        File >> input0
            >> input1 >> position.x >> position.y >> position.z
            >> input2 >> rotation.x >> rotation.y >> rotation.z
            >> input3 >> scale.x >> scale.y >> scale.z;

        /*  long_highway
            position 0.0 -1.0 0.0
            rotation 0.0 0.0 0.0
            scale 1.0 1.0 1.0
        */

        temp = input0;

        if (input1 == "position") {
            meshArray[i]->position.x = position.x;
            meshArray[i]->position.y = position.y;
            meshArray[i]->position.z = position.z;
        }
        if (input2 == "rotation") {
            /*meshArray[i]->rotation.x = rotation.x;
            meshArray[i]->rotation.y = rotation.y;
            meshArray[i]->rotation.z = rotation.z;*/
            meshArray[i]->AdjustRoationAngleFromEuler(rotation);
        }
        if (input3 == "scale") {
            meshArray[i]->scale.x = scale.x;
            meshArray[i]->scale.y = scale.y;
            meshArray[i]->scale.z = scale.z;
        }
    }
    File.close();

    std::string yes;
    float x, y, z;
    std::ifstream File1("cameraEye.txt");
    if (!File1.is_open()) {
        std::cerr << "Could not load file." << std::endl;
        return;
    }
    while (File1 >> yes >> x >> y >> z) {
        ::cameraEye.x = x;
        ::cameraEye.y = y;
        ::cameraEye.z = z;
    }
    File1.close();
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

void getIndex(std::vector<cMeshInfo*> v, cMeshInfo* theMesh, int &id)
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