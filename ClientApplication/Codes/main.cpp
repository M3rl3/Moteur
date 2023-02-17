#ifndef MOTEUR
//#define MOTEUR
#endif // !MOTEUR

#ifndef ECS_ENGINE
#define ECS_ENGINE    
#endif // !ECS_ENGINE

#include "ECSengine.h"

#include "RenderSystem.h"
#include "MeshSystem.h"
#include "ShaderSystem.h"

#include "Moteur.h"

#include "Scene.h"
#include "Timer.h"

#include <sstream>

//extern std::vector <cMeshInfo*> meshArray;
//extern AnimationManager* animationManager;

ECSengine* engine;
RenderSystem* renderSystem;

void Update(float dt);

void MoteurKeysCheck(bool* keys);
void ECSKeysCheck(bool* keys);

void ECSEngine();
void GoldenAgeEngine();

// The main class
int main(int argc, char** argv)
{
    // Uncomment to switch between engines
    
#ifdef ECS_ENGINE
    ECSEngine();
#endif // ECS_ENGINE

#ifdef MOTEUR
    GoldenAgeEngine();
#endif // MOTEUR

    return 0;
}

// example function for user input
void Update(float dt) {

#ifdef MOTEUR
    MoteurKeysCheck(Moteur::Engine_GetKeyPressedArray());
#endif // MOTEUR

#ifdef ECS_ENGINE
    ECSKeysCheck(renderSystem->GetKeyPressedArray());
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

void ECSKeysCheck(bool* keys) {

    const float CAMERA_MOVE_SPEED = 1.f;
    if (keys[GLFW_KEY_A])     // Left
    {
        renderSystem->GetCamera()->position.x -= CAMERA_MOVE_SPEED;
    }
    if (keys[GLFW_KEY_D])     // Right
    {
        renderSystem->GetCamera()->position.x += CAMERA_MOVE_SPEED;
    }
    if (keys[GLFW_KEY_W])     // Forward
    {
        renderSystem->GetCamera()->position.z += CAMERA_MOVE_SPEED;
    }
    if (keys[GLFW_KEY_S])     // Backwards
    {
        renderSystem->GetCamera()->position.z -= CAMERA_MOVE_SPEED;
    }
    if (keys[GLFW_KEY_Q])     // Down
    {
        renderSystem->GetCamera()->position.y -= CAMERA_MOVE_SPEED;
    }
    if (keys[GLFW_KEY_E])     // Up
    {
        renderSystem->GetCamera()->position.y += CAMERA_MOVE_SPEED;
    }
}

/// <summary>
/// The new engine initialization. Still WIP
/// </summary>
void ECSEngine() {

    // Check Memory Leak
    _CrtDumpMemoryLeaks();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    //_CrtSetBreakAlloc(185080);

    engine = new ECSengine();

    renderSystem = new RenderSystem();
    renderSystem->Initialize("ECSengine", 1366, 768, false);

    renderSystem->SetCameraPosition(glm::vec3(0.f, 0.1f, -60.f));
    renderSystem->SetCameraTarget(glm::vec3(1.f));

    unsigned int entityID = engine->CreateEntity();

    TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
    transformComponent->position = glm::vec3(0.f, 0.f, 5.f);
    transformComponent->scale = glm::vec3(1.f);
    transformComponent->rotation = glm::quat(glm::vec3(0.f));

    ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);

    unsigned int shaderID = 0;
    const char* v_Shader = "../assets/shaders/vertexShader.glsl";
    const char* f_Shader = "../assets/shaders/fragmentShader.glsl";

    ShaderSystem* shaderSystem = new ShaderSystem();
    shaderSystem->CreateShaderProgramFromFiles(shaderID, v_Shader, f_Shader);
    shaderComponent->shaderID = shaderID;

    MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);

    sModelDrawInfo steve;
    renderSystem->LoadMesh("../assets/meshes/steve.ply", "steve", steve, shaderID);
    meshComponent->plyModel = steve;

    MeshSystem* meshSystem = new MeshSystem();

    TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
    unsigned int textureID = 0;

    meshSystem->SetTexturePath("../assets/textures");
    meshSystem->Load2DTexture(textureID, "man.bmp");

    textureComponent->useRGBAColor = false;
    textureComponent->textureID = textureID;
    textureComponent->textures[0] = "man.bmp";

    textureComponent->textureRatios[0] = 1.f;
    textureComponent->textureRatios[1] = 0.f;
    textureComponent->textureRatios[2] = 0.f;
    textureComponent->textureRatios[3] = 0.f;

    AnimationComponent* animationComponent = engine->AddComponent<AnimationComponent>(entityID);
    animationComponent->animation.AnimationType = "TestAnimation";

    VelocityCompoent* velocityComponent = engine->AddComponent<VelocityCompoent>(entityID);
    velocityComponent->targeting = true;
    //velocityComponent->velocity = glm::vec3(0.f, 0.f, 5.f);
    velocityComponent->destination = glm::vec3(0.f, 0.f, 1000.f);

    MotionSystem* motionSystem = new MotionSystem();

    engine->AddSystem(renderSystem);
    engine->AddSystem(shaderSystem);
    engine->AddSystem(meshSystem);
    engine->AddSystem(motionSystem);

    engine->UpdateCallback(&Update);

    while (!glfwWindowShouldClose(renderSystem->GetWindow()->theWindow)) {
        engine->Update(0.25f);
    }
    engine->Shutdown();
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
    const char* v_Shader = "../assets/shaders/vertexShader.glsl";
    const char* f_Shader = "../assets/shaders/fragmentShader.glsl";

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