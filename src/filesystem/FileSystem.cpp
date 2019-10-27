#include "FileSystem.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "filesystem/Path.h"
#include "physfs/src/physfs.h"

namespace io {
core::SharedPtr<IFileSystem> CreateFileSystem(const Path& argv0)
{
    auto filesystem = std::make_shared<FileSystem>();
    return filesystem->Init(argv0) ? filesystem : nullptr;
}

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
    if (PHYSFS_isInit())
        PHYSFS_deinit();
}

bool FileSystem::Init(const Path& argv0)
{
    if (PHYSFS_init(argv0.AsString().c_str())) {
        PHYSFS_permitSymbolicLinks(1);
        return true;
    }
    return false;
}

bool FileSystem::SetWriteDirectory(const Path& path)
{
    return PHYSFS_setWriteDir(path.AsString().c_str());
}

Path FileSystem::GetWriteDirectory()
{
    return Path(PHYSFS_getWriteDir());
}

Path FileSystem::GetWorkingDirectory()
{
    return Path(PHYSFS_getBaseDir());
}

bool FileSystem::AddSearchDirectory(const Path& path)
{
    return PHYSFS_mount(path.AsString().c_str(), NULL, 0);
}

bool FileSystem::DirectoryExists(const Path& path)
{
    PHYSFS_Stat stat;

    if (PHYSFS_stat(path.AsString().c_str(), &stat)) {
        return stat.filetype == PHYSFS_FILETYPE_DIRECTORY;
    }

    return false;
}

bool FileSystem::FileExists(const Path& path)
{
    PHYSFS_Stat stat;

    if (PHYSFS_stat(path.AsString().c_str(), &stat)) {
        return stat.filetype == PHYSFS_FILETYPE_REGULAR;
    }

    return false;
}

bool FileSystem::CreateDirectory(const Path& path)
{
    return PHYSFS_mkdir(path.AsString().c_str());
}

bool FileSystem::Delete(const Path& path)
{
    return PHYSFS_delete(path.AsString().c_str());
}

core::SharedPtr<IFileWriter> FileSystem::OpenWrite(const Path& path)
{
    auto fileWriter = std::make_shared<FileWriter>();
    return fileWriter->Open(path) ? fileWriter : nullptr;
}

core::SharedPtr<IFileReader> FileSystem::OpenRead(const Path& path)
{
    auto fileReader = std::make_shared<FileReader>();

    if(fileReader->Open(path)){
        return fileReader;
    }

    elog::LogWarning(core::string::format("File not found: '%s'", path.AsString().c_str()));
    return nullptr;
}

namespace {
void AppendFiles(void* data, const char* directory, const char* fileName)
{
    Path path(directory);
    path = path.Append(std::string(fileName));

    auto paths = static_cast<core::Vector<Path>*>(data);
    paths->push_back(path);
}
}

core::Vector<Path> FileSystem::GetFilesInDirectory(const Path& directory)
{
    core::Vector<Path> paths;
    PHYSFS_enumerateFilesCallback(directory.AsString().c_str(), AppendFiles, &paths);
    return paths;
}
}