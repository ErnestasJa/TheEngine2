#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <iostream>
#include <chrono>
#include "filesystem/Path.h"

using namespace std::literals::string_literals;

::std::ostream &operator<<(::std::ostream &os, const io::Path &path)
{
    return os << path.AsString().c_str();
}

namespace Common
{
auto GetTimestamp()
{
    return std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
}

std::string GetTimestampString()
{
    return std::to_string(GetTimestamp());
}
}

#endif
