#ifndef THEPROJECT2_INCLUDE_TIMER_H_
#define THEPROJECT2_INCLUDE_TIMER_H_

#include <chrono>
namespace util {
class Timer {
public:
  Timer();
  void Start();
  int32_t MilisecondsElapsed();
  int32_t MicrosecondsElapsed();
  float SecondsElapsed();
  uint32_t SecondsSinceEpoch();
private:
  std::chrono::steady_clock::time_point m_startTime;
};
}

#endif // THEPROJECT2_INCLUDE_TIMER_H_
