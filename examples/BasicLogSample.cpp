#include "log/LogInc.h"
#include "core/StringExt.h"
#include <iostream>

namespace String = core::string;

class CoutPipe : public log::ILogStream
{
public:
    void Log(const core::String &logStr)
    {
        std::cout << logStr << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    using logger = log::Logger;

    for (int32_t i = 0; i < 10; i++) {
        auto coutPipe = core::MakeShared<CoutPipe>();
        logger::Get().AttachStream(coutPipe);
        log::Log(String::CFormat("Test %d", i));
    }

    log::Log("Should not be logged anywhere.");
    logger::Get().CleanDeadStreams();
    log::Log("Also should not be logged anywhere.");

    auto coutPipe = core::MakeShared<CoutPipe>();
    logger::Get().AttachStream(coutPipe);
    log::Log("Last log to cout.");

    return 0;
}