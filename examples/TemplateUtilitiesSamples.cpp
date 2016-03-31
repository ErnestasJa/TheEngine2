#include "log/LogInc.h"
#include "core/StringExt.h"
#include <iostream>

namespace String = core::string;

class LogPipe : public log::ILogStream
{
public:
    void Log(const log::LogSource source, const log::LogSeverity severity,
             const core::String &logStr)
    {
        if (source == log::LogSource::Other)
            printf("Other: %s\n", logStr.c_str());
    }
};
int main(int argc, char const *argv[])
{
    auto logStream = core::MakeShared<LogPipe>();
    log::AddLogStream(logStream);

    int container[] = {10, 20, -10, 30};
    core::Vector<int> vcontainer = {100, 200, 3000};

    bool result = core::Any(container, [](int val) { return val > 20; });
    bool result2 = core::Any(vcontainer, [](int val) { return val > 20; });

    log::Log(
        log::LogSource::Other, log::LogSeverity::Info,
        core::string::CFormat("container -> all higher than 20: %d", result));

    log::Log(
        log::LogSource::Other, log::LogSeverity::Info,
        core::string::CFormat("vcontainer -> all higher than 20: %d", result2));
    return 0;
}
