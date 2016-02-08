#include "core/StringExt.h"

namespace core
{
namespace string
{
    /*
    Some home grown functions to do basic string manipulation.
    They are not supposed to be fast, just do their job.
    Should not be used in where high performance/memory usage matters.
    */

    std::string Replace(const std::string& source, const std::string& toReplace,
                        const std::string& replacement)
    {
        if (toReplace.empty()) return source;
        uint32_t toReplaceSize = toReplace.size();

        std::size_t prevIndex = 0, index = 0;
        std::string src;

        while (true) {
            index = source.find(toReplace, prevIndex);

            if (index == std::string::npos) {
                src += source.substr(prevIndex);
                break;
            }

            src += source.substr(prevIndex, index - prevIndex) + replacement;
            prevIndex = index + toReplaceSize;
        }

        return src;
    }

    std::string trim_end(const std::string& s, const std::string& delimiters)
    {
        return s.substr(0, s.find_last_not_of(delimiters) + 1);
    }

    std::string trim_begin(const std::string& s, const std::string& delimiters)
    {
        return s.substr(s.find_first_not_of(delimiters));
    }

    std::string trim(const std::string& s, const std::string& delimiters)
    {
        return s.substr(s.find_first_not_of(delimiters),
                        s.find_last_not_of(delimiters) + 1);
    }
}
}