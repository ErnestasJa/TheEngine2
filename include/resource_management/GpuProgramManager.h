#ifndef THEPROJECT2_GPUPROGRAMMANAGER_H
#define THEPROJECT2_GPUPROGRAMMANAGER_H

namespace render {
    class IGpuProgram;
    class IRenderer;
}

namespace res {
struct GpuProgramInfo
{
    core::String Path;
    core::String Alias;
    core::SharedPtr<render::IGpuProgram> Program;
};

///todo: remove, ResourceManager now loads materials/shaders.
class GpuProgramManager
{
public:
    explicit GpuProgramManager(render::IRenderer* renderer, io::IFileSystem* fs);
    render::IGpuProgram* LoadProgram(const core::String& path, const core::String& alias = "");

private:
    core::String LoadShaderSource(const core::String& path);

private:
    render::IRenderer* m_renderer;
    io::IFileSystem* m_fileSystem;
    std::vector<GpuProgramInfo> m_loadedPrograms;
};
}


#endif //THEPROJECT2_GPUPROGRAMMANAGER_H
