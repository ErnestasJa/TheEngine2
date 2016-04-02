#include "window/WindowInc.h"
#include "render/RenderInc.h"
#include "input/InputInc.h"
#include "CommonUtil.h"

core::SharedPtr<render::IWindow> CreateWindow(
    const core::SharedPtr<render::IWindowModule> &wmodule);

// clang-format off

const char *quadVertSource = R"V0G0N(
#version 330

layout(location = 0) in vec3 pos;
smooth out vec3 posx;

void main(void)                  
{                                
    gl_Position = vec4(pos, 1);  
    posx = pos;                  
}
)V0G0N";

const char *quadFragSource = R"V0G0N(
#version 330

uniform vec4 colorOffset;
out vec4 FragColor;
in vec3 posx;

void main()               
{                         
    FragColor = vec4(sin(posx.y-posx.x+colorOffset.x), cos(posx.x+posx.y+colorOffset.y), sin(posx.x-posx.y+colorOffset.z), 1) +vec4(0.5,0.5,0.5,0.5);
};
)V0G0N";
// clang-format on

struct Mesh {
    std::vector<render::BufferDescriptor> BufferDescriptors;
    core::Vector<render::Vec3f> VertexBuffer;
    core::Vector<uint32_t> IndexBuffer;
    core::SharedPtr<render::IGpuBufferArrayObject> vao;

    Mesh()
    {
    }

    Mesh(const core::SharedPtr<render::IRenderer> &ptr)
    {
        BufferDescriptors.push_back(render::BufferDescriptor{
            1, render::BufferObjectType::index,
            render::BufferComponentDataType::uint32, 0});

        BufferDescriptors.push_back(render::BufferDescriptor{
            3, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 0});

        vao = ptr->CreateBufferArrayObject(BufferDescriptors);
    }

    void UploadBuffers()
    {
        vao->GetBufferObject(0)
            ->UpdateBuffer(IndexBuffer.size(), IndexBuffer.data());
        vao->GetBufferObject(1)
            ->UpdateBuffer(VertexBuffer.size(), VertexBuffer.data());
    }

    void Render()
    {
        vao->Render(IndexBuffer.size());
    }
};

class WindowInputHandler : public input::InputHandler
{
public:
    static auto Create(core::WeakPtr<render::IWindow> window,
                       core::SharedPtr<Mesh> mesh)
    {
        return core::MakeShared<WindowInputHandler>(window, mesh);
    }

public:
    WindowInputHandler(core::WeakPtr<render::IWindow> window,
                       core::SharedPtr<Mesh> mesh)
        : m_mesh(mesh), m_window(window)
    {
    }

    virtual bool OnKeyDown(const input::Key &key,
                           const bool IsRepeated) override
    {
        if (key == input::Keys::Q) {
            m_mesh->VertexBuffer = {
                {-1, 1, 0}, {-1, -1, 0}, {1, 1, 0}, {1, -1, 0}};
            m_mesh->IndexBuffer = {0, 1, 2, 1, 2, 3};
            m_mesh->UploadBuffers();
        } else if (key == input::Keys::T) {
            m_mesh->VertexBuffer = {{-1, 1, 0}, {-1, -1, 0}, {1, 1, 0}};
            m_mesh->IndexBuffer = {0, 1, 2};
            m_mesh->UploadBuffers();
        } else if (key == input::Keys::Y) {
            m_mesh->VertexBuffer = {{-1, -1, 0}, {1, 1, 0}, {1, -1, 0}};
            m_mesh->IndexBuffer = {0, 1, 2};
            m_mesh->UploadBuffers();
        }
        return false;
    }

private:
    core::SharedPtr<Mesh> m_mesh;
    core::WeakPtr<render::IWindow> m_window;
};

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<EngineCoutPipe>();
    log::AddLogStream(engineLogStream);

    auto wmodule = render::CreateDefaultWindowModule();
    wmodule->Initialize();

    auto window = CreateWindow(wmodule);

    LoadExtensions();
    auto debugMonitor = GetDebugMessageMonitor();
    auto renderer = render::CreateRenderer();
    auto program = renderer->CreateProgram(quadVertSource, quadFragSource);

    if (!program) {
        log::Log(log::LogSource::Engine, log::LogSeverity::Warn,
                 "Failed to load program");
        return -1;
    }

    program->Bind();

    auto mesh = core::MakeShared<Mesh>(renderer);
    auto handler = WindowInputHandler::Create(window, mesh);
    window->GetInputDevice().lock()->SetInputHandler(handler);

    uint32_t color = 0;
    core::pod::Vec4<float> uniform = {0, 0, 0, 0};
    core::pod::Vec3<int> colorv;

    while (window->ShouldClose() == false) {
        colorv.r = color % 255;
        colorv.g = (color / 2) % 255;
        colorv.b = (color / 3) % 255;
        color++;

        uniform.x += 0.01;
        uniform.y += 0.02;
        uniform.z += 0.03;

        const auto &uniforms = program->GetUniforms();
        if (uniforms.size()) {
            uniforms[0]->Set(uniform);
        }

        renderer->SetClearColor(colorv);
        renderer->Clear();
        mesh->Render();

        window->SwapBuffers();
        window->PollEvents();
        LogDebugMessagesAndFlush(debugMonitor);
    }

    wmodule->Finalize();

    return 0;
}

core::SharedPtr<render::IWindow> CreateWindow(
    const core::SharedPtr<render::IWindowModule> &wmodule)
{
    render::SWindowDefinition wDef;
    wDef.Dimensions = {1280, 720};
    wDef.Title = "Q - quad, T - trangle";
    wDef.DebugContext = true;

    return wmodule->CreateWindow(wDef);
}
