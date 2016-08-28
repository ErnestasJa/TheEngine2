#include "filesystem/Path.h"
#include "gtest/gtest.h"
#include <iostream>

using namespace std::literals::string_literals;
using namespace io;

::std::ostream& operator<<(::std::ostream& os, const io::Path& path)
{
    return os << std::string(path).c_str();
}

class PathTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        // Note: we can do something like in boost documentation.
        // Make table of inputs and expected outputs for each function.
        // Do tests based on table, then it will be easier to add more cases.
        // http://www.boost.org/doc/libs/1_59_0/libs/filesystem/doc/reference.html#Path-decomposition-table
        absolutePath             = "/first/second/third/fourth"s;
        relativePath             = "fifth/sixth"s;
        extension                = ".ext"s;
        fileNameWithoutExtension = "File"s;
        fileNameWithExtension    = "File.ext"s;
        unixRootPath             = "/"s;
        winRootPath              = "C:\\"s;
        emptyPath                = ""s;
    }

protected:
    std::string absolutePath, relativePath, emptyPath, unixRootPath, winRootPath,
        fileNameWithExtension, fileNameWithoutExtension, extension;
};

TEST_F(PathTest, ReturnsAppendedPathWithFileName)
{
    Path correctPath(absolutePath + path::Separator + fileNameWithExtension);

    ASSERT_EQ(correctPath, Path(absolutePath).Append(fileNameWithExtension));
}

TEST_F(PathTest, ReturnsAppendedPathWithoutAdditionalSeparators)
{
    Path absPathWithAppendedSeparator(absolutePath + path::Separator);
    Path relativePathWithPrependedSeparator(path::Separator + relativePath);
    Path correctPath = absolutePath + path::Separator + relativePath;

    ASSERT_EQ(correctPath, Path(absolutePath).Append(relativePath));
    ASSERT_EQ(correctPath, Path(absolutePath).Append(relativePathWithPrependedSeparator));
    ASSERT_EQ(correctPath, Path(absPathWithAppendedSeparator).Append(relativePath));
    ASSERT_EQ(correctPath,
              Path(absPathWithAppendedSeparator).Append(relativePathWithPrependedSeparator));
}

TEST_F(PathTest, ReturnsFileNameWithExtension)
{
    ASSERT_EQ(fileNameWithExtension, Path(fileNameWithExtension).GetFileName());
    ASSERT_EQ(fileNameWithExtension, Path(path::Separator + fileNameWithExtension).GetFileName());
    ASSERT_EQ(fileNameWithExtension,
              Path(absolutePath).Append(fileNameWithExtension).GetFileName());
}

TEST_F(PathTest, ReturnsFileNameWithoutExtension)
{
    ASSERT_EQ(fileNameWithoutExtension, Path(fileNameWithoutExtension).GetFileName());
    ASSERT_EQ(fileNameWithoutExtension,
              Path(path::Separator + fileNameWithoutExtension).GetFileName());
    ASSERT_EQ(fileNameWithoutExtension,
              Path(absolutePath).Append(fileNameWithoutExtension).GetFileName());
}

TEST_F(PathTest, ReturnsEmptyForEmptyPath)
{
    Path path("");

    ASSERT_EQ(emptyPath, path.GetFileName());
    ASSERT_EQ(emptyPath, path.GetExtension());
    ASSERT_EQ(emptyPath, path.GetParentDirectory());
}

TEST_F(PathTest, ReturnsParentDirWhenPathWithFile)
{
    Path path = Path(absolutePath).Append(fileNameWithoutExtension);

    ASSERT_EQ(absolutePath, path.GetParentDirectory());
}

TEST_F(PathTest, ReturnsParentDirWhenPathWithFileAndExtension)
{
    Path path = Path(absolutePath).Append(fileNameWithExtension);
    ASSERT_EQ(absolutePath, path.GetParentDirectory());
}

TEST_F(PathTest, ReturnsExtension)
{
    ASSERT_EQ(extension, Path(fileNameWithExtension).GetExtension());
    ASSERT_EQ(extension, Path(absolutePath).Append(fileNameWithExtension).GetExtension());
    ASSERT_EQ(extension, Path(relativePath).Append(fileNameWithExtension).GetExtension());
}

TEST_F(PathTest, ReturnsEmptyForExtension)
{
    ASSERT_EQ(emptyPath, Path().GetExtension());
    ASSERT_EQ(emptyPath, Path(fileNameWithoutExtension).GetExtension());
    ASSERT_EQ(emptyPath, Path(absolutePath).GetExtension());
    ASSERT_EQ(emptyPath, Path(relativePath).GetExtension());
    // Note: Do we need to handle unix hidden files?
}

TEST_F(PathTest, DetectsFilename)
{
    ASSERT_TRUE(Path(fileNameWithExtension).HasFileName());
    ASSERT_TRUE(Path(relativePath).Append(fileNameWithExtension).HasFileName());
    ASSERT_TRUE(Path(absolutePath).Append(fileNameWithExtension).HasFileName());
    ASSERT_FALSE(Path("").HasFileName());
    ASSERT_FALSE(Path(unixRootPath).HasFileName());
    ASSERT_FALSE(Path(winRootPath).HasFileName());
}

TEST_F(PathTest, CanAppendSingleSeparatorToPath)
{
    ASSERT_EQ(relativePath + path::Separator, Path(relativePath).Append(path::SeparatorStr));

    ASSERT_EQ(relativePath + path::Separator,
              Path(relativePath).Append(path::SeparatorStr).Append(path::SeparatorStr));

    ASSERT_EQ(Path(unixRootPath), Path(unixRootPath).Append(path::SeparatorStr));

    ASSERT_EQ(Path(winRootPath), Path(winRootPath).Append(path::SeparatorStr));
}

TEST_F(PathTest, DirSeparatorsAreNormalizedOnConstruction)
{
    std::string goodPath             = "C:/SomeRelativePath/BlaBla/SomeFile.exec";
    std::string badUnixSeparatorPath = "C://SomeRelativePath///////BlaBla//SomeFile.exec";
    std::string badWinSeparatorPath  = "C:\\\\SomeRelativePath\\\\\\\\BlaBla\\\\SomeFile.exec";

    ASSERT_EQ(goodPath, Path(badUnixSeparatorPath));
    ASSERT_EQ(goodPath, Path(badWinSeparatorPath));
}