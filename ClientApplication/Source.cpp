#include "Engine.h"

int main(int argc, char** argv) {

    Engine::Engine_CreateWindow("Engine", 1366, 768, false, false);

    Engine::Engine_Initialize();


    //Engine::Engine_Render();

    while (true) {
        Engine::Engine_Update();
    }

    Engine::Engine_Shutdown();

    return 0;
}