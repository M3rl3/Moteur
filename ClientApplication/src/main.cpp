#ifndef MOTEUR
//#define MOTEUR
#endif // !MOTEUR

#ifndef ECS_ENGINE
#define ECS_ENGINE    
#endif // !ECS_ENGINE

#include "ECSengine/ECSengine.h"

#include "Components/AIComponent.h"
#include "Components/TransformComponent.h"
#include "Components/MeshComponent.h"
#include "Components/ShaderComponent.h"
#include "Components/TextureComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/BoundingBoxComponent.h"
#include "Components/LitComponent.h"
#include "Components/SoundComponent.h"

#include "Systems/ShaderSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/MeshSystem.h"
#include "Systems/MotionSystem.h"
#include "Systems/LightSystem.h"
#include "Systems/SoundSystem.h"
#include "Systems/AISystem.h"

#include "OldEngine/Moteur.h"

#include "Scene.h"
#include "Timer/Timer.h"

#include <sstream>

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
            renderSystem->GetCamera()->MoveForward();
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_S)) {
            renderSystem->GetCamera()->MoveBackward();
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_D)) {
            renderSystem->GetCamera()->StrafeRight();
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_A)) {
            renderSystem->GetCamera()->StrafeLeft();
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_Q)) {
            renderSystem->GetCamera()->MoveUp();
        }
        if (renderSystem->IsKeyHeldDown(GLFW_KEY_E)) {
            renderSystem->GetCamera()->MoveDown();
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

        if (!renderSystem->GetMouseStatus()) {
            renderSystem->GetCamera()->target = transformComponent->position;
        }
    }
}

/// <summary>
/// The new engine initialization.
/// </summary>
void ECSEngine() {

    // Engine init
    ECSengine engine;

    // Init window and rendering systems
    renderSystem = new RenderSystem();
    renderSystem->Initialize("ECSengine", 1366, 768, false);

    renderSystem->SetCameraPosition(glm::vec3(-30.f, 5.f, -50.f));
    renderSystem->SetCameraTarget(glm::vec3(0.f, 0.f, 1.f));

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
    renderSystem->SetMeshPath("../assets/meshes");

    renderSystem->LoadMesh("skybox_sphere.ply", "skybox", shaderID);
    renderSystem->LoadMesh("steve.ply", "steve", shaderID);
    renderSystem->LoadMesh("creepyMonster.ply", "creepyMonster", shaderID);
    renderSystem->LoadMesh("west_town.ply", "plain", shaderID);
    
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
    renderSystem->Load2DTexturePNG("man.png");

    // BMP Textures
    renderSystem->Load2DTextureBMP("Archer.bmp");
    renderSystem->Load2DTextureBMP("seamless-green-grass-pattern.bmp");
    renderSystem->Load2DTextureBMP("full_low_body__BaseColor.bmp");

    // Sounds
    SoundSystem* soundSystem = new SoundSystem();

    // Flags
    constexpr int flags = FMOD_DEFAULT | FMOD_3D | FMOD_LOOP_NORMAL;

    // Sounds loaded here
    soundSystem->SetSoundPath("../assets/sounds/sfx");

    soundSystem->LoadSound("deep_stone_lullaby.mp3", flags);
    soundSystem->LoadSound("chicken.wav", flags);

    // Lighting
    LightSystem* lightSystem = new LightSystem();

    // Ambient light
    float ambientLight = 0.5f;
    lightSystem->SetAmbientLightAmount(ambientLight);

    // attenuation on all lights
    const glm::vec4 constLightAtten = glm::vec4(0.1f, 2.5e-5f, 2.5e-5f, 1.0f);

    cLight* newLight = lightSystem->AddLight(glm::vec4(0, 50, 0, 1));

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
        meshComponent->meshName = "skybox";
        meshComponent->isWireframe = false;
        meshComponent->isSkyBox = true;

        TextureComponent* textureComponent = engine.AddComponent<TextureComponent>(entityID);
        textureComponent->textureFormat = TextureFormat::PNG;
        textureComponent->textures[0] = "desert";
    }

    {   // Entity "steve"
    
        unsigned int entityID = engine.CreateEntity();

        transformComponent = engine.AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(-18.f, 0.f, -34.f);
        transformComponent->scale = glm::vec3(1.f);
        transformComponent->rotation = glm::quat(glm::vec3(0.f));

        ShaderComponent* shaderComponent = engine.AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine.AddComponent<MeshComponent>(entityID);
        meshComponent->meshName = "steve";
        meshComponent->isWireframe = false;

        TextureComponent* textureComponent = engine.AddComponent<TextureComponent>(entityID);
        textureComponent->useTexture = true;
        textureComponent->textureFormat = TextureFormat::PNG;
        textureComponent->textures[0] = "man.png";
        textureComponent->textureRatios[0] = 1.f;
        textureComponent->useRGBAColor = false;
        textureComponent->rgbaColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

        BoundingBoxComponent* boundingBoxComponent = engine.AddComponent<BoundingBoxComponent>(entityID);
        boundingBoxComponent->drawBBox = true;

        LitComponent* litComponent = engine.AddComponent<LitComponent>(entityID);
        litComponent->isLit = true;

        SoundComponent* soundComponent = engine.AddComponent<SoundComponent>(entityID);
        soundComponent->isPlaying = true;
        soundComponent->maxDistance = 1.f;
        soundComponent->soundVolume = 5.f;
        soundComponent->soundName = "deep_stone_lullaby.mp3";

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
        meshComponent->meshName = "creepyMonster";
        meshComponent->isWireframe = false;

        TextureComponent* textureComponent = engine.AddComponent<TextureComponent>(entityID);
        textureComponent->useTexture = true;
        textureComponent->textureFormat = TextureFormat::BMP;
        textureComponent->textures[0] = "full_low_body__BaseColor.bmp";
        textureComponent->textureRatios[0] = 1.f;
        textureComponent->useRGBAColor = false;
        textureComponent->rgbaColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

        LitComponent* litComponent = engine.AddComponent<LitComponent>(entityID);
        litComponent->isLit = true;

        SoundComponent* soundComponent = engine.AddComponent<SoundComponent>(entityID);
        soundComponent->isPlaying = true;
        soundComponent->maxDistance = 1.f;
        soundComponent->soundVolume = 5.f;
        soundComponent->soundName = "chicken.wav";

        AIComponent* aiComponent = engine.AddComponent<AIComponent>(entityID);
        aiComponent->radius = 3.0f;
        aiComponent->type = BehaviorType::PURSUE;
        aiComponent->transformComponent = transformComponent;
        aiComponent->speed = 1.0f;
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
        meshComponent->meshName = "plain";
        meshComponent->isWireframe = false;

        TextureComponent* textureComponent = engine.AddComponent<TextureComponent>(entityID);
        textureComponent->useRGBAColor = false;
        textureComponent->rgbaColor = glm::vec4(0.35f, 0.35f, 0.35f, 1.f);
        textureComponent->useTexture = false;
        textureComponent->textureFormat = TextureFormat::BMP;
        textureComponent->textures[0] = "seamless-green-grass-pattern.bmp";
        textureComponent->textureRatios[0] = 1.f;

        LitComponent* litComponent = engine.AddComponent<LitComponent>(entityID);
        litComponent->isLit = true;
    }

    // Add all the systems
    engine.AddSystem(renderSystem);
    engine.AddSystem(lightSystem);
    engine.AddSystem(shaderSystem);
    engine.AddSystem(motionSystem);
    engine.AddSystem(soundSystem);
    engine.AddSystem(aiSystem);

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