#include "util/Timer.h"
#include <chrono>

namespace util {
Timer::Timer(){
  m_startTime = std::chrono::steady_clock::now();
}

void Timer::Start() {
  m_startTime = std::chrono::steady_clock::now();
}

int32_t Timer::MilisecondsElapsed() {
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_startTime).count();
  return ms;
}

int32_t Timer::MicrosecondsElapsed() {
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::microseconds>(now - m_startTime).count();
  return ms;
}

float Timer::SecondsElapsed() {
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::microseconds>(now - m_startTime).count();
  return ms / 1e6f;
}

uint32_t Timer::SecondsSinceEpoch() {
  return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

}