#ifndef ILOG_PIPE_H
#define ILOG_PIPE_H

#include "ELogSeverity.h"
#include "ELogSource.h"

namespace elog {
class ILogStream
{
  public:
    virtual ~ILogStream() = default;
  virtual void Log(const LogSource source, const LogSeverity severity,
                   const core::String& logString) = 0;
};
} // namespace elog

#endif