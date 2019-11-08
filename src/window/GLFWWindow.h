#ifndef GLFWWINDOW_H
#define GLFWWINDOW_H

#include "window/IWindow.h"
#include "window/SWindowDefinition.h"

struct GLFWwindow;

namespace render {
class GLFWWindow : public IWindow
{
public:
    GLFWWindow();
    virtual ~GLFWWindow();
    virtual bool Init(const SWindowDefinition& windowDefinition);
    virtual core::pod::Vec2<int32_t> GetDimensions();
    virtual void SetDimensions(const core::pod::Vec2<int32_t>& dimensions);
    virtual core::pod::Vec2<int32_t> GetPosition();
    virtual void SetPosition(const core::pod::Vec2<int32_t>& position);
    virtual bool PollEvents();
    virtual void SwapBuffers();
    virtual bool ShouldClose();
    virtual void Close();
    virtual input::IInputDevice* GetInputDevice();
    virtual const SWindowDefinition& GetWindowDefinition();
    virtual void UpdateContext(); /// this is to be called after creating
                                  /// window and loading extensions.
    virtual void SetCursorMode(CursorMode cursorMode);
    GLFWwindow* GetUnderlyingWindow();

private:
    SWindowDefinition m_windowDefinition;
    GLFWwindow* m_window;
    core::SharedPtr<input::IInputDevice> m_inputDevice;
    bool m_close;
};
}

#endif
