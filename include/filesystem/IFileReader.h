#ifndef IFILE_READER_H
#define IFILE_READER_H

namespace io
{
class IFileReader
{
public:
    virtual ~IFileReader()
    {
    }
    virtual std::intmax_t GetLength() const = 0;
    virtual std::intmax_t GetPosition() const = 0;
    virtual std::intmax_t Read(
        core::TByteArray& array,
        std::size_t size = std::numeric_limits<std::size_t>::max()) = 0;
    virtual std::intmax_t Read(
        std::string& string,
        std::size_t size = std::numeric_limits<std::size_t>::max()) = 0;
    virtual bool Seek(std::size_t position) = 0;
};
}

#endif