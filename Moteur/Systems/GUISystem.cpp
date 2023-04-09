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

// list box stuff
int selectedItem;
bool isExpanded;

int selectedtexture;

GUISystem::GUISystem() 
{
    systemName = "GUISystem";
    drawReticle = false;
    index = 0;
    doOnce = true;

    selectedItem = 0;
    isExpanded = false;

    selectedtexture = 0;
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

    // Only populate the list once
    if (doOnce) {
        MeshComponent* mesh_component = nullptr;

        for (int i = 0; i < entities.size(); i++) {
            Entity* currentEntity = entities[i];

            mesh_component = currentEntity->GetComponentByType<MeshComponent>();

            if (mesh_component != nullptr) {
                // push back all the mesh item names into this vector
                meshNames.push_back(mesh_component->meshName);
            }
        }
        doOnce = false;
    }

    // The scene hierarchy
    if (ImGui::Begin("Hierarchy", &isExpanded))
    {    
        // Do this only if the window is expanded
        
        // Create a listbox for the entities
        if (ImGui::BeginListBox("Entities")) {
            if (meshNames.size() != NULL) {
                for (int i = 0; i < meshNames.size(); i++) {

                    // Iterate through the mesh names
                    std::string& meshName = meshNames[i];

                    // Get the index of the selected item
                    if (ImGui::Selectable(meshName.c_str(), selectedItem == i)) {
                        selectedItem = i;

                        // Update the index for displaying specific component data
                        index = selectedItem;
                    }
                }
            }
            ImGui::EndListBox();
        }

        // Clear the array and add all the mesh names again (in case of changes)
        if (ImGui::Button("Repopulate List")) {
            meshNames.clear();
            doOnce = true;
        }
    }
    ImGui::End();

    Entity* currentEntity = entities[index];

    // get the specific instances for all components
    transformComponent = currentEntity->GetComponentByType<TransformComponent>();
    velocityComponent = currentEntity->GetComponentByType<VelocityComponent>();
    shaderComponent = currentEntity->GetComponentByType<ShaderComponent>();
    meshComponent = currentEntity->GetComponentByType<MeshComponent>();
    textureComponent = currentEntity->GetComponentByType<TextureComponent>();
    rigidBodyComponent = currentEntity->GetComponentByType<RigidBodyComponent>();
    soundComponent = currentEntity->GetComponentByType<SoundComponent>();

    ImGui::Begin("Misc.");
    ImGui::Text("Camera Position");
    ImGui::InputFloat("CamX", &cam->position.x);
    ImGui::InputFloat("CamY", &cam->position.y);
    ImGui::InputFloat("CamZ", &cam->position.z);
    ImGui::Separator();
    ImGui::Text("Index: %d", index);
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
    ImGui::Separator();
    ImGui::Checkbox("Draw Reticle", &drawReticle);
    ImGui::End();

    ImGui::Begin("Transform Component");
    if (transformComponent != nullptr) {

        ImGui::Text("Position");
        ImGui::InputFloat("PosX", &transformComponent->position.x);
        ImGui::InputFloat("PosY", &transformComponent->position.y);
        ImGui::InputFloat("PosZ", &transformComponent->position.z);
        ImGui::Separator();

        ImGui::Text("Rotation");
        ImGui::InputFloat("RotX", &transformComponent->rotation.x);
        ImGui::InputFloat("RotY", &transformComponent->rotation.y);
        ImGui::InputFloat("RotZ", &transformComponent->rotation.z);
        ImGui::Separator();

        ImGui::Text("Scale");
        ImGui::InputFloat("ScaleX", &transformComponent->scale.x);
        ImGui::InputFloat("ScaleY", &transformComponent->scale.y);
        ImGui::InputFloat("ScaleZ", &transformComponent->scale.z);
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
        ImGui::SameLine();
        ImGui::Checkbox("Is Paused", &soundComponent->isPaused);
    }
    ImGui::End();

    ImGui::Begin("Texture Component");
    if (textureComponent != nullptr) {
        if (textureComponent->textures[1] != "") {
            if (ImGui::BeginCombo("Textures", textureComponent->textures[selectedtexture].c_str())) {
                if (sizeof(textureComponent->textures) != NULL) {
                    for (int i = 0; i < IM_ARRAYSIZE(textureComponent->textures); i++) {
                        bool isSelected = (selectedtexture == i);

                        if (ImGui::Selectable(textureComponent->textures[i].c_str(), isSelected)) {
                            selectedtexture = i;
                        }
                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                }
                ImGui::EndCombo();
            }
        }
        else {
            ImGui::InputText("Texture0", (char*)textureComponent->textures[0].c_str(), 30);
        }
        ImGui::InputFloat("TexRatio0", &textureComponent->textureRatios[selectedtexture]);
        ImGui::Text("Format: ");
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
        ImGui::InputFloat("ColR", &textureComponent->rgbaColor.r);
        ImGui::InputFloat("ColG", &textureComponent->rgbaColor.g);
        ImGui::InputFloat("ColB", &textureComponent->rgbaColor.b);
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