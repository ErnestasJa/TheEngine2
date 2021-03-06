#ifndef INPUTMAPPING_H
#define INPUTMAPPING_H

namespace input {
class Key
{
  public:
  Key(const core::String& name)
      : m_name(name)
      , m_id(KeyIdentifierCounter++)
  {
  }

  const core::String& Name() const
  {
    return m_name;
  }

  operator const core::String &() const
  {
    return m_name;
  }

  int GetId() const
  {
    return m_id;
  }

  private:
  core::String m_name;
  int m_id;

  private:
  static int KeyIdentifierCounter;
};

inline bool operator==(const Key& key, const core::String& str)
{
  return key.Name() == str;
}

struct Keys
{
  static const Key A;
  static const Key B;
  static const Key C;
  static const Key D;
  static const Key E;
  static const Key F;
  static const Key G;
  static const Key H;
  static const Key I;
  static const Key J;
  static const Key K;
  static const Key L;
  static const Key M;
  static const Key N;
  static const Key O;
  static const Key P;
  static const Key Q;
  static const Key R;
  static const Key S;
  static const Key T;
  static const Key U;
  static const Key V;
  static const Key W;
  static const Key X;
  static const Key Y;
  static const Key Z;
  static const Key SPACE;
  static const Key L_CTRL;
  static const Key R_CTRL;
  static const Key L_SHIFT;
  static const Key R_SHIFT;
  static const Key TAB;
  static const Key GRAVE_ACCENT;
  static const Key ESC;
  static const Key UNKNOWN;
};

enum MouseButtonAction
{
  Press,
  Release,
  Repeat
};

class MouseButton
{
  public:
  MouseButton(const core::String& name)
      : m_name(name)
      , m_id(ButtonIdentifierCounter++)
  {
  }

  const core::String& Name() const
  {
    return m_name;
  }

  operator const core::String &() const
  {
    return m_name;
  }

  int GetId() const
  {
    return m_id;
  }

  private:
  core::String m_name;
  int m_id;
  static int ButtonIdentifierCounter;
};

inline bool operator==(const MouseButton& button, const core::String& str)
{
  return button.Name() == str;
}

struct MouseButtons
{
  static const MouseButton Left;
  static const MouseButton Right;
  static const MouseButton UNKNOWN;
};
} // namespace input

#endif