#ifndef PATH_H
#define PATH_H

#include <string>

//Warning: Meant for simple path handling.
//Warning: Some platform specific things might be left out since we're dealing overall with relative paths.

class Path
{
public:
	Path();
	Path(const std::string & path);
	~Path();

	Path Append(const Path & path);
	Path GetFileName() const;
	Path GetExtension() const;
	Path GetParentDirectory() const;
	bool HasFileName() const;
	const std::string & AsString() const;

	operator std::string() const;
	Path & operator=(const Path & other);
private:
	std::string m_path;
};

#include "PathExt.h"

Path::Path(const std::string & path)
{
	m_path = PathExt::Normalize(path);
}

Path::Path()
: m_path("")
{
	
}

Path::~Path()
{

}

Path Path::Append(const Path & path)
{
  return PathExt::Append(m_path, std::string(path));
}

Path Path::GetFileName() const
{
	return PathExt::GetFileName(m_path);
}

Path Path::GetExtension() const
{
	return PathExt::GetExtension(m_path);
}

Path Path::GetParentDirectory() const
{
	return PathExt::GetParentDirectory(m_path);
}

bool Path::HasFileName() const
{
	return PathExt::HasFileName(m_path);
}

const std::string & Path::AsString() const
{
	return m_path;
}

Path::operator std::string() const
{
	return m_path;
}

Path & Path::operator=(const Path & other)
{
	m_path = other.m_path;
	return *this;
}

inline bool operator==(const Path & lhs, const Path & rhs)
{
	return rhs.AsString() == lhs.AsString();
}

#endif