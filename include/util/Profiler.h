#ifndef THEPROJECT2_PROFILER_H
#define THEPROJECT2_PROFILER_H
#include "util/Timer.h"

namespace util {
class Profiler {
  public:
    struct Entry {
      const char * entryName{};
      uint32_t startTimestamp{};
      uint32_t endTimestamp{};
      uint32_t threadId{};

      Entry()= default;

      Entry(const char * _entryName, uint32_t _startTimestamp, uint32_t _threadId){
        entryName = _entryName;
        startTimestamp = _startTimestamp;
        endTimestamp = _startTimestamp;
        threadId = _threadId;
      }

      Entry(const char * _entryName, uint32_t _startTimestamp, uint32_t _endTimestamp, uint32_t _threadId){
        entryName = _entryName;
        startTimestamp = _startTimestamp;
        endTimestamp = _endTimestamp;
        threadId = _threadId;
      }

    };
  public:

    Profiler(){
      m_timer.Start();
    }

    void Start(const char * entryName){
      m_entryStack.emplace(entryName, m_timer.MicrosecondsElapsed(), 1);
    }

    void Stop(){
      ASSERT(m_entryStack.empty() == false);

      auto time = m_timer.MicrosecondsElapsed();
      Entry e = m_entryStack.top();
      m_entryStack.pop();
      m_entries.emplace_back(e.entryName, e.startTimestamp, time, e.threadId);
    }

    template <class TLogWriter>
    void WriteLog(TLogWriter& writer){
      ASSERT(m_entryStack.empty());

      for(const auto & entry: m_entries){
        writer.Write(entry);
      }
    }

  private:
    util::Timer m_timer;
    core::Vector<Entry> m_entries;
    core::Stack<Entry> m_entryStack;
};
}


#endif // THEPROJECT2_PROFILER_H
