#ifndef IFILE_WRITER_H
#define IFILE_WRITER_H

namespace io
{
class IFileWriter
{
public:
    virtual ~IFileWriter();
    virtual std::intmax_t GetPosition() const = 0;
    virtual std::intmax_t Write(
        const core::TByteArray& array,
        std::intmax_t size = std::numeric_limits<std::uintmax_t>::max()) = 0;
    virtual std::intmax_t Write(
        const std::string& string,
        std::uintmax_t size = std::numeric_limits<std::uintmax_t>::max()) = 0;
    virtual bool Seek(std::uintmax_t position) = 0;
};
}

#endif