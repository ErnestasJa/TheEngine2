#include "FileWriter.h"
#include "filesystem/Path.h"
#include "physfs/src/physfs.h"

FileWriter::FileWriter(const Path& filePath)
{
    m_fileHandle = PHYSFS_openWrite(filePath.AsString().c_str());
}

FileWriter::~FileWriter()
{
    if (m_fileHandle != nullptr) PHYSFS_close(m_fileHandle);
}

std::intmax_t FileWriter::GetPosition() const
{
    return PHYSFS_tell(m_fileHandle);
}

template <class T>
std::intmax_t FileWriter::WriteFile(const T& dataBuffer, std::uintmax_t size)
{
    if (m_fileHandle) {
        std::uintmax_t writeSize =
            size > dataBuffer.size() ? dataBuffer.size() : size;

        auto bytesWrote =
            PHYSFS_writeBytes(m_fileHandle, dataBuffer.data(), writeSize);
        return bytesWrote;
    }

    return -1;
}

std::intmax_t FileWriter::Write(const TByteArray& array, std::intmax_t size)
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
