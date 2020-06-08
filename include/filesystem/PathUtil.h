#ifndef PATH_EXT_H
#define PATH_EXT_H

namespace io {
namespace path {
const auto Separator = '/';
const core::String SeparatorStr{ Separator };
const auto WindowsSeparator = '\\';
const core::String WindowsSeparatorStr{ WindowsSeparator };

core::String Append(const core::String& path, const core::String& pathToAppend);
core::String GetFileName(const core::String& path);
core::String GetExtension(const core::String& path);
core::String GetParentDirectory(const core::String& path);
bool HasFileName(const core::String& path);
core::String ConvertToUnixPath(const core::String& path);
core::String Normalize(const core::String& path);
} // namespace path
} // namespace io

#endif