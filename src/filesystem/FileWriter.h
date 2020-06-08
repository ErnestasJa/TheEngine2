#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "filesystem/IFileWriter.h"

struct PHYSFS_File;
namespace io {
class FileWriter : public IFileWriter
{
  public:
  FileWriter();
  virtual ~FileWriter();
  bool Open(const Path& filePath);
  virtual std::intmax_t GetPosition() const;
  virtual std::intmax_t Write(const core::TByteArray& array,
                              std::intmax_t size = std::numeric_limits<std::uintmax_t>::max());
  virtual std::intmax_t Write(const std::string& string,
                              std::uintmax_t size = std::numeric_limits<std::uintmax_t>::max());
  virtual bool Seek(std::uintmax_t position);

  private:
  template <class T>
  std::intmax_t WriteFile(const T& buffer,
                          std::uintmax_t size = std::numeric_limits<std::uintmax_t>::max());

  PHYSFS_File* m_fileHandle;
};
} // namespace io

#endif