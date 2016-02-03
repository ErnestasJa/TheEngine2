#include "log/LogInc.h"
#include "core/StringExt.h"
#include <iostream>

namespace String = core::string;

class EngineCoutPipe : public log::ILogStream
{
public:
    void Log(const log::LogSource source, const core::String &logStr)
    {
        if (source == log::LogSource::Engine)
            std::cout << "Engine log: " << logStr << std::endl;
    }
};

class OtherPipe : public log::ILogStream
{
public:
    void Log(const log::LogSource source, const core::String &logStr)
    {
        if (source == log::LogSource::Other)
            printf("Other: %s\n", logStr.c_str());
    }
};

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<EngineCoutPipe>();
    auto otherLogStream = core::MakeShared<OtherPipe>();
    log::AddLogStream(engineLogStream);
    log::AddLogStream(otherLogStream);

    for (int32_t i = 0; i < 10; i++) {
        log::Log(i % 2 ? log::LogSource::Other : log::LogSource::Engine,
                 String::CFormat("Test %d", i));

        if (i > 4)
            engineLogStream = nullptr;  // stop logging engine things from now.2
    }

    return 0;
}