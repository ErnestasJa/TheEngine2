#ifndef GENERIC_INPUT_HANDLER_H
#define GENERIC_INPUT_HANDLER_H

namespace core
{
class InputHandler
{
public:
    virtual ~InputHandler()
    {
    }

    virtual bool OnKeyDown(const int32_t KeyCode, const uint32_t CharacterCode,
                           const bool IsRepeated)
    {
        return false;
    }
    virtual bool OnKeyUp(const int32_t KeyCode, const uint32_t CharacterCode,
                         const bool IsRepeated)
    {
        return false;
    }

    virtual bool OnMouseMove(const int32_t x, const int32_t y)
    {
        return false;
    }

    virtual bool OnMouseWheel(const float delta)
    {
        return false;
    }
};
}

#endif