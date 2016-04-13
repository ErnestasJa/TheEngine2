#ifndef STRING_EXT_H
#define STRING_EXT_H

#include <cstdio>

namespace core
{
namespace string
{
    static const String Whitespace = " \f\r\n\t\v";
    static const char NullTerminator = '\0';

    template <class... T, int32_t BufferSize = 4096>
    String CFormat(const char* fmt, const T&... t)
    {
        char buffer[BufferSize];
        snprintf(buffer, BufferSize, fmt, t...);
        return buffer;
    }

    String Replace(const String& source, const String& toReplace,
                   const String& replacement);
    String TrimEnd(const String& source, const String& delimiters = Whitespace);
    String TrimBegin(const String& source,
                     const String& delimiters = Whitespace);
    String Trim(const String& source, const String& delimiters = Whitespace);
}
}

#endif