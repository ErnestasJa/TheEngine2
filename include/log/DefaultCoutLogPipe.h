//
// Created by ernes on 9/23/2019.
//

#ifndef THEPROJECT2_DEFAULTCOUTLOGPIPE_H
#define THEPROJECT2_DEFAULTCOUTLOGPIPE_H

#include <iostream>

namespace elog {

class DefaultCoutLogPipe : public ILogStream
{
  public:
    ~DefaultCoutLogPipe() override = default;
    void Log(const elog::LogSource source, const elog::LogSeverity severity,
           const core::String& logStr) override
    {
    if (source == elog::LogSource::Engine)
      std::cout << "Engine log: " << logStr << std::endl;
    }
};

} // namespace elog

#endif // THEPROJECT2_DEFAULTCOUTLOGPIPE_H
