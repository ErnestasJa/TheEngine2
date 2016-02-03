#ifndef ENGINE_LOG_H
#define ENGINE_LOG_H

#include "ILogStream.h"
namespace log
{
    void Log(const LogSource source, const core::String & log);
    void AddLogStream(const core::WeakPtr<ILogStream>& wlogStream);
    void CleanDeadStreams();
}

#endif