#ifndef GENERIC_INPUT_HANDLER_H
#define GENERIC_INPUT_HANDLER_H

namespace input
{
class Key;
class MouseButton;
class InputHandler
{
public:
    virtual ~InputHandler()
    {
    }

	virtual bool OnMouseDown(const MouseButton& key)
	{
		return false;
	}

	virtual bool OnMouseUp(const MouseButton& key)
	{
		return false;
	}

    virtual bool OnKeyDown(const Key& key, const bool repeated)
    {
        return false;
    }

    virtual bool OnKeyUp(const Key& key, const bool repeated)
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