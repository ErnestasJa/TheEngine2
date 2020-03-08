#include "input/InputMapping.h"

namespace input
{
int Key::KeyIdentifierCounter = 0;
int MouseButton::ButtonIdentifierCounter = 0;

const Key Keys::A("A");
const Key Keys::B("B");
const Key Keys::C("C");
const Key Keys::D("D");
const Key Keys::E("E");
const Key Keys::F("F");
const Key Keys::G("G");
const Key Keys::H("H");
const Key Keys::I("I");
const Key Keys::J("J");
const Key Keys::K("K");
const Key Keys::L("L");
const Key Keys::M("M");
const Key Keys::N("N");
const Key Keys::O("O");
const Key Keys::P("P");
const Key Keys::Q("Q");
const Key Keys::R("R");
const Key Keys::S("S");
const Key Keys::T("T");
const Key Keys::U("U");
const Key Keys::V("V");
const Key Keys::W("W");
const Key Keys::X("X");
const Key Keys::Y("Y");
const Key Keys::Z("Z");
const Key Keys::SPACE("Space");
const Key Keys::L_CTRL("LeftControl");
const Key Keys::L_SHIFT("LeftShift");
const Key Keys::R_CTRL("RightControl");
const Key Keys::R_SHIFT("RightShift");
const Key Keys::GRAVE_ACCENT("GraveAccent");
const Key Keys::TAB("TAB");
const Key Keys::ESC("Esc");
const Key Keys::UNKNOWN("Unknown");

const MouseButton MouseButtons::Left("Left");
const MouseButton MouseButtons::Right("Right");
const MouseButton MouseButtons::UNKNOWN("Unknown");
}