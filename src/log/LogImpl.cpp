#include "log/Log.h"

namespace elog {
namespace {
class Logger
{
  public:
  static Logger& Get()
  {
    static Logger log;
    return log;
  }

  public:
  void Log(const LogSource source, const LogSeverity severity, const core::String& str)
  {
    for (auto wlogStream : m_logStreams) {
      if (auto logPipe = wlogStream.lock()) {
        logPipe->Log(source, severity, str);
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
  core::Vector<core::WeakPtr<ILogStream>> m_logStreams;
};
} // namespace

void Log(const LogSource source, const LogSeverity severity, const core::String& log)
{
  Logger::Get().Log(source, severity, log);
}

void AddLogStream(const core::WeakPtr<ILogStream>& wlogStream)
{
  Logger::Get().AttachStream(wlogStream);
}

void CleanDeadStreams()
{
  Logger::Get().CleanDeadStreams();
}
} // namespace elog