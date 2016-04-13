#include "window/WindowInc.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    auto wmodule = render::CreateDefaultWindowModule();

    render::SWindowDefinition wDef;
    wDef.Dimensions = {1280, 720};
    wDef.Title = "Window example application";

    auto window = wmodule->CreateWindow(wDef);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    while (window->ShouldClose() == false) {
        window->SwapBuffers();
        window->PollEvents();
    }

    return 0;
}