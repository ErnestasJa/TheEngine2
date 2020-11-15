#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "filesystem/IFileSystem.h"

namespace io {
class FileSystem : public IFileSystem
{
  public:
  FileSystem();
  virtual ~FileSystem();
  virtual bool Init(const Path& argv0);
  virtual bool SetWriteDirectory(const Path& path);
  virtual Path GetWriteDirectory();
  virtual Path GetWorkingDirectory();
  virtual bool AddSearchDirectory(const Path& path);
  virtual bool DirectoryExists(const Path& path);
  virtual bool FileExists(const Path& path);
  virtual bool CreateDirectory(const Path& path);
  virtual bool Delete(const Path& path);
  virtual core::UniquePtr<IFileWriter> OpenWrite(const Path& path, bool append = false);
  virtual core::UniquePtr<IFileReader> OpenRead(const Path& path);
  virtual core::Vector<Path> GetFilesInDirectory(const Path& directory);
};
} // namespace io

#endif