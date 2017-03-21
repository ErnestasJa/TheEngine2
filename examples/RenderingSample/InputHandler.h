#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Camera.h"
#include "input/InputInc.h"
#include "window/WindowInc.h"

class CamInputHandler : public input::InputHandler
{
private:
    float m_speedPresets[3];
    int m_currentSpeedModifier;

    CamInputHandler(core::SharedPtr<Camera> cam) : m_cam(cam), m_speedPresets{0.2f, 1, 10}
    {
        m_mouseOld = m_mouseNew = {0, 0};
        m_currentSpeedModifier  = 0;
    }

public:
    static core::SharedPtr<CamInputHandler> Create(core::SharedPtr<Camera> cam)
    {
        return core::SharedPtr<CamInputHandler>(new CamInputHandler(cam));
    }

public:
    virtual bool OnKeyDown(const input::Key& key, const bool IsRepeated) override
    {
        const float MoveSpeed = m_speedPresets[m_currentSpeedModifier % 3];

        if (key == input::Keys::E && IsRepeated == false) {
            m_currentSpeedModifier++;
        }

        if (key == input::Keys::W) {
            m_cam->MoveForward(MoveSpeed);
        }

        if (key == input::Keys::S) {
            m_cam->MoveForward(-MoveSpeed);
        }
        if (key == input::Keys::A) {
            m_cam->MoveStrafe(-MoveSpeed);
        }
        if (key == input::Keys::D) {
            m_cam->MoveStrafe(MoveSpeed);
        }
        return true;
    }

    virtual bool OnMouseMove(const int32_t x, const int32_t y) override
    {
        const float MouseSpeed = 0.01;
        m_mouseOld             = m_mouseNew;
        m_mouseNew             = {x, y};

        auto rot = m_cam->GetRotation();

        rot.x -= ((m_mouseNew.x - m_mouseOld.x) * MouseSpeed);
        rot.y -= ((m_mouseNew.y - m_mouseOld.y) * MouseSpeed);

        m_cam->SetRotation(rot);

        return true;
    }

private:
    core::SharedPtr<Camera> m_cam;
    core::pod::Vec2<int32_t> m_mouseOld;
    core::pod::Vec2<int32_t> m_mouseNew;
};

#endif
