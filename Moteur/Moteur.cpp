#include "Moteur.h"

void Moteur::Initialize() {

    if (!glfwInit()) {
        std::cerr << "GLFW init failed." << std::endl;
        glfwTerminate();
        return;
    }

    const char* glsl_version = "#version 420";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* currentMonitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(currentMonitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(1366, 768, "Eendhan", NULL, NULL);

    // Uncomment for fullscreen support based on current monitor
    // window = glfwCreateWindow(mode->height, mode->width, "Physics 3", currentMonitor, NULL);

    if (!window) {
        std::cerr << "Window creation failed." << std::endl;
        glfwTerminate();
        return;
    }

    glfwSetWindowAspectRatio(window, 16, 9);

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
}

void Moteur::Render() {

    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    //Shader Manager
    cShaderManager* shadyMan = new cShaderManager();

    cShaderManager::cShader vertexShader;
    cShaderManager::cShader fragmentShader;

    vertexShader.fileName = "./shaders/vertexShader.glsl";
    fragmentShader.fileName = "./shaders/fragmentShader.glsl";

    if (!shadyMan->createProgramFromFile("ShadyProgram", vertexShader, fragmentShader)) {
        std::cout << "Error: Shader program failed to compile." << std::endl;
        std::cout << shadyMan->getLastError();
        return;
    }
    else {
        std::cout << "Shaders compiled." << std::endl;
    }

    shadyMan->useShaderProgram("ShadyProgram");
    shaderID = shadyMan->getIDFromFriendlyName("ShadyProgram");
    glUseProgram(shaderID);

    // Load asset paths from external file
    ReadFromFile();

    // VAO Manager
    VAOMan = new cVAOManager();

    // Scene
    sModelDrawInfo bulb;
    LoadModel(meshFiles[0], bulb);
    if (!VAOMan->LoadModelIntoVAO("bulb", bulb, shaderID)) {
        std::cerr << "Could not load model into VAO" << std::endl;
    }
    bulb_mesh = new cMeshInfo();
    bulb_mesh->meshName = "bulb";
    bulb_mesh->friendlyName = "bulb";
    bulb_mesh->isWireframe = false;
    meshArray.push_back(bulb_mesh);

    sModelDrawInfo flat_plain_obj;
    LoadModel(meshFiles[3], flat_plain_obj);
    if (!VAOMan->LoadModelIntoVAO("flat_plain", flat_plain_obj, shaderID)) {
        std::cerr << "Could not load model into VAO" << std::endl;
    }
    cMeshInfo* flat_plain = new cMeshInfo();
    flat_plain->meshName = "flat_plain";
    flat_plain->friendlyName = "flat_beholder";
    flat_plain->RGBAColour = glm::vec4(20.5f, 20.5f, 20.5f, 1.f);
    flat_plain->useRGBAColour = true;
    meshArray.push_back(flat_plain);

    sModelDrawInfo terrain_obj;
    LoadModel(meshFiles[4], terrain_obj);
    if (!VAOMan->LoadModelIntoVAO("terrain", terrain_obj, shaderID)) {
        std::cerr << "Could not load model into VAO" << std::endl;
    }
    cMeshInfo* terrain_mesh = new cMeshInfo();
    terrain_mesh->meshName = "terrain";
    terrain_mesh->isWireframe = false;
    terrain_mesh->RGBAColour = glm::vec4(25.f, 25.f, 25.f, 1.f);
    terrain_mesh->doNotLight = false;
    terrain_mesh->useRGBAColour = true;
    terrain_mesh->isTerrainMesh = false;
    meshArray.push_back(terrain_mesh);

    LoadModel(meshFiles[1], player_obj);
    if (!VAOMan->LoadModelIntoVAO("player", player_obj, shaderID)) {
        std::cerr << "Could not load model into VAO" << std::endl;
    }
    player_mesh = new cMeshInfo();
    player_mesh->meshName = "player";
    player_mesh->friendlyName = "player";
    player_mesh->isWireframe = false;
    player_mesh->RGBAColour = glm::vec4(25.f, 25.f, 25.f, 1.f);
    player_mesh->useRGBAColour = false;
    player_mesh->drawBBox = true;
    player_mesh->CopyVertices(player_obj);
    meshArray.push_back(player_mesh);

    sModelDrawInfo moon_obj;
    LoadModel(meshFiles[5], moon_obj);
    if (!VAOMan->LoadModelIntoVAO("moon", moon_obj, shaderID)) {
        std::cerr << "Could not load model into VAO" << std::endl;
    }
    cMeshInfo* moon_mesh = new cMeshInfo();
    moon_mesh->meshName = "moon";
    moon_mesh->friendlyName = "moon";
    moon_mesh->isWireframe = false;
    moon_mesh->useRGBAColour = false;
    moon_mesh->RGBAColour = glm::vec4(100.f, 25.f, 25.f, 1.f);
    moon_mesh->hasTexture = true;
    moon_mesh->textures[0] = "moon_texture.bmp";
    moon_mesh->textureRatios[0] = 1.0f;
    meshArray.push_back(moon_mesh);

    // skybox sphere with inverted normals
    sModelDrawInfo skybox_sphere_obj;
    LoadModel(meshFiles[12], skybox_sphere_obj);
    if (!VAOMan->LoadModelIntoVAO("skybox_sphere", skybox_sphere_obj, shaderID)) {
        std::cerr << "Could not load model into VAO" << std::endl;
    }
    skybox_sphere_mesh = new cMeshInfo();
    skybox_sphere_mesh->meshName = "skybox_sphere";
    skybox_sphere_mesh->friendlyName = "skybox_sphere";
    skybox_sphere_mesh->isSkyBoxMesh = true;
    meshArray.push_back(skybox_sphere_mesh);

    // skybox/cubemap textures
    std::cout << "\nLoading Textures";

    std::string errorString = "";
    TextureMan = new cBasicTextureManager();

    TextureMan->SetBasePath("../assets/textures");

    const char* skybox_name = "NightSky";
    if (TextureMan->CreateCubeTextureFromBMPFiles("NightSky",
        "SpaceBox_right1_posX.bmp",
        "SpaceBox_left2_negX.bmp",
        "SpaceBox_top3_posY.bmp",
        "SpaceBox_bottom4_negY.bmp",
        "SpaceBox_front5_posZ.bmp",
        "SpaceBox_back6_negZ.bmp",
        true, errorString))
    {
        std::cout << "\nLoaded skybox textures: " << skybox_name << std::endl;
    }
    else
    {
        std::cout << "\nError: failed to load skybox because " << errorString;
    }

    // Basic texture2D
    if (TextureMan->Create2DTextureFromBMPFile("moon_texture.bmp"))
    {
        std::cout << "Loaded moon texture." << std::endl;
    }
    else
    {
        std::cout << "Error: failed to load moon texture.";
    }

    if (TextureMan->Create2DTextureFromBMPFile("Dungeons_2_Texture_01_A.bmp"))
    {
        std::cout << "Loaded dungeon texture." << std::endl;
    }
    else
    {
        std::cout << "Error: failed to load dungeon texture.";
    }

    // reads scene descripion files for positioning and other info
    ReadSceneDescription();
}

void Moteur::Update() {

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
    if (enableMouse) {
        view = glm::lookAt(cameraEye, cameraEye + cameraTarget, upVector);
        projection = glm::perspective(glm::radians(fov), ratio, 0.1f, 10000.f);
    }
    else {
        view = glm::lookAt(cameraEye, cameraTarget, upVector);
        projection = glm::perspective(0.6f, ratio, 0.1f, 10000.f);
    }

    glm::vec4 viewport = glm::vec4(0, 0, width, height);

    GLint eyeLocationLocation = glGetUniformLocation(shaderID, "eyeLocation");
    glUniform4f(eyeLocationLocation, cameraEye.x, cameraEye.y, cameraEye.z, 1.f);

    currentTime = glfwGetTime();
    timeDiff = currentTime - beginTime;
    frameCount++;

    if (theEditMode == TAKE_CONTROL) {
        cameraEye = player_mesh->position - glm::vec3(15.f, -4.f, 0.f);
    }

    // Update particle position per frame
    bulb_mesh->position = player_mesh->position - glm::vec3(75.f, -25.f, 0.f);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        mouseClick = true;
    }
    else mouseClick = false;

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

        // just flatten a beholder for no reason
        if (currentMesh->meshName == "beholderr") {
            glm::mat4 rotation = glm::mat4(currentMesh->rotation);

            model *= translationMatrix;
            model *= rotation;
            model *= scaling;
        }
        else {

            glm::mat4 rotationX = glm::rotate(glm::mat4(1.f), currentMesh->rotation.x, glm::vec3(1.f, 0.f, 0.f));
            glm::mat4 rotationY = glm::rotate(glm::mat4(1.f), currentMesh->rotation.y, glm::vec3(0.f, 1.f, 0.f));
            glm::mat4 rotationZ = glm::rotate(glm::mat4(1.f), currentMesh->rotation.z, glm::vec3(0.f, 0.f, 1.f));

            model *= translationMatrix;
            model *= rotationX;
            model *= rotationY;
            model *= rotationZ;
            model *= scaling;
        }

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

        elapsed_frames++;
        if (elapsed_frames > 100) {
            //for (int j = 0; j < meshArray.size(); j++) {
            //    cMeshInfo* theMesh = meshArray[j];
            //    RandomizePositions(theMesh);
            //}
            player_mesh->KillAllForces();
            elapsed_frames = 0;
        }

        glm::vec3 cursorPos;

        // Division is expensive
        cursorPos.x = width * 0.5;
        cursorPos.y = height * 0.5;

        glm::vec3 worldSpaceCoordinates = glm::unProject(cursorPos, view, projection, viewport);

        glm::normalize(worldSpaceCoordinates);

        if (mouseClick) {}

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

        // Render all the child meshes
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
        // Done post drawing the actual mesh to avoid having it mess with the og model matrix
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
        ss << " Camera: " << "(" << cameraEye.x << ", " << cameraEye.y << ", " << cameraEye.z << ")"
            << " Target: Index = " << object_index << ", MeshName: " << meshArray[object_index]->friendlyName << ", Position: (" << meshArray[object_index]->position.x << ", " << meshArray[object_index]->position.y << ", " << meshArray[object_index]->position.z << ")"
            << " FPS: " << frameRate << " ms: " << frameTime << " GPU: " << renderer;

        glfwSetWindowTitle(window, ss.str().c_str());

        beginTime = currentTime;
        frameCount = 0;
    }
}

