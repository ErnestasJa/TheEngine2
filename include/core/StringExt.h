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

    std::string Replace(const std::string& source, const std::string& toReplace,
                        const std::string& replacement);

    std::string TrimEnd(const std::string& s,
                        const std::string& delimiters = Whitespace);

    std::string TrimBegin(const std::string& s,
                          const std::string& delimiters = Whitespace);

    std::string Trim(const std::string& s,
                     const std::string& delimiters = Whitespace);
}
}

#endif