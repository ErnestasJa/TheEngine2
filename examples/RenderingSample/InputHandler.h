#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "input/InputInc.h"
#include "window/WindowInc.h"
#include "Camera.h"

class CamInputHandler : public input::InputHandler
{
private:
    CamInputHandler(core::SharedPtr<Camera> cam) : m_cam(cam)
    {
        m_mouseOld = m_mouseNew = {0, 0};
    }

public:
    static core::SharedPtr<CamInputHandler> Create(core::SharedPtr<Camera> cam)
    {
        return core::SharedPtr<CamInputHandler>(new CamInputHandler(cam));
    }

public:
    virtual bool OnKeyDown(const input::Key &key,
                           const bool IsRepeated) override
    {
        if (key == input::Keys::W) {
            m_cam->MoveForward(1);
        }

        if (key == input::Keys::S) {
            m_cam->MoveForward(-1);
        }
        if (key == input::Keys::A) {
            m_cam->MoveStrafe(-1);
        }
        if (key == input::Keys::D) {
            m_cam->MoveStrafe(1);
        }
        return true;
    }

    virtual bool OnMouseMove(const int32_t x, const int32_t y) override
    {
        m_mouseOld = m_mouseNew;
        m_mouseNew = {x, y};

        printf("Awww shiet: %i, %i\n", x, y);

        auto rot = m_cam->GetRotation();

        rot.x -= ((m_mouseNew.x - m_mouseOld.x) * 0.01);
        rot.y -= ((m_mouseNew.y - m_mouseOld.y) * 0.01);

        m_cam->SetRotation(rot);

        return true;
    }

private:
    core::SharedPtr<Camera> m_cam;
    core::pod::Vec2<int32_t> m_mouseOld;
    core::pod::Vec2<int32_t> m_mouseNew;
};

#endif