#ifndef MOTEUR
//#define MOTEUR
#endif // !MOTEUR

#ifndef ECS_ENGINE
#define ECS_ENGINE    
#endif // !ECS_ENGINE

#include "ECSengine.h"

#include "AIComponent.h"
#include "AISystem.h"
#include "RenderSystem.h"
#include "MeshSystem.h"
#include "ShaderSystem.h"

#include "Moteur.h"

#include "Scene.h"
#include "Timer.h"

#include <sstream>

#include "../Moteur/cBasicTextureManager/cPNGTexture.h"

enum GameMode {
    CAMERA,
    PLAYER,
    NONE
};

GameMode gameMode = CAMERA;

AISystem* aiSystem;

RenderSystem* renderSystem;

// Player's transform and velocity
TransformComponent* transformComponent;
VelocityCompoent* velocityComponent;

void Update(float dt);

void MoteurKeysCheck(bool* keys);
void ECSKeysCheck();

void ECSEngine();
void GoldenAgeEngine();

// The main class
int main(int argc, char** argv)
{
    
#ifdef ECS_ENGINE
    ECSEngine();
#endif // ECS_ENGINE

#ifdef MOTEUR
    GoldenAgeEngine();
#endif // MOTEUR

    return 0;
}

// custom user-defined update function
void Update(float dt) {

#ifdef MOTEUR
    MoteurKeysCheck(Moteur::Engine_GetKeyPressedArray());
#endif // MOTEUR

#ifdef ECS_ENGINE
    ECSKeysCheck();
#endif // ECS_ENGINE
}

void MoteurKeysCheck(bool* keys) {

    const float CAMERA_MOVE_SPEED = 1.f;
    if (keys[GLFW_KEY_A])     // Left
    {
        Moteur::Engine_GetCameraObject()->position.x -= CAMERA_MOVE_SPEED;
    }
    if (keys[GLFW_KEY_D])     // Right
    {
        Moteur::Engine_GetCameraObject()->position.x += CAMERA_MOVE_SPEED;
    }
    if (keys[GLFW_KEY_W])     // Forward
    {
        Moteur::Engine_GetCameraObject()->position.z += CAMERA_MOVE_SPEED;
    }
    if (keys[GLFW_KEY_S])     // Backwards
    {
        Moteur::Engine_GetCameraObject()->position.z -= CAMERA_MOVE_SPEED;
    }
    if (keys[GLFW_KEY_Q])     // Down
    {
        Moteur::Engine_GetCameraObject()->position.y -= CAMERA_MOVE_SPEED;
    }
    if (keys[GLFW_KEY_E])     // Up
    {
        Moteur::Engine_GetCameraObject()->position.y += CAMERA_MOVE_SPEED;
    }


}

void ECSKeysCheck() {
    constexpr float MOVE_SPEED = 1.f;

    if (renderSystem->IsKeyPressed(GLFW_KEY_C)) {
        gameMode = CAMERA;
    }
    if (renderSystem->IsKeyPressed(GLFW_KEY_F)) {
        gameMode = PLAYER;
    }

    switch (gameMode)
    {
    case CAMERA:

        if (renderSystem->IsKeyHeldDown(GLFW_KEY_W)) {
            renderSystem->GetCamera()->position.z += MOVE_SPEED;
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_S)) {
            renderSystem->GetCamera()->position.z -= MOVE_SPEED;
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_D)) {
            renderSystem->GetCamera()->position.x += MOVE_SPEED;
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_A)) {
            renderSystem->GetCamera()->position.x -= MOVE_SPEED;
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_Q)) {
            renderSystem->GetCamera()->position.y += MOVE_SPEED;
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_E)) {
            renderSystem->GetCamera()->position.y -= MOVE_SPEED;
        }
        break;

    case PLAYER:

        if (renderSystem->IsKeyHeldDown(GLFW_KEY_W)) {
            velocityComponent->velocity.z += MOVE_SPEED;
            transformComponent->rotation = glm::quat(glm::vec3(0.0f, glm::radians(0.0f), 0.0f));
        }
        if (renderSystem->IsKeyReleased(GLFW_KEY_W)) {
            velocityComponent->velocity = glm::vec3(0.f);
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_S)) {
            velocityComponent->velocity.z -= MOVE_SPEED;
            transformComponent->rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
        }
        if (renderSystem->IsKeyReleased(GLFW_KEY_S)) {
            velocityComponent->velocity = glm::vec3(0.f);
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_D)) {
            velocityComponent->velocity.x -= MOVE_SPEED;
            transformComponent->rotation = glm::quat(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
        }
        if (renderSystem->IsKeyReleased(GLFW_KEY_D)) {
            velocityComponent->velocity = glm::vec3(0.f);
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_A)) {
            velocityComponent->velocity.x += MOVE_SPEED;
            transformComponent->rotation = glm::quat(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
        }
        if (renderSystem->IsKeyReleased(GLFW_KEY_A)) {
            velocityComponent->velocity = glm::vec3(0.f);
        }
        break;

    case NONE:
        // This should never come to pass
        break;
    default:
        break;
    }

    if (gameMode == PLAYER) {
        renderSystem->GetCamera()->position = transformComponent->position - glm::vec3(0, -3, 10);
        renderSystem->GetCamera()->target = transformComponent->position;
    }
}

