#include "filesystem/Path.h"
#include <algorithm>

namespace io {
namespace path {
core::String Append(const core::String& path, const core::String& pathToAppend)
{
  auto size       = path.size();
  auto appendSize = pathToAppend.size();

  if (size > 0 && appendSize > 0) {
    if (path[size - 1] == Separator && pathToAppend[0] == Separator)
      return core::String(path).append(pathToAppend, 1, core::String::npos);
    if (path[size - 1] == Separator || pathToAppend[0] == Separator)
      return path + pathToAppend;
    else
      return path + Separator + pathToAppend;
  }

  return appendSize > 0 ? pathToAppend : path;
}

core::String GetFileName(const core::String& path)
{
  auto pos = path.find_last_of(Separator);

  if (pos == core::String::npos)
    return path;

  return path.substr(pos + 1);
}

core::String GetExtension(const core::String& path)
{
  // Note: could avoid creating temp string
  auto name = GetFileName(path);

  if (name.size() > 0) {
    auto pos = name.find_last_of(".");

    if (pos != core::String::npos && pos > 0) {
      return name.substr(pos);
    }
  }

  return "";
}

core::String GetParentDirectory(const core::String& path)
{
  auto pos = path.find_last_of(Separator);

  if (pos == core::String::npos)
    return path;

  return path.substr(0, pos);
}

bool HasFileName(const core::String& path)
{
  return GetFileName(path).size() > 0;
}

core::String ConvertToUnixPath(const core::String& path)
{
  return core::string::Replace(path, WindowsSeparatorStr, SeparatorStr);
}

core::String Normalize(const core::String& path)
{
  auto normalizedPath = ConvertToUnixPath(path);
  auto duplicatePred  = [](const char a, const char b) { return a == Separator && b == Separator; };

  normalizedPath.erase(std::unique(normalizedPath.begin(), normalizedPath.end(), duplicatePred),
                       normalizedPath.end());

  return normalizedPath;
}
} // namespace path
} // namespace io