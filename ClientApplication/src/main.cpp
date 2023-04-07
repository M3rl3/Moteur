#include "Scene/Scene.h"
#include "../Moteur/Global.h"

// main function
int main(int argc, char** argv)
{
    Scene scene;

    scene.Initialize();
    scene.Render();

    while (!glfwWindowShouldClose(win->theWindow)) {
        scene.Update(scene.GetDeltaTime());
    }

    scene.Shutdown();

    return 0;
}