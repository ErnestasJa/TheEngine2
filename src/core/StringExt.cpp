#include "core/StringExt.h"

namespace core
{
namespace string
{
    std::string Replace(const std::string& source, const std::string& toReplace,
                        const std::string& replacement)
    {
        std::size_t index = 0;
        std::string src = source;

        while (true) {
            index = src.find(toReplace, index);
            if (index == std::string::npos) break;

            src.replace(index, replacement.size(), replacement);
            index += replacement.size();
        }

        return src;
    }
}
}