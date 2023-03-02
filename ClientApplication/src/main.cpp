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

RenderSystem* renderSystem;

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
    constexpr float CAMERA_MOVE_SPEED = 1.f;

    if (renderSystem->IsKeyHeldDown(GLFW_KEY_W)) {
        renderSystem->GetCamera()->position.z += CAMERA_MOVE_SPEED;
    }
    if (renderSystem->IsKeyHeldDown(GLFW_KEY_S)) {
        renderSystem->GetCamera()->position.z -= CAMERA_MOVE_SPEED;
    }
    if (renderSystem->IsKeyHeldDown(GLFW_KEY_D)) {
        renderSystem->GetCamera()->position.x += CAMERA_MOVE_SPEED;
    }
    if (renderSystem->IsKeyHeldDown(GLFW_KEY_A)) {
        renderSystem->GetCamera()->position.x -= CAMERA_MOVE_SPEED;
    }
    if (renderSystem->IsKeyHeldDown(GLFW_KEY_Q)) {
        renderSystem->GetCamera()->position.y += CAMERA_MOVE_SPEED;
    }
    if (renderSystem->IsKeyHeldDown(GLFW_KEY_E)) {
        renderSystem->GetCamera()->position.y -= CAMERA_MOVE_SPEED;
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

    renderSystem->SetCameraPosition(glm::vec3(0.f, 0.1f, -60.f));
    renderSystem->SetCameraTarget(glm::vec3(1.f));

    // Shaders loaded here
    unsigned int shaderID = 0;
    const char* v_Shader = "../assets/shaders/vertexShader.glsl";
    const char* f_Shader = "../assets/shaders/fragmentShader.glsl";

    ShaderSystem* shaderSystem = new ShaderSystem();
    shaderSystem->CreateShaderProgramFromFiles(shaderID, v_Shader, f_Shader);

    // Meshes loaded here
    sModelDrawInfo steve;
    renderSystem->LoadMesh("../assets/meshes/steve.ply", "steve", steve, shaderID);
    
    sModelDrawInfo plain;
    renderSystem->LoadMesh("../assets/meshes/plane_reoriented.ply", "plain", plain, shaderID);

    // Textures loaded here
    MeshSystem* meshSystem = new MeshSystem();
    meshSystem->SetTexturePath("../assets/textures");

    unsigned int textureID = 0;
    meshSystem->Load2DTexture(textureID, "man.bmp");

    // Lighting
    LightSystem* lightSystem = new LightSystem();
    
    float ambientLight = 0.025f;
    lightSystem->SetAmbientLightAmount(ambientLight);

    cLight* newLight = lightSystem->AddLight(glm::vec4(-20, 20, 0, 1));

    newLight->diffuse = glm::vec4(10, 10, 10, 1);
    newLight->specular = glm::vec4(1.f);
    newLight->atten = glm::vec4(0.5f, 0.01f, 0.0f, 1.f);
    newLight->direction = glm::vec4(1.f);
    newLight->param1 = glm::vec4(0, 0, 0, 1);
    newLight->param2 = glm::vec4(1, 0, 0, 1);

    // If a velocity component exits
    MotionSystem* motionSystem = new MotionSystem();

    // Scene

    {   // Entity "steve"
    
        unsigned int entityID = engine.CreateEntity();

        TransformComponent* transformComponent = engine.AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(0.f, 0.f, 5.f);
        transformComponent->scale = glm::vec3(1.f);
        transformComponent->rotation = glm::quat(glm::vec3(0.f));

        ShaderComponent* shaderComponent = engine.AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine.AddComponent<MeshComponent>(entityID);
        meshComponent->plyModel = steve;

        TextureComponent* textureComponent = engine.AddComponent<TextureComponent>(entityID);
        textureComponent->useRGBAColor = true;
        textureComponent->rgbaColor = glm::vec4(100, 100, 100, 1);
        textureComponent->textureID = textureID;
        textureComponent->textures[0] = "man.bmp";
        textureComponent->textureRatios[0] = 1.f;

        LitComponent* litComponent = engine.AddComponent<LitComponent>(entityID);
        litComponent->doNotLight = false;

        BoundingBoxComponent* boundingBoxComponent = engine.AddComponent<BoundingBoxComponent>(entityID);
        boundingBoxComponent->drawBBox = true;

        AnimationComponent* animationComponent = engine.AddComponent<AnimationComponent>(entityID);
        animationComponent->animation.AnimationType = "TestAnimation";

        VelocityCompoent* velocityComponent = engine.AddComponent<VelocityCompoent>(entityID);
        velocityComponent->targeting = true;
        //velocityComponent->velocity = glm::vec3(0.f, 0.f, 5.f);
        velocityComponent->destination = glm::vec3(0.f, 0.f, 1000.f);
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
        textureComponent->useRGBAColor = true;
        textureComponent->rgbaColor = glm::vec4(25, 25, 25, 1);

        LitComponent* litComponent = engine.AddComponent<LitComponent>(entityID);
        litComponent->doNotLight = false;
    }

    // Add all the systems
    engine.AddSystem(renderSystem);
    engine.AddSystem(lightSystem);
    engine.AddSystem(meshSystem);
    engine.AddSystem(shaderSystem);    
    engine.AddSystem(motionSystem);

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