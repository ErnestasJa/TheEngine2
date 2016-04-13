#include "filesystem/Path.h"
#include <algorithm>

namespace io
{
namespace path
{
    std::string Append(const std::string& path, const std::string& pathToAppend)
    {
        auto size = path.size();
        auto appendSize = pathToAppend.size();

        if (size > 0 && appendSize > 0) {
            if (path[size - 1] == Separator && pathToAppend[0] == Separator)
                return std::string(path)
                    .append(pathToAppend, 1, std::string::npos);
            if (path[size - 1] == Separator || pathToAppend[0] == Separator)
                return path + pathToAppend;
            else
                return path + Separator + pathToAppend;
        }

        return appendSize > 0 ? pathToAppend : path;
    }

    std::string GetFileName(const std::string& path)
    {
        auto pos = path.find_last_of(Separator);

        if (pos == std::string::npos) return path;

        return path.substr(pos + 1);
    }

    std::string GetExtension(const std::string& path)
    {
        // Note: could avoid creating temp string
        auto name = GetFileName(path);

        if (name.size() > 0) {
            auto pos = name.find_last_of(".");

            if (pos != std::string::npos && pos > 0) {
                return name.substr(pos);
            }
        }

        return "";
    }

    std::string GetParentDirectory(const std::string& path)
    {
        auto pos = path.find_last_of(Separator);

        if (pos == std::string::npos) return path;

        return path.substr(0, pos);
    }

    bool HasFileName(const std::string& path)
    {
        return GetFileName(path).size() > 0;
    }

    std::string ConvertToUnixPath(const std::string& path)
    {
        return core::string::Replace(path, WindowsSeparatorStr, SeparatorStr);
    }

    std::string Normalize(const std::string& path)
    {
        auto normalizedPath = ConvertToUnixPath(path);
        auto duplicatePred = [](const char a, const char b) {
            return a == Separator && b == Separator;
        };

        normalizedPath.erase(std::unique(normalizedPath.begin(),
                                         normalizedPath.end(), duplicatePred),
                             normalizedPath.end());

        return normalizedPath;
    }
}
}