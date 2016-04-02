#include "window/WindowInc.h"
#include "input/InputInc.h"
#include <iostream>

class WindowInputHandler : public input::InputHandler
{
public:
    static auto Create(core::WeakPtr<render::IWindow> window)
    {
        return core::MakeShared<WindowInputHandler>(window);
    }

public:
    WindowInputHandler(core::WeakPtr<render::IWindow> window)
        : m_quit(false), m_window(window)
    {
    }

    virtual bool OnKeyDown(const input::Key &key,
                           const bool IsRepeated) override
    {
        if (key == input::Keys::Q || key == input::Keys::Esc) m_quit = true;

        if (auto wnd = m_window.lock()) {
            if (key == input::Keys::W) {
                auto dims = wnd->GetDimensions();
                dims.x += 20;
                dims.y += 20;
                wnd->SetDimensions(dims);
            } else if (key == input::Keys::E) {
                auto dims = wnd->GetDimensions();
                dims.x -= 20;
                dims.y -= 20;
                wnd->SetDimensions(dims);
            }
        }

        std::cout << "Key name:" << key.Name() << std::endl;
        return false;
    }

    bool AppQuit()
    {
        return m_quit;
    }

private:
    bool m_quit;
    core::WeakPtr<render::IWindow> m_window;
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

    auto handler = WindowInputHandler::Create(window);
    window->GetInputDevice().lock()->SetInputHandler(handler);

    while (window->ShouldClose() == false && handler->AppQuit() == false) {
        window->SwapBuffers();
        window->PollEvents();
    }

    wmodule->Finalize();

    return 0;
}