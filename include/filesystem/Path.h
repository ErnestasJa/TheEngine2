#ifndef PATH_H
#define PATH_H

// Warning: Meant for simple path handling.
// Some platform specific things might be left out since we're dealing overall
// with relative paths.
// All paths shall be considered as relative paths when doing operations on
// them.

#include "PathUtil.h"

namespace io
{
class Path
{
public:
    Path(const std::string& path)
    {
        m_path = io::path::Normalize(path);
    }

    Path() : m_path("")
    {
    }

    ~Path()
    {
    }

    Path Append(const Path& path)
    {
        return io::path::Append(m_path, std::string(path));
    }

    Path GetFileName() const
    {
        return io::path::GetFileName(m_path);
    }
    Path GetExtension() const
    {
        return io::path::GetExtension(m_path);
    }
    Path GetParentDirectory() const
    {
        return io::path::GetParentDirectory(m_path);
    }

    bool HasFileName() const
    {
        return io::path::HasFileName(m_path);
    }
    const std::string& AsString() const
    {
        return m_path;
    }
    operator std::string() const
    {
        return m_path;
    }
    Path& operator=(const Path& other)
    {
        m_path = other.m_path;
        return *this;
    }

private:
    std::string m_path;
};

inline bool operator==(const Path& lhs, const Path& rhs)
{
    return rhs.AsString() == lhs.AsString();
}
}

#endif