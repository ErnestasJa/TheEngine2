#include "window/WindowInc.h"
#include "render/RenderInc.h"
#include "CommonUtil.h"

core::SharedPtr<render::IWindow> CreateWindow(
    const core::SharedPtr<render::IWindowModule> &wmodule);

// clang-format off
#define MULTILINE "#version 330\n\n"

const char *quadVertSource = MULTILINE 
"layout(location = 0) in vec3 pos;      \n"
"                                       \n"
"void main(void)                        \n"
"{                                      \n"
"    gl_Position = vec4(pos, 1);        \n"
"}                                      \n";

const char *quadFragSource = MULTILINE 
"out vec4 FragColor;                    \n"
"                                       \n"
"void main()                            \n"
"{                                      \n"
"    FragColor = vec4(1, 1, 1, 1);      \n"
"}                                      \n";
// clang-format on

struct Mesh {
    std::vector<render::BufferDescriptor> BufferDescriptors;
    core::Vector<render::Vec3f> VertexBuffer;
    core::Vector<uint32_t> IndexBuffer;
    core::SharedPtr<render::IGpuBufferArrayObject> vao;

    Mesh()
    {
    }

    void UploadBuffers()
    {
        BufferDescriptors.clear();

        BufferDescriptors.push_back(render::BufferDescriptor{
            1, render::BufferObjectType::index,
            render::BufferComponentDataType::uint32,
            (uint8_t *)IndexBuffer.data(), (uint32_t)IndexBuffer.size()});

        BufferDescriptors.push_back(render::BufferDescriptor{
            3, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32,
            (uint8_t *)VertexBuffer.data(), (uint32_t)VertexBuffer.size()});
    }
};

Mesh SetupQuad(const core::SharedPtr<render::IRenderer> &renderer);

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<EngineCoutPipe>();
    log::AddLogStream(engineLogStream);

    auto wmodule = render::CreateDefaultWindowModule();
    wmodule->Initialize();

    auto window = CreateWindow(wmodule);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    LoadExtensions();
    auto debugMonitor = GetDebugMessageMonitor();
    auto renderer = render::CreateRenderer();
    auto program = renderer->CreateProgram(quadVertSource, quadFragSource);

    if (!program) {
        log::Log(log::LogSource::Engine, log::LogSeverity::Warn,
                 "Failed to load program");
    } else {
        program->Bind();
    }

    LogDebugMessagesAndFlush(debugMonitor);

    Mesh m = SetupQuad(renderer);

    uint32_t color = 0;

    while (window->ShouldClose() == false) {
        render::Vec3i colorv;

        colorv.r = color % 255;
        colorv.g = (color / 2) % 255;
        colorv.b = (color / 3) % 255;

        renderer->SetClearColor(colorv);
        renderer->Clear();
        m.vao->Render();
        color++;

        window->SwapBuffers();
        window->PollEvents();
    }

    wmodule->Finalize();

    return 0;
}

core::SharedPtr<render::IWindow> CreateWindow(
    const core::SharedPtr<render::IWindowModule> &wmodule)
{
    render::SWindowDefinition wDef;
    wDef.Dimensions = {1280, 720};
    wDef.Title = "Window example application";
    wDef.DebugContext = true;

    return wmodule->CreateWindow(wDef);
}

Mesh SetupQuad(const core::SharedPtr<render::IRenderer> &renderer)
{
    Mesh mesh;
    mesh.VertexBuffer = {{-1, 1, 0}, {-1, -1, 0}, {1, 1, 0}};
    mesh.IndexBuffer = {0, 1, 2, 2, 1, 0};
    mesh.UploadBuffers();

    mesh.vao = renderer->CreateBufferArrayObject(mesh.BufferDescriptors);
    mesh.vao->GetBufferObject(0)->UpdateBuffer(mesh.BufferDescriptors[0]);
    mesh.vao->GetBufferObject(1)->UpdateBuffer(mesh.BufferDescriptors[1]);
    return mesh;
}