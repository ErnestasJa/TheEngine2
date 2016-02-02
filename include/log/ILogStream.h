#ifndef ILOG_PIPE_H
#define ILOG_PIPE_H

namespace log
{
class ILogStream
{
public:
    virtual void Log(const core::String& logString) = 0;
};
}

#endif