#include "GUISystem.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../Global.h"

GUISystem::GUISystem() 
{
    systemName = "GUISystem";
    drawReticle = false;
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

    // Render a gui here
    ImGui::Begin("Man");
    ImGui::Text("I Love Ass");
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