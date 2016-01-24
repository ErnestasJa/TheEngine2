#ifndef ILOG_PIPE_H
#define ILOG_PIPE_H

namespace log
{
class ILogPipe
{
public:
    virtual void Log(const core::String& logString) = 0;
};
}

#endif