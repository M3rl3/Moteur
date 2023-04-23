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
#include "../Components/CharacterControllerComponent.h"
#include "../Components/SoundComponent.h"

#include "../PhysicsEngine/interfaces/ShapeType.h"

#include "../Global.h"

#include <glm/gtc/type_ptr.hpp>

// list box stuff
int selectedItem;
bool isExpanded;

int selectedtexture;

ImDrawList* drawlist;

GUISystem::GUISystem() 
{
    systemName = "GUISystem";
    drawReticle = false;
    index = 0;
    doOnce = true;

    selectedItem = 0;
    isExpanded = false;

    selectedtexture = 0;

    drawlist;
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
    CharacterControllerComponent* characterController = nullptr;
    SoundComponent* soundComponent = nullptr;

    MeshComponent* meshComponent_player = nullptr;

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

                    if (meshName == "plane") {
                        continue;
                    }
                    
                    if (meshName == "tree1") {
                        continue;
                    }

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
    characterController = currentEntity->GetComponentByType<CharacterControllerComponent>();
    soundComponent = currentEntity->GetComponentByType<SoundComponent>();

    ImGui::Begin("Misc.");
    ImGui::Text("Camera");
    ImGui::InputFloat3("5", glm::value_ptr(cam->position));
    ImGui::Separator();
    /*ImGui::Text("Index: %d", index);
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
    ImGui::Separator();*/
    ImGui::Checkbox("Draw Reticle", &drawReticle);
    ImGui::End();

    ImGui::Begin("Health Bar");
    meshComponent_player = GetPlayerMesh(entities);

    ImVec2 barSize(200, 20);

    // Draw the progress bar
    ImGui::ProgressBar(meshComponent_player->health, barSize);
    ImGui::End();
    
    ImGui::Begin("Transform Component");
    if (transformComponent != nullptr) {

        ImGui::Text("Position");
        ImGui::InputFloat3("0", glm::value_ptr(transformComponent->position));
        ImGui::Separator();

        ImGui::Text("Rotation");
        ImGui::InputFloat3("1", glm::value_ptr(transformComponent->rotation));
        ImGui::Separator();

        ImGui::Text("Scale");
        ImGui::InputFloat3("2", glm::value_ptr(transformComponent->scale));
    }
    ImGui::End();

    ImGui::Begin("Mesh Component");
    if (meshComponent != nullptr) {
        char buffer[30];
        strcpy_s(buffer, meshComponent->meshName.c_str());
        ImGui::InputText("Mesh Name", buffer, 30);
    
        if (meshComponent->meshName.c_str() != buffer) {
            meshComponent->meshName = buffer;
        }

        ImGui::Checkbox("Visible", &meshComponent->isVisible);
        ImGui::SameLine();
        ImGui::Checkbox("Wireframe", &meshComponent->isWireframe);
    }
    ImGui::End();

    ImGui::Begin("Velocity Component");
    if (velocityComponent != nullptr) {
        ImGui::Text("Velocity");
        ImGui::InputFloat3("3", glm::value_ptr(velocityComponent->velocity));
        ImGui::Checkbox("Use Velocity", &velocityComponent->useVelocity);
    }
    ImGui::End();

    ImGui::Begin("Rigidbody Component");
    if (rigidBodyComponent != nullptr) {
        ImGui::Checkbox("Use Physics", &rigidBodyComponent->usePhysics);

        ImGui::Text("Body Shape: ");
        ImGui::SameLine();

        switch (rigidBodyComponent->bodyShape->GetShapeType())
        {
            case physics::ShapeType::Box: {
                ImGui::Text("Box");
                break;
            }
            case physics::ShapeType::Capsule: {
                ImGui::Text("Capsule");
                break;
            }
            case physics::ShapeType::Cylinder: {
                ImGui::Text("Cylinder");
                break;
            }
            case physics::ShapeType::Ghost: {
                ImGui::Text("Ghost");
                break;
            }
            case physics::ShapeType::Plane: {
                ImGui::Text("Plane");
                break;
            }
            case physics::ShapeType::Sphere: {
                ImGui::Text("Sphere");
                break;
            }
            default: {
                // No idea what this shape is
                ImGui::Text("Unknown Shape");
                break;
            }     
        }
    }
    ImGui::End();

    ImGui::Begin("Character Controller Component");
    if (characterController != nullptr) {
        ImGui::InputFloat("Step Height", &characterController->stepHeight);
        ImGui::Checkbox("Is Controllable", &characterController->isControllable);
        ImGui::Checkbox("Can Jump", &characterController->canJump);
    }
    ImGui::End();

    ImGui::Begin("Sound Component");
    if (soundComponent != nullptr) {
        char buffer[30];
        strcpy_s(buffer, soundComponent->soundName.c_str());
        ImGui::InputText("Sound Name", buffer, 30);
        /*if (soundComponent->soundName.c_str() != buffer) {
            soundComponent->soundName = buffer;
        }*/
        ImGui::InputFloat("Volume", &soundComponent->soundVolume);
        ImGui::InputFloat("Falloff", &soundComponent->maxDistance);
        ImGui::Checkbox("Is Playing", &soundComponent->isPlaying);
        ImGui::SameLine();
        ImGui::Checkbox("Is Paused", &soundComponent->isPaused);
    }
    ImGui::End();

    ImGui::Begin("Texture Component");
    if (textureComponent != nullptr) {

        if (ImGui::BeginCombo("Textures", textureComponent->textures[selectedtexture].c_str())) {
            for (int i = 0; i < 8; i++) {

                // Only add the texture strings that are initialized
                if (!textureComponent->textures[i].empty()) {
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
        
        ImGui::InputFloat("TexRatio", &textureComponent->textureRatios[selectedtexture]);
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
        ImGui::InputFloat3("4", glm::value_ptr(textureComponent->rgbaColor));
        ImGui::Checkbox("Use Color", &textureComponent->useRGBAColor);
        ImGui::Separator();
        ImGui::Checkbox("isReflective", &textureComponent->isReflective);
        ImGui::SameLine();
        ImGui::Checkbox("isTransparent", &textureComponent->isTransparent);
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

MeshComponent* GUISystem::GetPlayerMesh(const std::vector<Entity*>& entities)
{
    MeshComponent* meshComponent = nullptr;

    for (Entity* entity : entities) {

        meshComponent = entity->GetComponentByType<MeshComponent>();

        if (meshComponent->isPlayer) {
            return meshComponent;
        }
    }

    return nullptr;
}