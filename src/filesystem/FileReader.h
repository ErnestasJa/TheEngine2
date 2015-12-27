#ifndef FILE_READER_H
#define FILE_READER_H

#include "filesystem/IFileReader.h"

struct PHYSFS_File;
namespace io
{
class FileReader : public IFileReader
{
public:
    FileReader(const Path& filePath);
    virtual ~FileReader();
    virtual std::intmax_t GetLength() const;
    virtual std::intmax_t GetPosition() const;
    virtual std::intmax_t Read(
        core::TByteArray& array,
        std::uintmax_t size = std::numeric_limits<std::uintmax_t>::max());
    virtual std::intmax_t Read(
        std::string& string,
        std::uintmax_t size = std::numeric_limits<std::uintmax_t>::max());
    virtual bool Seek(std::uintmax_t position);

private:
    template <class T>
    std::intmax_t ReadFile(
        T& buffer,
        std::uintmax_t size = std::numeric_limits<std::uintmax_t>::max());
    PHYSFS_File* m_fileHandle;
};
}

#endif