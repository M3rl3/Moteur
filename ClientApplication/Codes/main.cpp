#include "Engine.h"
#include "Scene.h"

int main(int argc, char** argv)
{
    // Check Memory Leak
    _CrtDumpMemoryLeaks();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    //_CrtSetBreakAlloc(185080);



    Engine::Engine_CreateWindow("Engine", 1366, 768, false, false);

    Engine::Engine_Initialize();

    glm::vec3 cameraEye = glm::vec3(0, 20, -140);
    Engine::Engine_SetCameraPosition(cameraEye);

    unsigned int shaderID;
    const char* v_Shader = "../assets/shaders/vertexShader.glsl";
    const char* f_Shader = "../assets/shaders/fragmentShader.glsl";

    Engine::Engine_CreateShaderProgramFromFiles(shaderID, v_Shader, f_Shader);

    //{
    //    int modelID;
    //    const char* model_path = "../assets/meshes/steve.ply";
    //    const char* model_name = "steve";
    //    Engine::Engine_LoadModel(modelID, model_path, model_name, false, glm::vec3(0.f, 0.f, -75.f), glm::vec4(1.f));
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
    
    Scene* pScene = new Scene;
    pScene->Ready();

    while (!glfwWindowShouldClose(Engine::Engine_GetWindow())) {
        Engine::Engine_Update();

        pScene->Update(0.1f);
        pScene->Render();
    }

    pScene->Destroy();

    Engine::Engine_Shutdown();

    return 0;
}