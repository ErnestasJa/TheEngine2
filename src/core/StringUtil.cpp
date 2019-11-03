

namespace core {
namespace string {

std::string Replace(const std::string& source, const std::string& toReplace,
                    const std::string& replacement)
{
    if (toReplace.empty())
        return source;
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

std::string TrimEnd(const std::string& s, const std::string& delimiters)
{
    if (s.size() == 0)
        return s;

    auto pos = s.find_last_not_of(delimiters);

    if (pos == core::String::npos)
        return core::String();

    return s.substr(0, pos + 1);
}

std::string TrimBegin(const std::string& s, const std::string& delimiters)
{
    if (s.size() == 0)
        return s;

    auto pos = s.find_first_not_of(delimiters);
    if (pos == core::String::npos)
        return core::String();

    return s.substr(pos);
}

std::string Trim(const std::string& s, const std::string& delimiters)
{
    if (s.size() == 0)
        return s;

    auto posBegin = s.find_first_not_of(delimiters);
    auto posEnd   = s.find_last_not_of(delimiters);

    if (posBegin == core::String::npos)
        return core::String();

    return s.substr(posBegin, (posEnd + 1) - posBegin);
}
}
}