/// <summary>
/// The new engine initialization. Still WIP
/// </summary>
void ECSEngine() {

    // Engine init
    ECSengine engine;

    // Init window and rendering systems
    renderSystem = new RenderSystem();
    renderSystem->Initialize("ECSengine", 1366, 768, false);

    renderSystem->SetCameraPosition(glm::vec3(-30.f, 5.f, -50.f));
    renderSystem->SetCameraTarget(glm::vec3(1.f));

    // AI System
    aiSystem = new AISystem();

    // Shaders loaded here
    unsigned int shaderID = 0;
    const char* v_Shader = "../assets/shaders/vertexShader.glsl";
    const char* g_Shader = "../assets/shaders/geometryShader_passthrough.glsl";
    const char* f_Shader = "../assets/shaders/fragmentShader.glsl";

    ShaderSystem* shaderSystem = new ShaderSystem();
    shaderSystem->CreateShaderProgramFromFiles(shaderID, v_Shader, g_Shader, f_Shader);

    // Meshes loaded here
    sModelDrawInfo skybox;
    renderSystem->LoadMesh("../assets/meshes/skybox_sphere.ply", "skybox", skybox, shaderID);
    
    sModelDrawInfo steve;
    renderSystem->LoadMesh("../assets/meshes/steve.ply", "steve", steve, shaderID);

    sModelDrawInfo creepyMonster;
    renderSystem->LoadMesh("../assets/meshes/creepyMonster.ply", "creepyMonster", creepyMonster, shaderID);
    
    sModelDrawInfo plain;
    renderSystem->LoadMesh("../assets/meshes/west_town.ply", "plain", plain, shaderID);
    
    // Textures loaded here
    renderSystem->SetTexturePath("../assets/textures");

    // Skybox textures

    // BMP
    unsigned int skyboxTextureID0 = 0;
    std::string errorString0 = "";

    std::string skyboxNameBMP = "sunnyday";
    renderSystem->LoadCubeMapTextureBMP(skyboxTextureID0, skyboxNameBMP,
        "TropicalSunnyDayLeft2048.bmp",
        "TropicalSunnyDayRight2048.bmp",
        "TropicalSunnyDayDown2048.bmp",
        "TropicalSunnyDayUp2048.bmp",
        "TropicalSunnyDayFront2048.bmp",
        "TropicalSunnyDayBack2048.bmp",
        true, errorString0);


    // PNG
    unsigned int skyboxTextureID1 = 0;
    std::string errorString1 = "";

    std::string skyboxNamePNG = "desert";
    renderSystem->LoadCubeMapTexturePNG(skyboxTextureID1, skyboxNamePNG,
        "desertlf.png",
        "desertrt.png",
        "desertdn.png",
        "desertup.png",
        "desertft.png",
        "desertbk.png",
        true, errorString1);

    // 2D textures

    // PNG Textures
    unsigned int textureID = 0;
    renderSystem->Load2DTexturePNG(textureID, "man.png");

    // BMP Textures
    unsigned int textureID1 = 0;
    renderSystem->Load2DTextureBMP(textureID1, "Archer.bmp");
    
    unsigned int textureID2 = 0;
    renderSystem->Load2DTextureBMP(textureID2, "seamless-green-grass-pattern.bmp");

    unsigned int textureID3 = 0;
    renderSystem->Load2DTextureBMP(textureID3, "full_low_body__BaseColor.bmp");
    
    unsigned int textureID4 = 0;
    renderSystem->Load2DTextureBMP(textureID4, "full_low_body__Roughness.bmp");

    unsigned int textureID5 = 0;
    renderSystem->Load2DTextureBMP(textureID5, "full_low_body__AO.bmp");

    unsigned int textureID6 = 0;
    renderSystem->Load2DTextureBMP(textureID6, "full_low_body__Metallic.bmp");

    unsigned int textureID7 = 0;
    renderSystem->Load2DTextureBMP(textureID7, "full_low_body__Normal.bmp");

    // Lighting
    LightSystem* lightSystem = new LightSystem();

    // Ambient light
    float ambientLight = 0.025f;
    lightSystem->SetAmbientLightAmount(ambientLight);

    // attenuation on all lights
    const glm::vec4 constLightAtten = glm::vec4(0.1f, 0.25f, 1.0e-7f, 1.0f);

    cLight* newLight = lightSystem->AddLight(glm::vec4(-20, 20, 0, 1));

    newLight->diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f);
    newLight->specular = glm::vec4(1.f, 1.f, 1.f, 1.f);
    newLight->direction = glm::vec4(1.f, 1.f, 1.f, 1.f);
    newLight->atten = constLightAtten;
    newLight->param1 = glm::vec4(0.f, 50.f, 50.f, 1.f);
    newLight->param2 = glm::vec4(1.f, 0.f, 0.f, 1.f);

    // If a velocity component exists
    MotionSystem* motionSystem = new MotionSystem();

    // Scene

    {   // Entity "skybox"

        unsigned int entityID = engine.CreateEntity();

        TransformComponent* transformComponent = engine.AddComponent<TransformComponent>(entityID);

        ShaderComponent* shaderComponent = engine.AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine.AddComponent<MeshComponent>(entityID);
        meshComponent->plyModel = skybox;
        meshComponent->isSkyBox = true;

        TextureComponent* textureComponent = engine.AddComponent<TextureComponent>(entityID);
        textureComponent->textureID[0] = skyboxTextureID1;
        textureComponent->textures[0] = skyboxNamePNG;
    }

    {   // Entity "steve"
    
        unsigned int entityID = engine.CreateEntity();

        transformComponent = engine.AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(-18.f, 0.f, -34.f);
        transformComponent->scale = glm::vec3(1.f);
        transformComponent->rotation = glm::quat(glm::vec3(0.f));
        transformComponent->SetType("Player");

        ShaderComponent* shaderComponent = engine.AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine.AddComponent<MeshComponent>(entityID);
        meshComponent->plyModel = steve;

        TextureComponent* textureComponent = engine.AddComponent<TextureComponent>(entityID);
        textureComponent->useTexture = true;
        textureComponent->textureID[0] = textureID;
        textureComponent->textureRatios[0] = 1.f;
        textureComponent->useRGBAColor = false;
        textureComponent->rgbaColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

        BoundingBoxComponent* boundingBoxComponent = engine.AddComponent<BoundingBoxComponent>(entityID);
        boundingBoxComponent->drawBBox = true;

        LitComponent* litComponent = engine.AddComponent<LitComponent>(entityID);
        litComponent->doNotLight = false;

        velocityComponent = engine.AddComponent<VelocityCompoent>(entityID);
        velocityComponent->velocity = glm::vec3(0.f, 0.f, 0.f);
    }

    {   // Entity "creepyMonster"

        unsigned int entityID = engine.CreateEntity();

        TransformComponent* transformComponent = engine.AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(10.f, 0.f, -30.f);
        transformComponent->scale = glm::vec3(1.f);
        transformComponent->rotation = glm::quat(glm::vec3(0.f));

        ShaderComponent* shaderComponent = engine.AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine.AddComponent<MeshComponent>(entityID);
        meshComponent->plyModel = creepyMonster;

        TextureComponent* textureComponent = engine.AddComponent<TextureComponent>(entityID);
        textureComponent->useTexture = true;
        textureComponent->textureID[0] = textureID3;
        textureComponent->textureID[1] = textureID4;
        textureComponent->textureID[2] = textureID5;
        textureComponent->textureID[3] = textureID6;
        textureComponent->textureID[4] = textureID7;
        textureComponent->textureRatios[0] = 0.75f;
        textureComponent->textureRatios[1] = 0.25f;
        textureComponent->textureRatios[2] = 0.25f;
        textureComponent->textureRatios[3] = 0.25f;
        textureComponent->textureRatios[4] = 0.25f;
        textureComponent->useRGBAColor = false;
        textureComponent->rgbaColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

        LitComponent* litComponent = engine.AddComponent<LitComponent>(entityID);
        litComponent->doNotLight = false;

        AIComponent* aiComponent = engine.AddComponent<AIComponent>(entityID);
        aiComponent->radius = 3.0f;
        aiComponent->type = BehaviorType::PURSUE;
        aiComponent->transformComponent = transformComponent;
        aiComponent->speed = 1.0f;
        //aiComponent->targetPosition = transformComponent->position;
    }

    {   // Entity "plain"

        unsigned int entityID = engine.CreateEntity();

        TransformComponent* transformComponent = engine.AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(0.f);
        transformComponent->scale = glm::vec3(10.f);
        transformComponent->rotation = glm::quat(glm::vec3(0.f));

        ShaderComponent* shaderComponent = engine.AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine.AddComponent<MeshComponent>(entityID);
        meshComponent->plyModel = plain;

        TextureComponent* textureComponent = engine.AddComponent<TextureComponent>(entityID);
        textureComponent->useRGBAColor = false;
        textureComponent->rgbaColor = glm::vec4(1, 1, 1, 1);
        textureComponent->useTexture = false;
        textureComponent->textureID[0] = textureID2;
        textureComponent->textures[0] = "seamless-green-grass-pattern.bmp";
        textureComponent->textureRatios[0] = 1.f;

        LitComponent* litComponent = engine.AddComponent<LitComponent>(entityID);
        litComponent->doNotLight = false;
    }

    // Add all the systems
    engine.AddSystem(renderSystem);
    engine.AddSystem(lightSystem);
    engine.AddSystem(shaderSystem);    
    engine.AddSystem(motionSystem);
    engine.AddSystem(aiSystem);
    // engine.AddSystem(meshSystem);

    // User defined update method (for user inputs)
    engine.UpdateCallback(&Update);

    // The actual update method
    while (!glfwWindowShouldClose(renderSystem->GetWindow()->theWindow)) {
        engine.Update(0.25f);
    }

    // Gracefully close everything down
    engine.Shutdown();
}

