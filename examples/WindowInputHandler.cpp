#include "window/WindowInc.h"
#include "input/InputInc.h"
#include <iostream>

class WindowInputHandler : public core::InputHandler
{
public:
    static auto Create()
    {
        return core::MakeShared<WindowInputHandler>();
    }

public:
    WindowInputHandler() : m_quit(false)
    {
    }

    virtual bool OnKeyDown(const core::Key &key, const bool IsRepeated) override
    {
        if (key == core::Keys::Q) m_quit = true;

        return false;
    }

    bool AppQuit()
    {
        return m_quit;
    }

private:
    bool m_quit;
};

int main(int argc, char const *argv[])
{
    auto wmodule = render::CreateDefaultWindowModule();
    wmodule->Initialize();

    render::SWindowDefinition wDef;
    wDef.Dimensions = {1280, 720};
    wDef.Title = "Window input test app. Press 'q' to quit.";

    auto window = wmodule->CreateWindow(wDef);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    auto handler = WindowInputHandler::Create();
    window->GetInputDevice().lock()->SetInputHandler(handler);

    while (window->ShouldClose() == false && handler->AppQuit() == false) {
        window->SwapBuffers();
        window->PollEvents();
    }

    wmodule->Finalize();

    return 0;
}