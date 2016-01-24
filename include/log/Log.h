#ifndef ENGINE_LOG_H
#define ENGINE_LOG_H

#include "ILogPipe.h"
namespace log
{
class TLog
{
public:
    static TLog& Get()
    {
        static TLog log;
        return log;
    }

public:
    void Log(const core::String& str)
    {
        for (auto wlogPipe : m_logPipes) {
            if (auto logPipe = wlogPipe.lock()) {
                logPipe->Log(str);
            }
        }
    }

    void AttachPipe(const core::WeakPtr<ILogPipe>& wlogPipe)
    {
        m_logPipes.push_back(wlogPipe);
    }

    void CleanDeadPipes()
    {
        auto it = std::begin(m_logPipes);

        while (it != std::end(m_logPipes)) {
            if (it->lock())
                ++it;
            else
                it = m_logPipes.erase(it);
        }
    }

private:
    core::Vector<core::WeakPtr<ILogPipe> > m_logPipes;
};
}

#endif