#include "GLFW/glfw3.h"
#include "input/InputMapping.h"

namespace input {
inline const Key& MapKey(int32_t key)
{
    switch (key) {
    case GLFW_KEY_A:
        return Keys::A;
    case GLFW_KEY_B:
        return Keys::B;
    case GLFW_KEY_C:
        return Keys::C;
    case GLFW_KEY_D:
        return Keys::D;
    case GLFW_KEY_E:
        return Keys::E;
    case GLFW_KEY_F:
        return Keys::F;
    case GLFW_KEY_G:
        return Keys::G;
    case GLFW_KEY_H:
        return Keys::H;
    case GLFW_KEY_I:
        return Keys::I;
    case GLFW_KEY_J:
        return Keys::J;
    case GLFW_KEY_K:
        return Keys::K;
    case GLFW_KEY_L:
        return Keys::L;
    case GLFW_KEY_M:
        return Keys::M;
    case GLFW_KEY_N:
        return Keys::N;
    case GLFW_KEY_O:
        return Keys::O;
    case GLFW_KEY_P:
        return Keys::P;
    case GLFW_KEY_Q:
        return Keys::Q;
    case GLFW_KEY_R:
        return Keys::R;
    case GLFW_KEY_S:
        return Keys::S;
    case GLFW_KEY_T:
        return Keys::T;
    case GLFW_KEY_U:
        return Keys::U;
    case GLFW_KEY_V:
        return Keys::V;
    case GLFW_KEY_W:
        return Keys::W;
    case GLFW_KEY_X:
        return Keys::X;
    case GLFW_KEY_Y:
        return Keys::Y;
    case GLFW_KEY_Z:
        return Keys::Z;
    case GLFW_KEY_SPACE:
        return Keys::Space;
    case GLFW_KEY_ESCAPE:
        return Keys::Esc;
    default:
        return Keys::Unknown;
    }
}

inline const MouseButton& MapMouseButton(int32_t button)
{
	switch (button) {
	case GLFW_MOUSE_BUTTON_RIGHT:
		return MouseButtons::Right;
	case GLFW_MOUSE_BUTTON_LEFT:
		return MouseButtons::Left;
	default:
		return MouseButtons::Unknown;
	}
}
}