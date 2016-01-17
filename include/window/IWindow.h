#ifndef IWINDOW_H
#define IWINDOW_H

namespace render
{
class IWindow
{
public:
    virtual ~IWindow()
    {
    }

    virtual core::pod::Vec2<int32_t> GetDimensions() = 0;
    virtual core::pod::Vec2<int32_t> GetPosition() = 0;
    virtual bool PollEvents() = 0;
    virtual void SwapBuffers() = 0;
    virtual bool ShouldClose() = 0;
};
}

#endif