// All lights managed here
void Moteur::ManageLights() {

    GLint PositionLocation = glGetUniformLocation(shaderID, "sLightsArray[0].position");
    GLint DiffuseLocation = glGetUniformLocation(shaderID, "sLightsArray[0].diffuse");
    GLint SpecularLocation = glGetUniformLocation(shaderID, "sLightsArray[0].specular");
    GLint AttenLocation = glGetUniformLocation(shaderID, "sLightsArray[0].atten");
    GLint DirectionLocation = glGetUniformLocation(shaderID, "sLightsArray[0].direction");
    GLint Param1Location = glGetUniformLocation(shaderID, "sLightsArray[0].param1");
    GLint Param2Location = glGetUniformLocation(shaderID, "sLightsArray[0].param2");

    //glm::vec3 lightPosition0 = meshArray[1]->position;
    glm::vec3 lightPosition0 = meshArray[0]->position;
    glUniform4f(PositionLocation, lightPosition0.x, lightPosition0.y, lightPosition0.z, 1.0f);
    //glUniform4f(PositionLocation, 0.f, 0.f, 0.f, 1.0f);
    glUniform4f(DiffuseLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(SpecularLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(AttenLocation, 0.1f, 0.5f, 0.0f, 1.f);
    glUniform4f(DirectionLocation, 1.f, 1.f, 1.f, 1.f);
    glUniform4f(Param1Location, 0.f, 0.f, 0.f, 1.f); //x = Light Type
    glUniform4f(Param2Location, 1.f, 0.f, 0.f, 1.f); //x = Light on/off
}

float Moteur::RandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

bool Moteur::RandomizePositions(cMeshInfo* mesh) {

    int i = 0;
    float x, y, z, w;

    x = RandomFloat(-500, 500);
    y = mesh->position.y;
    z = RandomFloat(-200, 200);

    mesh->position = glm::vec3(x, y, z);

    return true;
}

void Moteur::ReadFromFile() {

    std::ifstream readFile("readFile.txt");
    std::string input0;

    while (readFile >> input0) {
        meshFiles.push_back(input0);
        readIndex++;
    }
}

void Moteur::Shutdown() {

    glfwDestroyWindow(window);
    glfwTerminate();

    window = nullptr;

    crosshair.Shutdown();

    exit(EXIT_SUCCESS);
}
