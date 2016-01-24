#ifndef STRING_EXT_H
#define STRING_EXT_H

#include <cstdio>

namespace core
{
namespace string
{
    template <class... T, int32_t BufferSize = 4096>
    String CFormat(const char* fmt, const T&... t)
    {
        char buffer[BufferSize];
        snprintf(buffer, BufferSize, fmt, t...);
        return buffer;
    }
}
}

#endif