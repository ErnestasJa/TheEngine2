#include "gtest/gtest.h"
#include "Common.h"
#include "filesystem/IFileSystem.h"

namespace
{
const char *argv0;
}

int main(int argc, char **argv)
{
    argv0 = argv[0];
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class FileSystemTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        ASSERT_NE(argv0, nullptr);
        testDirectoryPath = io::Path(std::string(argv0));
        testExecutableName = "FileSystemTest"s;

        if (testDirectoryPath.HasFileName() &&
            testDirectoryPath.GetFileName() == testExecutableName)
            testDirectoryPath = testDirectoryPath.GetParentDirectory();

        testPath = testDirectoryPath.Append(testExecutableName);
        ASSERT_EQ(testExecutableName, testPath.GetFileName());

        fileSystem = io::CreateFileSystem(testDirectoryPath);
        ASSERT_TRUE(fileSystem->AddSearchDirectory(testDirectoryPath));
        ASSERT_TRUE(fileSystem->SetWriteDirectory(testDirectoryPath));
    }

protected:
    io::Path testDirectoryPath, testPath, testExecutableName;
    core::SharedPtr<io::IFileSystem> fileSystem;
};

TEST_F(FileSystemTest, CanFindSelf)
{
    ASSERT_TRUE(fileSystem->FileExists(testPath.GetFileName()));
}

TEST_F(FileSystemTest, FailsWhenCheckingDirectoryThatIsFile)
{
    ASSERT_FALSE(fileSystem->DirectoryExists(testPath.GetFileName()));
}

TEST_F(FileSystemTest, CanCreateDirectory)
{
    io::Path dirName = "TestDirectory"s;

    ASSERT_FALSE(fileSystem->DirectoryExists(dirName));
    ASSERT_TRUE(fileSystem->CreateDirectory(dirName));
    ASSERT_TRUE(fileSystem->DirectoryExists(dirName));
}

TEST_F(FileSystemTest, CanCreateWriteFile)
{
    io::Path testFile = "TestFile"s;

    {
        auto file = fileSystem->OpenWrite(testFile);
        ASSERT_NE(nullptr, file.get());
    }

    ASSERT_TRUE(fileSystem->FileExists(testFile));
}

TEST_F(FileSystemTest, CanWriteStringToFile)
{
    io::Path testFile = "TestFile"s;
    auto testString = "Hello world!"s;

    {
        auto file = fileSystem->OpenWrite(testFile);
        ASSERT_NE(nullptr, file.get());

        auto bytesWrote = file->Write(testString);
        ASSERT_EQ(testString.length(), bytesWrote);
    }

    ASSERT_TRUE(fileSystem->FileExists(testFile));
}