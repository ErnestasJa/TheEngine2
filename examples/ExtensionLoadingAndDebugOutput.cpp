#include "window/WindowInc.h"
#include "render/RenderInc.h"
#include "log/LogInc.h"
#include <iostream>

class EngineCoutPipe : public elog::ILogStream
{
public:
    void Log(const elog::LogSource source, const elog::LogSeverity severity,
             const core::String &logStr)
    {
        if (source == elog::LogSource::Engine)
            std::cout << "Engine log: " << logStr << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<EngineCoutPipe>();
    elog::AddLogStream(engineLogStream);

    render::SWindowDefinition wDef;
    wDef.Dimensions = {1280, 720};
    wDef.Title = "Window example application";
    wDef.DebugContext = true;

    auto context = render::CreateContext(wDef);
    auto window = context->GetWindow().get();
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

    return 0;
}