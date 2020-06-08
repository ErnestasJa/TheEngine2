#ifndef THEPROJECT2_INPUTHANDLERHANDLE_H
#define THEPROJECT2_INPUTHANDLERHANDLE_H


namespace input {
class IInputDevice;
class InputHandler;
class [[nodiscard]] InputHandlerHandle
{
  public:
  InputHandlerHandle()
  {
  }
  InputHandlerHandle(input::IInputDevice * device, InputHandler * handler);

  private:
  InputHandler* m_handler;
  IInputDevice* m_device;
  core::SharedPtr<int> m_handleCount;
};
} // namespace input

#endif // THEPROJECT2_INPUTHANDLERHANDLE_H
