#ifndef ILOG_PIPE_H
#define ILOG_PIPE_H

#include "ELogSeverity.h"
#include "ELogSource.h"

namespace log
{
class ILogStream
{
public:
    virtual void Log(const LogSource source, const LogSeverity severity,
                     const core::String& logString) = 0;
};
}

#endif