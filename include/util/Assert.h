#ifndef THEPROJECT2_ASSERT_H
#define THEPROJECT2_ASSERT_H

#include "Config.h"

#if ENGINE_DEBUG
#include <cstdlib>
#endif

#define ASSERT_NO_MSG(x)                                                                           \
  if_debug                                                                                         \
  {                                                                                                \
    if (!(x)) {                                                                                    \
      elog::LogError(core::string::format("Assertion failed <{}> at: {}:{} , func: {}", #x,  \
                                          __FILE__, __LINE__, __FUNCTION__));                      \
      std::abort();                                                                                \
    }                                                                                              \
  }


#define ASSERT_MSG(x, msg)                                                                         \
  if_debug                                                                                         \
  {                                                                                                \
    if (!(x)) {                                                                                    \
      auto str = core::String((msg));                                                              \
      elog::LogError(                                                                              \
          core::string::format("Assertion failed <{}> at: {}:{} , func: {}\nMessage:{}", #x, \
                               __FILE__, __LINE__, __FUNCTION__, str.c_str()));                    \
      std::abort();                                                                                \
    }                                                                                              \
  }

#define ASSERT_X(x, y, z, FUNC, ...) FUNC

#define ASSERT(...) ASSERT_X(, ##__VA_ARGS__, ASSERT_MSG(__VA_ARGS__), ASSERT_NO_MSG(__VA_ARGS__), )

#endif // THEPROJECT2_ASSERT_H
