#ifndef COMMONUTIL_H
#define COMMONUTIL_H
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

core::SharedPtr<render::IRendererDebugMessageMonitor> GetDebugMessageMonitor()
{
    auto debugMonitor = render::GetRendererDebugMessageMonitor();

    if (debugMonitor) {
        debugMonitor->SetDebugging(true);
    } else {
        std::cout << "Failed to load debuging monitor" << std::endl;
    }

    return debugMonitor;
}

void LogEngine(const core::String &message)
{
    logging::Log(logging::LogSource::Engine, logging::LogSeverity::Debug,
                 message);
}

void LogDebugMessagesAndFlush(
    const core::SharedPtr<render::IRendererDebugMessageMonitor> &dbgMonitor)
{
    if (dbgMonitor)
        for (auto msg : dbgMonitor->GetMessages()) {
            logging::Log(logging::LogSource::Engine,
                         logging::LogSeverity::Debug, msg->AsString());
        }

    dbgMonitor->ClearMessages();
}

void LoadExtensions()
{
    auto extLoader = render::CreateExtensionLoader();

    if (extLoader->LoadExtensions())
        std::cout << "OpenGL extensions loaded." << std::endl;
    else
        std::cout << "Failed to load OpenGL extensions." << std::endl;
}

#endif