// The current game engine used
void GoldenAgeEngine() {

    // Check Memory Leak
    _CrtDumpMemoryLeaks();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    //_CrtSetBreakAlloc(185080);

    Moteur::Engine_CreateWindow("Engine", 1366, 768, false, false);

    Moteur::Engine_Initialize();

    glm::vec3 cameraEye = glm::vec3(0, 20, -140);
    Moteur::Engine_SetCameraPosition(cameraEye);

    unsigned int shaderID;
    const char* v_Shader = "../assets/shaders/vertexShader_backup.glsl";
    const char* f_Shader = "../assets/shaders/fragmentShader_backup.glsl";

    Moteur::Engine_CreateShaderProgramFromFiles(shaderID, v_Shader, f_Shader);

    //{
    //    int modelID;
    //    const char* model_path = "../assets/meshes/steve.ply";
    //    const char* model_name = "steve";
    //    Engine::Engine_LoadModel(modelID, model_path, model_name, false, glm::vec3(0.f, 0.f, -75.f), glm::vec4(1.f));
    //    cMeshInfo* playermesh = Engine::Engine_GetMeshObjectFromVector(modelID);
    //    Engine::Engine_SetPlayerMesh(modelID);
    //}
    //
    //{
    //    int modelID;
    //    const char* model_path = "../assets/meshes/bulb.ply";
    //    const char* model_name = "bulb";
    //    Engine::Engine_LoadModel(modelID, model_path, model_name, false, glm::vec3(0.f, 0.f, -5.f), glm::vec4(1.f));
    //}
    //
    //{
    //    int modelID;
    //    const char* model_path = "../assets/meshes/terrain.ply";
    //    const char* model_name = "terrain";
    //    Engine::Engine_LoadModel(modelID, model_path, model_name, false, glm::vec3(50.f, -5.f, 0.f), glm::vec4(1.f));
    //}

    CTimer* pTimer = CTimer::GetInstance();
    unsigned int iFPS = 0;
    float fCheckTime = 0.f;
    std::stringstream ssTitle;
    GLFWwindow* pWindow = Moteur::Engine_GetWindow()->theWindow;
    Camera* pCamera = Moteur::Engine_GetCameraObject();

    Scene* pScene = new Scene;
    pScene->Ready();

    // callback for a user defined update method
    Moteur::Engine_UpdateCallback(&Update);

    while (!glfwWindowShouldClose(pWindow)) {

        pTimer->Update();

        if (pTimer->IsUpdateAvailable())
        {
            float dt = pTimer->GetTimeDelta();

            pScene->Update(dt);
            pScene->Render();

            // made a getter for the mesh vector
            std::vector <cMeshInfo*> theDrawingArray;
            Moteur::Engine_GetDrawingArray(theDrawingArray);

            if (Moteur::Engine_GetAnimationManager() != nullptr) {
                Moteur::Engine_GetAnimationManager()->Update(theDrawingArray, dt);
            }

            Moteur::Engine_Update(dt);
            iFPS++;
        }

        fCheckTime += pTimer->GetTimeDefault();
        if (fCheckTime >= 1.f)
        {
            ssTitle.str("");

            ssTitle << "Camera: " << "(" <<
                pCamera->position.x << ", " <<
                pCamera->position.y << ", " <<
                pCamera->position.z << ") " <<
                "FPS: " << iFPS;

            glfwSetWindowTitle(pWindow, ssTitle.str().c_str());

            iFPS = 0;
            fCheckTime = 0.f;
        }
    }

    pScene->Destroy();

    Moteur::Engine_Shutdown();

    return;
}