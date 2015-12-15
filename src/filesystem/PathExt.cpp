#include "filesystem/PathExt.h"
#include <algorithm>

namespace PathExt
{
	//REFACTOR: Move to StringExt
	std::string Replace(const std::string & source, const std::string & toReplace, const std::string & replacement)
	{
		std::size_t index = 0;
		std::string src = source;

		while (true)
		{
			index = src.find(toReplace, index);
		    if (index == std::string::npos) break;

		    src.replace(index, replacement.size(), replacement);
			index += replacement.size();
		}

		return src;
	}

	std::string Append(const std::string & path, const std::string & pathToAppend)
	{
		auto size = path.size();
		auto appendSize = pathToAppend.size();

		if(size > 0 && appendSize > 0)
		{
			if(path[size-1] == Separator[0] || pathToAppend[0] == Separator[0])
				return path + pathToAppend;
			else
				return path + Separator + pathToAppend; 
		}

		return appendSize > 0 ? pathToAppend : path;
	}

	std::string GetFileName(const std::string & path)
	{
		auto pos = path.find_last_of(Separator);

		if(pos == std::string::npos)
			return path;

		return path.substr(pos+1);
	}

	std::string GetExtension(const std::string & path)
	{
		//Note: could avoid creating temp string
		auto name = GetFileName(path);

		if(name.size() > 0)
		{
			auto pos = name.find_last_of(".");

			if(pos != std::string::npos && pos > 0)
			{
				return name.substr(pos);
			}
		}

		return "";
	}

	std::string GetParentDirectory(const std::string & path)
	{
		auto pos = path.find_last_of(Separator);

		if(pos == std::string::npos)
			return path;

		return path.substr(0, pos);
	}

	bool HasFileName(const std::string & path)
	{
		return GetFileName(path).size() > 0;
	}

	std::string ConvertToUnixPath(const std::string & path)
	{
		return Replace(path, WindowsSeparator, Separator);
	}
}