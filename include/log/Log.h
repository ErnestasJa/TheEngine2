#ifndef ENGINE_LOG_H
#define ENGINE_LOG_H

#include "ILogStream.h"
namespace log
{
class Logger
{
public:
    static Logger& Get()
    {
        static Logger log;
        return log;
    }

public:
    void Log(const core::String& str)
    {
        for (auto wlogStream : m_logStreams) {
            if (auto logPipe = wlogStream.lock()) {
                logPipe->Log(str);
            }
        }
    }

    void AttachStream(const core::WeakPtr<ILogStream>& wlogStream)
    {
        m_logStreams.push_back(wlogStream);
    }

    void CleanDeadStreams()
    {
        auto it = std::begin(m_logStreams);

        while (it != std::end(m_logStreams)) {
            if (it->lock())
                ++it;
            else
                it = m_logStreams.erase(it);
        }
    }

private:
    core::Vector<core::WeakPtr<ILogStream> > m_logStreams;
};

void Log(const core::String &str ){
    Logger::Get().Log( str );
}
}

#endif