

namespace core {
namespace string {

core::String Replace(const core::String& source, const core::String& toReplace,
                     const core::String& replacement)
{
  if (toReplace.empty())
    return source;
  uint32_t toReplaceSize = toReplace.size();

  std::size_t prevIndex = 0, index = 0;
  core::String src;

  while (true) {
    index = source.find(toReplace, prevIndex);

    if (index == core::String::npos) {
      src += source.substr(prevIndex);
      break;
    }

    src += source.substr(prevIndex, index - prevIndex) + replacement;
    prevIndex = index + toReplaceSize;
  }

  return src;
}

core::String TrimEnd(const core::String& s, const core::String& delimiters)
{
  if (s.size() == 0)
    return s;

  auto pos = s.find_last_not_of(delimiters);

  if (pos == core::String::npos)
    return core::String();

  return s.substr(0, pos + 1);
}

core::String TrimBegin(const core::String& s, const core::String& delimiters)
{
  if (s.size() == 0)
    return s;

  auto pos = s.find_first_not_of(delimiters);
  if (pos == core::String::npos)
    return core::String();

  return s.substr(pos);
}

core::String Trim(const core::String& s, const core::String& delimiters)
{
  if (s.size() == 0)
    return s;

  auto posBegin = s.find_first_not_of(delimiters);
  auto posEnd   = s.find_last_not_of(delimiters);

  if (posBegin == core::String::npos)
    return core::String();

  return s.substr(posBegin, (posEnd + 1) - posBegin);
}
} // namespace string
} // namespace core