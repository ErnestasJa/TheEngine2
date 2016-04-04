#include "log/LogInc.h"
#include "core/StringExt.h"
#include <iostream>

namespace String = core::string;

class EngineCoutPipe : public logging::ILogStream
{
public:
    void Log(const logging::LogSource source,
             const logging::LogSeverity severity, const core::String &logStr)
    {
        if (source == logging::LogSource::Engine)
            std::cout << "Engine log: " << logStr << std::endl;
    }
};

class OtherPipe : public logging::ILogStream
{
public:
    void Log(const logging::LogSource source,
             const logging::LogSeverity severity, const core::String &logStr)
    {
        if (source == logging::LogSource::Other)
            printf("Other: %s\n", logStr.c_str());
    }
};

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<EngineCoutPipe>();
    auto otherLogStream = core::MakeShared<OtherPipe>();
    logging::AddLogStream(engineLogStream);
    logging::AddLogStream(otherLogStream);

    for (int32_t i = 0; i < 10; i++) {
        auto logSrc =
            i % 2 ? logging::LogSource::Other : logging::LogSource::Engine;
        logging::Log(logSrc, logging::LogSeverity::Info,
                     String::CFormat("Test %d", i));

        if (i > 4)
            engineLogStream = nullptr;  // stop logging engine things from now.2
    }

    return 0;
}