#include "FileWriter.h"
#include "filesystem/Path.h"
#include "physfs/src/physfs.h"

namespace io {
FileWriter::FileWriter()
{
}

FileWriter::~FileWriter()
{
  if (m_fileHandle != nullptr)
    PHYSFS_close(m_fileHandle);
}

bool FileWriter::Open(const Path& filePath)
{
  m_fileHandle = PHYSFS_openWrite(filePath.AsString().c_str());
  return m_fileHandle != nullptr;
}

bool FileWriter::OpenAppend(const Path& filePath)
{
  m_fileHandle = PHYSFS_openAppend(filePath.AsString().c_str());
  return m_fileHandle != nullptr;
}

std::intmax_t FileWriter::GetPosition() const
{
  return PHYSFS_tell(m_fileHandle);
}

template <class T> std::intmax_t FileWriter::WriteFile(const T& dataBuffer, std::uintmax_t size)
{
  if (m_fileHandle) {
    std::uintmax_t writeSize = size > dataBuffer.size() ? dataBuffer.size() : size;

    auto bytesWrote = PHYSFS_writeBytes(m_fileHandle, dataBuffer.data(), writeSize);
    return bytesWrote;
  }

  return -1;
}

std::intmax_t FileWriter::Write(const core::TByteArray& array, std::intmax_t size)
{
  return WriteFile(array, size);
}

std::intmax_t FileWriter::Write(const std::string& string, std::uintmax_t size)
{
  return WriteFile(string, size);
}

bool FileWriter::Seek(std::uintmax_t position)
{
  return PHYSFS_seek(m_fileHandle, position) != 0;
}
} // namespace io
