#include "resource_management/GpuProgramManager.h"
#include "render/IRenderer.h"
#include "render/IGpuProgram.h"
#include "filesystem/IFileSystem.h"

namespace res {
GpuProgramManager::GpuProgramManager(render::IRenderer* renderer, io::IFileSystem* fs)
{
    m_fileSystem = fs;
    m_renderer   = renderer;
}

render::IGpuProgram* GpuProgramManager::LoadProgram(const core::String& path,
                                                    const core::String& alias)
{
    auto programInfo =
        std::find_if(m_loadedPrograms.begin(), m_loadedPrograms.end(),
                     [&](const GpuProgramInfo& p) { return !alias.empty() && p.Alias == alias || p.Path == path; });

    if (programInfo != m_loadedPrograms.end()) {
        return (*programInfo).Program.get();
    }

    auto vertexSource   = LoadShaderSource(path + ".vert");
    auto fragmentSource = LoadShaderSource(path + ".frag");
    auto geometrySource = LoadShaderSource(path + ".geom");

    auto gpuProgram = m_renderer->CreateProgram(vertexSource, fragmentSource, geometrySource);

    if (gpuProgram) {
        GpuProgramInfo gpuProgramInfo;

        gpuProgramInfo.Alias   = alias;
        gpuProgramInfo.Path    = path;
        gpuProgramInfo.Program = gpuProgram;

        m_loadedPrograms.push_back(gpuProgramInfo);
    }

    return gpuProgram.get();
}

core::String GpuProgramManager::LoadShaderSource(const core::String& path)
{
    core::String fileContents;

    auto file = m_fileSystem->OpenRead(path);
    if (file) {
        file->Read(fileContents);
        elog::LogInfo(fileContents);
    }
    else {
        elog::LogInfo("Failed to read shader source: " + path);
    }

    return fileContents;
}
}