#ifndef COMMONUTIL_H
#define COMMONUTIL_H
#include "render/RenderInc.h"
#include "log/LogInc.h"
#include <iostream>

class EngineCoutPipe : public log::ILogStream
{
public:
    void Log(const log::LogSource source, const log::LogSeverity severity,
             const core::String &logStr)
    {
        if (source == log::LogSource::Engine)
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

void LogDebugMessagesAndFlush(
    const core::SharedPtr<render::IRendererDebugMessageMonitor> &dbgMonitor)
{
    if (dbgMonitor)
        for (auto msg : dbgMonitor->GetMessages()) {
            log::Log(log::LogSource::Engine, log::LogSeverity::Debug,
                     msg->AsString());
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