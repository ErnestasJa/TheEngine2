#ifndef IWINDOW_MODULE_H
#define IWINDOW_MODULE_H

namespace render
{
class IWindow;
struct SWindowDefinition;

class IWindowModule
{
public:
    virtual ~IWindowModule()
    {
    }

    virtual core::SharedPtr<IWindow> CreateWindow(
        const SWindowDefinition& windowDefinition) = 0;
};

core::SharedPtr<IWindowModule> CreateDefaultWindowModule();
}

#endif