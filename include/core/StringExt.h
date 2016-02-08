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

    std::string Replace(const std::string& source, const std::string& toReplace,
                        const std::string& replacement);

    std::string trim_end(const std::string& s,
                         const std::string& delimiters = " \f\n\r\t\v");

    std::string trim_begin(const std::string& s,
                           const std::string& delimiters = " \f\n\r\t\v");

    std::string trim(const std::string& s,
                     const std::string& delimiters = " \f\n\r\t\v");
}
}

#endif