#include "ECSengine.h"

#include "Moteur.h"

#include "Scene.h"
#include "Timer.h"

#include <sstream>

//extern std::vector <cMeshInfo*> meshArray;
//extern AnimationManager* animationManager;

void Update(float dt);
void GoldenAgeEngine();

int main(int argc, char** argv)
{

    //GoldenAgeEngine();
    ECSengine* engine = new ECSengine();

    //engine->CreateWindow("ECSdemo", 1366, 768, false);
    //engine->Initialize();

    unsigned int entityID = engine->CreateEntity();
    std::string componentName = "TransfromComponent";
    Component* transformComponent;
    transformComponent = dynamic_cast<TransformComponent*>(engine->AddComponent(entityID, componentName));
    //transformComponent->SetPosition(glm::vec3(1.f));

    //while (!glfwWindowShouldClose(engine->GetWindow()->theWindow)) {
    //    //std::cout << "bomb has been planted!";
    //}
    //engine->Shutdown();

    return 0;
}

// example function for user input
void Update(float dt) {

    //Engine::Engine_GetCameraObject()->position.x += 1.f;
}

// engine from the golden age
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