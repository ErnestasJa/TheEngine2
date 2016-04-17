#ifndef COMMONUTIL_H
#define COMMONUTIL_H
#include "render/RenderInc.h"
#include "log/LogInc.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <algorithm>
#include <iostream>

namespace sutil
{
template <class T, class U>
inline U convert(const T &vec);

inline core::pod::Vec3<float> convert(const glm::vec3 &value)
{
    return core::pod::Vec3<float>{value.x, value.y, value.z};
}

inline core::pod::Vec4<float> convert(const glm::vec4 &value)
{
    return core::pod::Vec4<float>{value.x, value.y, value.z, value.w};
}

inline glm::vec3 convert(const core::pod::Vec3<float> &value)
{
    return glm::vec3{value.x, value.y, value.z};
}

inline glm::vec4 convert(const core::pod::Vec4<float> &value)
{
    return glm::vec4{value.x, value.y, value.z, value.w};
}

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

void LogEngine(const core::String &message)
{
    elog::Log(elog::LogSource::Engine, elog::LogSeverity::Debug, message);
}

void LogDebugMessagesAndFlush(render::IRendererDebugMessageMonitor *dbgMonitor)
{
    if (dbgMonitor) {
        for (auto msg : dbgMonitor->GetMessages()) {
            elog::Log(elog::LogSource::Engine, elog::LogSeverity::Debug,
                      msg->AsString());
        }

        dbgMonitor->ClearMessages();
    }
}

void LogShaderUniforms(core::SharedPtr<render::IGpuProgram> const &program)
{
    for (const auto &uniform : program->GetUniforms()) {
        LogEngine(
            core::string::CFormat("Uniform [%s]", uniform->GetName().c_str()));
    }
}

glm::mat4 BuildRotation(const glm::vec3 &rotation)
{
    glm::mat4 m;
    m = glm::rotate(m, rotation.x, glm::vec3(1.f, 0.f, 0.f));
    m = glm::rotate(m, rotation.y, glm::vec3(0.f, 1.f, 0.f));
    m = glm::rotate(m, rotation.z, glm::vec3(0.f, 0.f, 1.f));
    return m;
}
}

#endif