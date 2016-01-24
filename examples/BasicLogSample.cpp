#include "log/LogInc.h"
#include "core/StringExt.h"
#include <iostream>

class CoutPipe : public log::ILogPipe
{
public:
    void Log(const core::String &logStr)
    {
        std::cout << logStr << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    using log = log::TLog;
    namespace string = core::string;

    for (int32_t i = 0; i < 10; i++) {
        auto coutPipe = core::MakeShared<CoutPipe>();
        log::Get().AttachPipe(coutPipe);
        log::Get().Log(string::CFormat("Test %d", i));
    }

    log::Get().Log("Should not be logged anywhere.");
    log::Get().CleanDeadPipes();
    log::Get().Log("Also should not be logged anywhere.");

    auto coutPipe = core::MakeShared<CoutPipe>();
    log::Get().AttachPipe(coutPipe);
    log::Get().Log("Last log to cout.");

    return 0;
}