#ifndef STRING_EXT_H
#define STRING_EXT_H

#include <cstdio>
#include <glm/vec3.hpp>
#include "fmt/format.h"

template <>
struct fmt::formatter<glm::vec3> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const glm::vec3 &d, FormatContext &ctx) {
        return format_to(ctx.out(), "vec3({:03.2f}, {:03.2f}, {:03.2f})", d.x, d.y, d.z);
    }
};

namespace core {
namespace string {
static const String Whitespace   = " \f\r\n\t\v";
static const char NullTerminator = '\0';


template <class... T, int32_t BufferSize = 2048> String format(const char* fmt, const T&... t)
{
    return fmt::format(fmt, t...);
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