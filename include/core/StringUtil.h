#ifndef STRING_EXT_H
#define STRING_EXT_H

#include <cstdio>

namespace core {
namespace string {
static const String Whitespace   = " \f\r\n\t\v";
static const char NullTerminator = '\0';

template <class... T, int32_t BufferSize = 2048> String format(const char* fmt, const T&... t)
{
    char buffer[BufferSize];

    snprintf(buffer, BufferSize-1, fmt, t...);
    return buffer;
}


String Replace(const String& source, const String& toReplace, const String& replacement);
String TrimEnd(const String& source, const String& delimiters = Whitespace);
String TrimBegin(const String& source, const String& delimiters = Whitespace);
String Trim(const String& source, const String& delimiters = Whitespace);

inline bool EndsWith(const core::String & value, const core::String & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}
}
}

#endif