#include "gtest/gtest.h"
#include "filesystem/Path.h"
#include <iostream>

using namespace std::literals::string_literals;

::std::ostream& operator<<(::std::ostream& os, const Path& path) {
  return os << std::string(path).c_str();
}

TEST(PathAppendTests, ReturnsAppendedPathWithFileNAme)
{
	Path path("/home/serengeor/Coding");
	Path path2("TheEngine2");
	Path correctPath("/home/serengeor/Coding/TheEngine2");

	Path appendPathResult = path.Append(path2);

	ASSERT_EQ(correctPath, appendPathResult);
}

TEST(PathAppendTests, ReturnsAppendedPathWithoutAdditionalSeparators)
{
	//REFACTOR: Clarify this test
	Path path("/home/serengeor/Coding");
	Path pathS("/home/serengeor/Coding/");
	Path path2("TheEngine2");
	Path path2S("/TheEngine2");
	Path correctPath("/home/serengeor/Coding/TheEngine2");

	Path appendPathResult1 = path.Append(path2);
	Path appendPathResult2 = pathS.Append(path2);
	Path appendPathResult3 = path.Append(path2S);
	Path appendPathResult4 = pathS.Append(path2S);

	ASSERT_EQ(correctPath, appendPathResult1);
	ASSERT_EQ(correctPath, appendPathResult2);
	ASSERT_EQ(correctPath, appendPathResult3);
	ASSERT_EQ(correctPath, appendPathResult4);
}

TEST(PathFileNameTests, ReturnsFileNameWithoutPath)
{
	Path path("/home/serengeor/Coding/TheEngine2");
	Path path2("/TheEngine2");

	ASSERT_EQ("TheEngine2"s, path.GetFileName());
	ASSERT_EQ("TheEngine2"s, path2.GetFileName());
}

TEST(PathFileNameTests, ReturnsFileNameIfNotInDirectory)
{
	Path path("TheEngine2");

	ASSERT_EQ("TheEngine2"s, path.GetFileName());
}

TEST(PathFileNameTests, ReturnsFileNameWithExtension)
{
	Path path("/home/serengeor/Coding/TheEngine2.exe");

	ASSERT_EQ("TheEngine2.exe"s, path.GetFileName());
}

TEST(PathParentDirectoryTests, ReturnsParentDirWhenPathWithFile)
{
	std::string pathString = "/home/serengeor/Coding";
	std::string fileString = "/TheEngine2";
	Path path(pathString+fileString);

	ASSERT_EQ(pathString, path.GetParentDirectory());
}

TEST(PathParentDirectoryTests, ReturnsParentDirWhenPathWithFileAndExtension)
{
	std::string pathString = "/home/serengeor/Coding";
	std::string fileString = "/TheEngine2.exe";
	Path path(pathString+fileString);

	ASSERT_EQ(pathString, path.GetParentDirectory());
}

TEST(PathExtensionTests, ReturnsExtensionWithDotWhenFileWithPath)
{
	std::string pathString = "/home/serengeor/Coding";
	std::string fileString = "/TheEngine2.exe";
	Path path(pathString+fileString);

	ASSERT_EQ(".exe"s, path.GetExtension());
}

TEST(PathExtensionTests, ReturnsExtensionWithDotWhenFileWithout)
{
	std::string fileString = "TheEngine2.exe";
	Path path(fileString);

	ASSERT_EQ(".exe"s, path.GetExtension());
}

TEST(PathExtensionTests, ReturnsEmptyStringForExtensionWhenNoExtension)
{
	std::string fileString = "TheEngine2";
	Path path(fileString);

	ASSERT_EQ(""s, path.GetExtension());
}