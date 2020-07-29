#ifndef THEPROJECT2_INPUTHANDLERHANDLE_H
#define THEPROJECT2_INPUTHANDLERHANDLE_H


namespace input {
class IInputDevice;
class InputHandler;
class [[nodiscard]] InputHandlerHandle
{
  public:
  InputHandlerHandle(): m_handler(nullptr), m_device(nullptr)
  {
  }
  InputHandlerHandle(input::IInputDevice * device, InputHandler * handler);

  void Disconnect();

  private:
  InputHandler* m_handler;
  IInputDevice* m_device;
};
} // namespace input

#endif // THEPROJECT2_INPUTHANDLERHANDLE_H
