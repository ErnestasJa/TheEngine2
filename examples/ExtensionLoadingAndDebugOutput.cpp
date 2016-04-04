#include "window/WindowInc.h"
#include "render/RenderInc.h"
#include "log/LogInc.h"
#include <iostream>

class EngineCoutPipe : public logging::ILogStream
{
public:
    void Log(const logging::LogSource source,
             const logging::LogSeverity severity, const core::String &logStr)
    {
        if (source == logging::LogSource::Engine)
            std::cout << "Engine log: " << logStr << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<EngineCoutPipe>();
    logging::AddLogStream(engineLogStream);

    auto wmodule = render::CreateDefaultWindowModule();
    wmodule->Initialize();

    render::SWindowDefinition wDef;
    wDef.Dimensions = {1280, 720};
    wDef.Title = "Window example application";
    wDef.DebugContext = true;
    wDef.ForwardCompatible = true;
    wDef.ContextMajorVersion = 3;
    wDef.ContextMinorVersion = 3;

    auto window = wmodule->CreateWindow(wDef);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    auto extLoader = render::CreateExtensionLoader();

    if (extLoader->LoadExtensions())
        std::cout << "OpenGL extensions loaded." << std::endl;
    else
        std::cout << "Failed to load OpenGL extensions." << std::endl;

    auto debugMonitor = render::GetRendererDebugMessageMonitor();

    if (debugMonitor) {
        debugMonitor->SetDebugging(true);
    } else {
        std::cout << "Failed to load debuging monitor" << std::endl;
    }

    if (debugMonitor) {
        for (const auto &msg : debugMonitor->GetMessages()) {
            std::cout << "Debug message: " << msg->AsString() << std::endl;
        }

        debugMonitor->ClearMessages();
    }

    while (window->ShouldClose() == false) {
        window->SwapBuffers();
        window->PollEvents();
    }

    wmodule->Finalize();

    return 0;
}