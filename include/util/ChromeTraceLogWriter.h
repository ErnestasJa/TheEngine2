#ifndef THEPROJECT2_CHROMETRACELOGWRITER_H
#define THEPROJECT2_CHROMETRACELOGWRITER_H

#include "Profiler.h"
#include "filesystem/IFileWriter.h"

namespace util {

class ChromeTraceLogWriter
{
  public:
  ChromeTraceLogWriter(core::UniquePtr<io::IFileWriter> fileWriter)
      : m_fileWriter(core::Move(fileWriter))
  {
  }

  void Write(const Profiler::Entry& entry)
  {
    ASSERT(entry.startTimestamp <= entry.endTimestamp, core::string::format("startTs: {}, endTs: {}", entry.startTimestamp, entry.endTimestamp));

    m_logBuffer +=
        core::string::format("{{\"name\":\"{}\",\"ph\":\"X\",\"ts\":{},\"dur\":{},\"pid\":1,\"tid\":{}}},",
                             entry.entryName, entry.startTimestamp,
                             entry.endTimestamp - entry.startTimestamp, entry.threadId);
  }

  void FlushLog(){
    ASSERT(m_fileWriter!=nullptr);

    m_fileWriter->Write("[");
    m_fileWriter->Write(m_logBuffer, m_logBuffer.size()-1);
    m_fileWriter->Write("]");
  }

  private:
  core::String m_logBuffer;
  core::UniquePtr<io::IFileWriter> m_fileWriter;
};

} // namespace util


#endif // THEPROJECT2_CHROMETRACELOGWRITER_H
