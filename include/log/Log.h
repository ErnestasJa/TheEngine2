#ifndef ENGINE_LOG_H
#define ENGINE_LOG_H

#include "ILogStream.h"
namespace elog {
void Log(const LogSource source, const LogSeverity severity, const core::String& log);
void AddLogStream(const core::WeakPtr<ILogStream>& wlogStream);
void CleanDeadStreams();

// shorthand methods for engine logs
inline void LogInfo(const core::String& log)
{
    Log(LogSource::Engine, LogSeverity::Info, log);
}
inline void LogWarning(const core::String& log)
{
    Log(LogSource::Engine, LogSeverity::Warn, log);
}
inline void LogError(const core::String& log)
{
    Log(LogSource::Engine, LogSeverity::Error, log);
}
}

#endif