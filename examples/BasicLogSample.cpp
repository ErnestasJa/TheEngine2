#include "log/LogInc.h"
#include "core/StringExt.h"
#include <iostream>

namespace String = core::string;

class EngineCoutPipe : public elog::ILogStream
{
public:
    void Log(const elog::LogSource source, const elog::LogSeverity severity,
             const core::String &logStr)
    {
        if (source == elog::LogSource::Engine)
            std::cout << "Engine log: " << logStr << std::endl;
    }
};

class OtherPipe : public elog::ILogStream
{
public:
    void Log(const elog::LogSource source, const elog::LogSeverity severity,
             const core::String &logStr)
    {
        if (source == elog::LogSource::Other)
            printf("Other: %s\n", logStr.c_str());
    }
};

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<EngineCoutPipe>();
    auto otherLogStream = core::MakeShared<OtherPipe>();
    elog::AddLogStream(engineLogStream);
    elog::AddLogStream(otherLogStream);

    for (int32_t i = 0; i < 10; i++) {
        auto logSrc = i % 2 ? elog::LogSource::Other : elog::LogSource::Engine;
        elog::Log(logSrc, elog::LogSeverity::Info,
                  String::CFormat("Test %d", i));

        if (i > 4)
            engineLogStream = nullptr;  // stop elog engine things from now.2
    }

    return 0;
}