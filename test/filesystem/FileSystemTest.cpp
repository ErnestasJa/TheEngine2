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
    virtual void SetUp() override
    {
        ASSERT_NE(argv0, nullptr);

        writeFilePath = "TestWriteFile"s + Common::GetTimestampString();
        readFilePath = "testdata/TestReadFile.txt"s;
        directoryPath = "TestDirectory"s + Common::GetTimestampString();
        testExecutableName = "FileSystemTest"s;
        testDirectoryPath = io::Path(std::string(argv0));
        readFileContents =
            "Hello?!\n"
            "World!";

        if (testDirectoryPath.HasFileName() &&
            testDirectoryPath.GetFileName() == testExecutableName)
            testDirectoryPath = testDirectoryPath.GetParentDirectory();

        testPath = testDirectoryPath.Append(testExecutableName);
        ASSERT_EQ(testExecutableName, testPath.GetFileName());

        fileSystem = io::CreateFileSystem(testDirectoryPath);
        ASSERT_TRUE(fileSystem->AddSearchDirectory(testDirectoryPath));
        ASSERT_TRUE(fileSystem->SetWriteDirectory(testDirectoryPath));
    }

    virtual void TearDown() override
    {
        if (fileSystem->FileExists(writeFilePath))
            ASSERT_TRUE(fileSystem->Delete(writeFilePath));
        if (fileSystem->FileExists(directoryPath))
            ASSERT_TRUE(fileSystem->Delete(directoryPath));
    }

protected:
    io::Path testDirectoryPath, testPath, testExecutableName;
    io::Path writeFilePath, readFilePath, directoryPath;
    std::string readFileContents;
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
    ASSERT_FALSE(fileSystem->DirectoryExists(directoryPath));
    ASSERT_TRUE(fileSystem->CreateDirectory(directoryPath));
    ASSERT_TRUE(fileSystem->DirectoryExists(directoryPath));
}

TEST_F(FileSystemTest, CanCreateWriteFile)
{
    {
        auto file = fileSystem->OpenWrite(writeFilePath);
        ASSERT_NE(nullptr, file.get());
    }

    ASSERT_TRUE(fileSystem->FileExists(writeFilePath));
}

TEST_F(FileSystemTest, CanWriteStringToFile)
{
    auto testString = "Hello world!"s;

    {
        auto file = fileSystem->OpenWrite(writeFilePath);
        ASSERT_NE(nullptr, file.get());

        auto bytesWrote = file->Write(testString);
        ASSERT_EQ(testString.length(), bytesWrote);
    }

    ASSERT_TRUE(fileSystem->FileExists(writeFilePath));
}

TEST_F(FileSystemTest, CanReadStringFromFile)
{
    auto file = fileSystem->OpenRead(readFilePath);
    ASSERT_NE(nullptr, file.get());

    std::string testString;
    auto bytesRead = file->Read(testString);
    ASSERT_TRUE(bytesRead > 0);
}

TEST_F(FileSystemTest, StringContentsReadFromFileAreCorrect)
{
    auto file = fileSystem->OpenRead(readFilePath);
    ASSERT_NE(nullptr, file.get());

    std::string testString;
    auto bytesRead = file->Read(testString);

    ASSERT_TRUE(bytesRead > -1);
    ASSERT_TRUE(readFileContents.size() == (std::size_t)bytesRead);
    ASSERT_EQ(readFileContents, testString);
    ASSERT_EQ(testString, readFileContents);
}

TEST_F(FileSystemTest, ByteBufferContentsReadFromFileAreCorrect)
{
    core::TByteArray correctArray(readFileContents.begin(),
                                  readFileContents.end());

    auto file = fileSystem->OpenRead(readFilePath);
    ASSERT_NE(nullptr, file.get());

    core::TByteArray testByteArray;
    auto bytesRead = file->Read(testByteArray);

    ASSERT_TRUE(bytesRead > -1);
    ASSERT_TRUE(correctArray.size() == (std::size_t)bytesRead);
    ASSERT_EQ(correctArray, testByteArray);
}