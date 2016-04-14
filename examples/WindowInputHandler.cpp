#include "window/WindowInc.h"
#include "render/RenderInc.h"
#include "input/InputInc.h"
#include <iostream>

class WindowInputHandler : public input::InputHandler
{
public:
    static auto Create(render::IWindow *window)
    {
        return core::MakeShared<WindowInputHandler>(window);
    }

public:
    WindowInputHandler(render::IWindow *window)
        : m_quit(false), m_window(window)
    {
    }

    virtual bool OnKeyDown(const input::Key &key,
                           const bool IsRepeated) override
    {
        if (key == input::Keys::Q || key == input::Keys::Esc) m_quit = true;

        if (m_window) {
            if (key == input::Keys::W) {
                auto dims = m_window->GetDimensions();
                dims.x += 20;
                dims.y += 20;
                m_window->SetDimensions(dims);
            } else if (key == input::Keys::E) {
                auto dims = m_window->GetDimensions();
                dims.x -= 20;
                dims.y -= 20;
                m_window->SetDimensions(dims);
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
    render::IWindow *m_window;
};

int main(int argc, char const *argv[])
{
    render::SWindowDefinition wDef;
    wDef.Dimensions = {1280, 720};
    wDef.Title = "Window input test app. Press 'q' to quit.";
    auto context = render::CreateContext(wDef);
    auto window = context->GetWindow().get();

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

    return 0;
}