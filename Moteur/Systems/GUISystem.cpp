#include "GUISystem.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../Components/ShaderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoundingBoxComponent.h"
#include "../Components/TextureComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SoundComponent.h"

#include "../Global.h"

GUISystem::GUISystem() 
{
    systemName = "GUISystem";
    drawReticle = false;
    index = 0;
}

GUISystem::~GUISystem()
{
}

void GUISystem::Initialize(Window* window)
{
    // Init dear ImGUI	
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window->theWindow, true);
    ImGui_ImplOpenGL3_Init(window->glslVersion);

    //ImVec2 window_size = ImVec2(window->width, window->height);
    //ImGui::SetNextWindowSize(window_size);

    ImGui::StyleColorsDark();		// dark theme
}

void GUISystem::Process(const std::vector<Entity*>& entities, float dt)
{
    // Start a new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Crosshair
    if (drawReticle) {

        ImVec2 window_size = ImGui::GetIO().DisplaySize;
        ImVec2 window_center = ImVec2(window_size.x * 0.5f, window_size.y * 0.5f);

        ImGui::GetForegroundDrawList()->AddCircle(
            window_center,                // window center
            window_size.y * 0.01f,        // window radius
            IM_COL32(255, 255, 255, 255), // color
            0.f,                          // number of sides
            2.f                           // thickness of each side
        );
    }

    // Make a copy of all the entity components
    TransformComponent* transformComponent = nullptr;
    ShaderComponent* shaderComponent = nullptr;
    MeshComponent* meshComponent = nullptr;
    BoundingBoxComponent* boundingBoxComponent = nullptr;
    TextureComponent* textureComponent = nullptr;
    VelocityComponent* velocityComponent = nullptr;
    RigidBodyComponent* rigidBodyComponent = nullptr;
    SoundComponent* soundComponent = nullptr;

    Entity* currentEntity = entities[index];

    // get the specific instances for all components
    transformComponent = currentEntity->GetComponentByType<TransformComponent>();
    velocityComponent = currentEntity->GetComponentByType<VelocityComponent>();
    shaderComponent = currentEntity->GetComponentByType<ShaderComponent>();
    meshComponent = currentEntity->GetComponentByType<MeshComponent>();
    textureComponent = currentEntity->GetComponentByType<TextureComponent>();
    rigidBodyComponent = currentEntity->GetComponentByType<RigidBodyComponent>();
    soundComponent = currentEntity->GetComponentByType<SoundComponent>();

    ImGui::Begin("Index");
    ImGui::Text("%d", index);
    if (ImGui::Button("+")) {
        index++;
        if (index > entities.size() - 1) {
            index = 0;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("-")) {
        index--;
        if (index < 0) {
            index = entities.size() - 1;
        }
    }
    ImGui::End();

    ImGui::Begin("Transform Component");
    if (transformComponent != nullptr) {

        ImGui::Text("Position");
        ImGui::InputFloat("Position X", &transformComponent->position.x);
        ImGui::InputFloat("Position Y", &transformComponent->position.y);
        ImGui::InputFloat("Position Z", &transformComponent->position.z);
        ImGui::Separator();

        ImGui::Text("Rotation");
        ImGui::InputFloat("Rotation X", &transformComponent->rotation.x);
        ImGui::InputFloat("Rotation Y", &transformComponent->rotation.y);
        ImGui::InputFloat("Rotation Z", &transformComponent->rotation.z);
        ImGui::Separator();

        ImGui::Text("Scale");
        ImGui::InputFloat("Scale X", &transformComponent->scale.x);
        ImGui::InputFloat("Scale Y", &transformComponent->scale.y);
        ImGui::InputFloat("Scale Z", &transformComponent->scale.z);
    }
    ImGui::End();

    ImGui::Begin("Mesh Component");
    if (meshComponent != nullptr) {
        char buffer[20];
        strcpy_s(buffer, meshComponent->meshName.c_str());
        ImGui::InputText("Mesh Name", buffer, 20);
        ImGui::Checkbox("Wireframe", &meshComponent->isWireframe);
    }
    ImGui::End();

    ImGui::Begin("Velocity Component");
    if (velocityComponent != nullptr) {
        ImGui::Text("Velocity");
        ImGui::InputFloat("VelX", &velocityComponent->velocity.x);
        ImGui::InputFloat("VelY", &velocityComponent->velocity.y);
        ImGui::InputFloat("VelZ", &velocityComponent->velocity.z);
        ImGui::Checkbox("Use Velocity", &velocityComponent->useVelocity);
    }
    ImGui::End();

    ImGui::Begin("Rigidbody Component");
    if (rigidBodyComponent != nullptr) {
        ImGui::Checkbox("Use Physics", &rigidBodyComponent->isInfluenced);
    }
    ImGui::End();

    ImGui::Begin("Sound Component");
    if (soundComponent != nullptr) {
        ImGui::InputText("Sound Name", (char*)soundComponent->soundName.c_str(), 30);
        ImGui::InputFloat("Volume", &soundComponent->soundVolume);
        ImGui::InputFloat("Falloff", &soundComponent->maxDistance);
        ImGui::Checkbox("Is Playing", &soundComponent->isPlaying);
    }
    ImGui::End();

    ImGui::Begin("Texture Component");
    if (textureComponent != nullptr) {
        ImGui::InputText("Texture0", (char*)textureComponent->textures[0].c_str(), 30);
        ImGui::InputFloat("TexRatio0", &textureComponent->textureRatios[0]);
        ImGui::InputInt("Format", (int*)&textureComponent->textureFormat);
        ImGui::SameLine();
        if ((int)textureComponent->textureFormat == 0) {
            ImGui::Text("BMP");
        }
        if ((int)textureComponent->textureFormat == 1) {
            ImGui::Text("PNG");
        }
        ImGui::Checkbox("Use Texture", &textureComponent->useTexture);
        ImGui::Separator();

        ImGui::Text("Color");
        ImGui::InputFloat("ColorR", &textureComponent->rgbaColor.r);
        ImGui::InputFloat("ColorG", &textureComponent->rgbaColor.g);
        ImGui::InputFloat("ColorB", &textureComponent->rgbaColor.b);
        ImGui::Checkbox("Use Color", &textureComponent->useRGBAColor);
    }
    ImGui::End();

    // Render imgui stuff to screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Gracefully close everything down
void GUISystem::Shutdown()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}