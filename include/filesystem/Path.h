#ifndef PATH_H
#define PATH_H

// Warning: Meant for simple path handling.
// Some platform specific things might be left out since we're dealing overall
// with relative paths.
// All paths shall be considered as relative paths when doing operations on
// them.

#include "PathExt.h"

namespace io
{
class Path
{
public:
    Path();
    Path(const std::string& path);
    ~Path();

    Path Append(const Path& path);
    Path GetFileName() const;
    Path GetExtension() const;
    Path GetParentDirectory() const;
    bool HasFileName() const;
    const std::string& AsString() const;

    operator std::string() const;
    Path& operator=(const Path& other);

private:
    std::string m_path;
};

Path::Path(const std::string& path)
{
    m_path = io::PathExt::Normalize(path);
}

Path::Path() : m_path("")
{
}

Path::~Path()
{
}

Path Path::Append(const Path& path)
{
    return io::PathExt::Append(m_path, std::string(path));
}

Path Path::GetFileName() const
{
    return io::PathExt::GetFileName(m_path);
}

Path Path::GetExtension() const
{
    return io::PathExt::GetExtension(m_path);
}

Path Path::GetParentDirectory() const
{
    return io::PathExt::GetParentDirectory(m_path);
}

bool Path::HasFileName() const
{
    return io::PathExt::HasFileName(m_path);
}

const std::string& Path::AsString() const
{
    return m_path;
}

Path::operator std::string() const
{
    return m_path;
}

Path& Path::operator=(const Path& other)
{
    m_path = other.m_path;
    return *this;
}

inline bool operator==(const Path& lhs, const Path& rhs)
{
    return rhs.AsString() == lhs.AsString();
}
}

#endif