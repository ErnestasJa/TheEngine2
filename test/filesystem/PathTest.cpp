#include "gtest/gtest.h"
#include "filesystem/Path.h"
#include <iostream>

using namespace std::literals::string_literals;

::std::ostream &operator<<(::std::ostream &os, const Path &path) {
  return os << std::string(path).c_str();
}

class PathTest : public ::testing::Test {
protected:
  virtual void SetUp() 
  {
  	absolutePath = "/first/second/third/fourth"s;
  	relativePath = "fifth/sixth"s;
  	extension = ".ext"s;
  	fileNameWithoutExtension = "File"s;
  	fileNameWithExtension = "File.ext"s;
  	unixRootPath = "/"s;
  	winRootPath = "C:\\"s;
  	emptyPath = ""s;
  }

protected:
	std::string absolutePath,
		relativePath,
		emptyPath,

		unixRootPath,
		winRootPath,
		
		fileNameWithExtension,
		fileNameWithoutExtension,
		extension;
};

TEST_F(PathTest, ReturnsAppendedPathWithFileName) {
  Path correctPath(absolutePath+PathExt::Separator+fileNameWithExtension);

  ASSERT_EQ(correctPath, Path(absolutePath).Append(fileNameWithExtension));
}

TEST_F(PathTest, ReturnsAppendedPathWithoutAdditionalSeparators) {
  Path absPathWithAppendedSeparator(absolutePath + PathExt::Separator);
  Path relativePathWithPrependedSeparator(PathExt::Separator + relativePath);
  Path correctPath = absolutePath + PathExt::Separator + relativePath;

  ASSERT_EQ(correctPath, Path(absolutePath).Append(relativePath));
  ASSERT_EQ(correctPath, Path(absolutePath).Append(relativePathWithPrependedSeparator));
  ASSERT_EQ(correctPath, Path(absPathWithAppendedSeparator).Append(relativePath));
  ASSERT_EQ(correctPath, Path(absPathWithAppendedSeparator).Append(relativePathWithPrependedSeparator));
}

TEST_F(PathTest, ReturnsFileNameWithExtension) {
  ASSERT_EQ(fileNameWithExtension, Path(fileNameWithExtension).GetFileName());
  ASSERT_EQ(fileNameWithExtension, Path(PathExt::Separator + fileNameWithExtension).GetFileName());
  ASSERT_EQ(fileNameWithExtension, Path(absolutePath).Append(fileNameWithExtension).GetFileName());
}

TEST_F(PathTest, ReturnsFileNameWithoutExtension) {
  ASSERT_EQ(fileNameWithoutExtension, Path(fileNameWithoutExtension).GetFileName());
  ASSERT_EQ(fileNameWithoutExtension, Path(PathExt::Separator + fileNameWithoutExtension).GetFileName());
  ASSERT_EQ(fileNameWithoutExtension, Path(absolutePath).Append(fileNameWithoutExtension).GetFileName());
}

TEST_F(PathTest, ReturnsEmptyForEmptyPath) {
  Path path("");

  ASSERT_EQ(emptyPath, path.GetFileName());
  ASSERT_EQ(emptyPath, path.GetExtension());
  ASSERT_EQ(emptyPath, path.GetParentDirectory());
}

TEST_F(PathTest, ReturnsParentDirWhenPathWithFile) {
  Path path = Path(absolutePath).Append(fileNameWithoutExtension);

  ASSERT_EQ(absolutePath, path.GetParentDirectory());
}

TEST_F(PathTest, ReturnsParentDirWhenPathWithFileAndExtension) {
  Path path = Path(absolutePath).Append(fileNameWithExtension);
  ASSERT_EQ(absolutePath, path.GetParentDirectory());
}

TEST_F(PathTest, ReturnsExtension) {
  ASSERT_EQ(extension, Path(fileNameWithExtension).GetExtension());
  ASSERT_EQ(extension, Path(absolutePath).Append(fileNameWithExtension).GetExtension());
  ASSERT_EQ(extension, Path(relativePath).Append(fileNameWithExtension).GetExtension());
}

TEST_F(PathTest, ReturnsEmptyForExtension) {
  ASSERT_EQ(emptyPath, Path().GetExtension());
  ASSERT_EQ(emptyPath, Path(fileNameWithoutExtension).GetExtension());
  ASSERT_EQ(emptyPath, Path(absolutePath).GetExtension());
  ASSERT_EQ(emptyPath, Path(relativePath).GetExtension());
  //Note: Do we need to handle unix hidden files?
}

TEST_F(PathTest, DirSeparatorsAreNormalizedOnConstruction) {
  std::string goodPath = "C:/SomeRelativePath/BlaBla/SomeFile.exec";
  std::string badUnixSeparatorPath =
      "C://SomeRelativePath///////BlaBla//SomeFile.exec";
  std::string badWinSeparatorPath =
      "C:\\\\SomeRelativePath\\\\\\\\BlaBla\\\\SomeFile.exec";

  ASSERT_EQ(goodPath, Path(badUnixSeparatorPath));
  ASSERT_EQ(goodPath, Path(badWinSeparatorPath));
}