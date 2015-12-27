#ifndef IFILE_SYSTEM_H
#define IFILE_SYSTEM_H

#include "IFileWriter.h"
#include "IFileReader.h"

namespace io
{
class IFileSystem
{
public:
    virtual ~IFileSystem();
    virtual bool SetWriteDirectory(const Path& path) = 0;
    virtual Path GetWriteDirectory() = 0;
    virtual Path GetWorkingDirectory() = 0;
    virtual bool AddSearchDirectory(const Path& path) = 0;
    virtual bool DirectoryExists(const Path& path) = 0;
    virtual bool FileExists(const Path& path) = 0;
    virtual bool CreateDirectory(const Path& path) = 0;
    virtual core::SharedPtr<IFileWriter> OpenWrite(const Path& path) = 0;
    virtual core::SharedPtr<IFileReader> OpenRead(const Path& path) = 0;
    virtual core::Vector<Path> GetFilesInDirectory(const Path& directory) = 0;
};
}

#endif