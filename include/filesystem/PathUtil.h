#ifndef PATH_EXT_H
#define PATH_EXT_H

namespace io
{
namespace path
{
    const auto Separator = '/';
    const std::string SeparatorStr{Separator};
    const auto WindowsSeparator = '\\';
    const std::string WindowsSeparatorStr{WindowsSeparator};

    std::string Append(const std::string& path,
                       const std::string& pathToAppend);
    std::string GetFileName(const std::string& path);
    std::string GetExtension(const std::string& path);
    std::string GetParentDirectory(const std::string& path);
    bool HasFileName(const std::string& path);
    std::string ConvertToUnixPath(const std::string& path);
    std::string Normalize(const std::string& path);
}
}

#endif