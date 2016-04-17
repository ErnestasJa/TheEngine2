#include "window/WindowInc.h"
#include "render/RenderInc.h"
#include "input/InputInc.h"
#include "CommonUtil.h"
#include "Geometry.h"

// clang-format off
const char *quadVertSource = R"V0G0N(
#version 330

layout(location = 0) in vec3 pos;
smooth out vec4 vertPos;

uniform mat4 mvp;

void main(void)                  
{                         
    vertPos = mvp * vec4(pos,1);
    gl_Position = vertPos;
}
)V0G0N";

const char *quadFragSource = R"V0G0N(
#version 330

uniform vec4 colorOffset;
out vec4 FragColor;
in vec4 vertPos;

void main()               
{                 
    FragColor = vec4(sin(vertPos.y-vertPos.x+colorOffset.x), cos(vertPos.x+vertPos.y+colorOffset.y), sin(vertPos.x-vertPos.y+colorOffset.z), 1);

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

    Mesh(render::IRenderer *ptr)
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

    void UploadQuad()
    {
        VertexBuffer = {{-1, 1, 0}, {-1, -1, 0}, {1, 1, 0}, {1, -1, 0}};
        IndexBuffer = {0, 1, 2, 1, 2, 3};
        UploadBuffers();
    }

    void UploadCube()
    {
        VertexBuffer = geom::CubeVertices;
        IndexBuffer = geom::CubeIndices;
        UploadBuffers();
    }

    void Render()
    {
        vao->Render(IndexBuffer.size());
    }
};

class WindowInputHandler : public input::InputHandler
{
public:
    static auto Create(render::IWindow *window, core::SharedPtr<Mesh> mesh)
    {
        return core::MakeShared<WindowInputHandler>(window, mesh);
    }

public:
    WindowInputHandler(render::IWindow *window, core::SharedPtr<Mesh> mesh)
        : m_mesh(mesh), m_window(window)
    {
    }

    virtual bool OnKeyDown(const input::Key &key,
                           const bool IsRepeated) override
    {
        if (key == input::Keys::C) {
            m_mesh->UploadCube();
        } else if (key == input::Keys::Q) {
            m_mesh->UploadQuad();
        } else if (key == input::Keys::W) {
            m_mvp = glm::translate(m_mvp, glm::vec3(0, 0, +1));
        } else if (key == input::Keys::S) {
            m_mvp = glm::translate(m_mvp, glm::vec3(0, 0, -1));
        } else if (key == input::Keys::A) {
            m_mvp = glm::translate(m_mvp, glm::vec3(-1, 0, 0));
        } else if (key == input::Keys::D) {
            m_mvp = glm::translate(m_mvp, glm::vec3(+1, 0, 0));
        }

        return false;
    }

    glm::mat4 GetViewMatrix()
    {
        return m_mvp;
    }

private:
    glm::mat4 m_mvp;
    core::SharedPtr<Mesh> m_mesh;
    render::IWindow *m_window;
};

class BaseMaterial
{
public:
    BaseMaterial(const core::SharedPtr<render::IGpuProgram> &shader)
    {
        u_mvp = shader->GetUniform("mvp");
    }

    void SetUniforms()
    {
        u_mvp->SetMat4(glm::value_ptr(MVP));
    }

    glm::mat4 MVP;

private:
    render::IGpuProgramUniform *u_mvp;
};

render::SWindowDefinition GetWindowDefinition();

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<sutil::EngineCoutPipe>();
    elog::AddLogStream(engineLogStream);

    auto context = render::CreateContext(GetWindowDefinition());
    auto window = context->GetWindow().get();
    auto renderer = context->GetRenderer().get();

    auto dbg = renderer->GetDebugMessageMonitor();
    if (dbg) dbg->SetDebugging(true);

    auto program = renderer->CreateProgram(quadVertSource, quadFragSource);

    if (!program) {
        elog::Log(elog::LogSource::Engine, elog::LogSeverity::Warn,
                  "Failed to load program");
        return -1;
    }

    program->Bind();
    sutil::LogShaderUniforms(program);

    auto mesh = core::MakeShared<Mesh>(renderer);
    auto handler = WindowInputHandler::Create(window, mesh);
    window->GetInputDevice().lock()->SetInputHandler(handler);

    uint32_t color = 0;
    glm::vec3 rotation = {0, 0, 0};
    glm::vec4 colorOffset = {0, 0, 0, 0};

    // matrix setup
    glm::mat4 ViewMat = glm::lookAt(glm::vec3(0, 0, -8),  // position
                                    glm::vec3(0, 0, 1),   // look at
                                    glm::vec3(0, 1, 0)    // up vec
                                    );

    glm::mat4 ProjectionMat =
        glm::perspective(45.0f,             // FoV
                         1280.0f / 720.0f,  // Aspect Ratio.
                         0.1f,              // Near clipping plane.
                         100.0f             // Far clipping plane.
                         );

    BaseMaterial material(program);

    // make stuff happen
    while (window->ShouldClose() == false) {
        auto colorv = core::pod::Vec3<int32_t>{color % 255, (color / 2) % 255,
                                               (color / 3) % 255};
        auto RotationMat = sutil::BuildRotation(rotation);

        rotation += glm::vec3(0.0025f, 0.0030f, 0.0020f);
        colorOffset += glm::vec4(0.01, 0.015, 0.02, 0);
        color++;

        material.MVP = ProjectionMat * handler->GetViewMatrix() * RotationMat;
        material.SetUniforms();

        renderer->SetClearColor(colorv);
        renderer->Clear();
        mesh->Render();

        window->SwapBuffers();
        window->PollEvents();
        // sutil::LogDebugMessagesAndFlush(debugMonitor);
    }

    return 0;
}

render::SWindowDefinition GetWindowDefinition()
{
    render::SWindowDefinition wDef;
    wDef.Dimensions = {1280, 720};
    wDef.ForwardCompatible = true;
    wDef.Title = "C - cube, Q - quad, T - triangle";
    wDef.DebugContext = true;

    return wDef;
}
