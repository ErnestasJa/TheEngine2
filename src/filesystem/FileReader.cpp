#include "FileReader.h"
#include "filesystem/Path.h"
#include "physfs/src/physfs.h"

namespace io {
FileReader::FileReader()
{
}

FileReader::~FileReader()
{
  if (m_fileHandle != nullptr)
    PHYSFS_close(m_fileHandle);
}

bool FileReader::Open(const Path& filePath)
{
  m_fileHandle = PHYSFS_openRead(filePath.AsString().c_str());
  return m_fileHandle != nullptr;
}

std::intmax_t FileReader::GetLength() const
{
  return PHYSFS_fileLength(m_fileHandle);
}

std::intmax_t FileReader::GetPosition() const
{
  return PHYSFS_tell(m_fileHandle);
}

template <class T> std::intmax_t FileReader::ReadFile(T& dataBuffer, std::uintmax_t size)
{
  if (m_fileHandle) {
    std::intmax_t fileLength   = GetLength();
    std::intmax_t filePosition = GetPosition();

    if (filePosition > -1 && fileLength > -1) {
      std::uintmax_t readSize = fileLength - filePosition;
      if (size < readSize)
        readSize = size;

      dataBuffer.resize(readSize);

      std::intmax_t bytesRead = PHYSFS_readBytes(m_fileHandle, (void*)dataBuffer.data(), readSize);

      if (bytesRead > -1)
        return bytesRead;
    }
  }

  // Failed to read, buffer should be empty.
  T().swap(dataBuffer);
  return -1;
}

std::intmax_t FileReader::Read(core::TByteArray& array, std::uintmax_t size)
{
  return ReadFile(array, size);
}

std::intmax_t FileReader::Read(std::string& string, std::uintmax_t size)
{
  return ReadFile(string, size);
}

std::intmax_t FileReader::Read(void* buffer,
                           std::uintmax_t size){
    if (m_fileHandle) {
        std::intmax_t fileLength   = GetLength();
        std::intmax_t filePosition = GetPosition();

        if (filePosition > -1 && fileLength > -1) {
            std::uintmax_t readSize = fileLength - filePosition;
            if (size < readSize)
                readSize = size;

            std::intmax_t bytesRead = PHYSFS_readBytes(m_fileHandle, buffer, readSize);

            if (bytesRead > -1)
                return bytesRead;
        }
    }

    return -1;
}

bool FileReader::Seek(std::uintmax_t position)
{
  return PHYSFS_seek(m_fileHandle, position) != 0;
}
} // namespace io