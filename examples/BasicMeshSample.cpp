#include "window/WindowInc.h"
#include "render/RenderInc.h"
#include "input/InputInc.h"
#include "CommonUtil.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <algorithm>

core::SharedPtr<render::IWindow> CreateWindow(
    const core::SharedPtr<render::IWindowModule> &wmodule);

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
        : m_mesh(mesh), m_window(window), m_mvp(glm::mat4(1.0f))
    {
    }

    virtual bool OnKeyDown(const input::Key &key,
                           const bool IsRepeated) override
    {
        if (key == input::Keys::C) {
            // clang-format off
            m_mesh->VertexBuffer = {
                {-1, +1, -1}, {-1, +1, +1}, {+1, +1, +1}, {+1, +1, -1},   // top
                {-1, -1, -1}, {-1, -1, +1}, {+1, -1, +1}, {+1, -1, -1},  // bot
            };

            m_mesh->IndexBuffer = {
            //begin cube
            0,1,2,0,2,3, //top face
            4,5,6,4,6,7, //top face
            4,0,3,4,3,7, //front face
            5,1,2,5,2,6, //back face
            4,0,5,0,5,1, //front face
            7,3,6,6,3,2, //back face
            };

            // clang-format on
            m_mesh->UploadBuffers();
        } else if (key == input::Keys::Q) {
            // clang-format off
            m_mesh->VertexBuffer = {
                {-1, 1, 0}, {-1, -1, 0}, {1, 1, 0}, {1, -1, 0}};

            m_mesh->IndexBuffer = {0, 1, 2, 1, 2, 3};

            // clang-format on
            m_mesh->UploadBuffers();
        } else if (key == input::Keys::T) {
            m_mesh->VertexBuffer = {{-1, 1, 0}, {-1, -1, 0}, {1, 1, 0}};
            m_mesh->IndexBuffer = {0, 1, 2};
            m_mesh->UploadBuffers();
        } else if (key == input::Keys::I) {
            m_mvp = glm::translate(m_mvp, glm::vec3(0, 0, +1));
        } else if (key == input::Keys::K) {
            m_mvp = glm::translate(m_mvp, glm::vec3(0, 0, -1));
        } else if (key == input::Keys::J) {
            m_mvp = glm::translate(m_mvp, glm::vec3(-1, 0, 0));
        } else if (key == input::Keys::L) {
            m_mvp = glm::translate(m_mvp, glm::vec3(+1, 0, 0));
        }

        return false;
    }

    glm::mat4 GetMvp()
    {
        return m_mvp;
    }

private:
    glm::mat4 m_mvp;
    core::SharedPtr<Mesh> m_mesh;
    core::WeakPtr<render::IWindow> m_window;
};
void LogShaderUniforms(core::SharedPtr<render::IGpuProgram> const &program);

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<EngineCoutPipe>();
    logging::AddLogStream(engineLogStream);

    auto wmodule = render::CreateDefaultWindowModule();
    wmodule->Initialize();

    auto window = CreateWindow(wmodule);

    LoadExtensions();
    auto debugMonitor = GetDebugMessageMonitor();
    auto renderer = render::CreateRenderer();
    auto program = renderer->CreateProgram(quadVertSource, quadFragSource);

    if (!program) {
        logging::Log(logging::LogSource::Engine, logging::LogSeverity::Warn,
                     "Failed to load program");
        return -1;
    }

    program->Bind();
    LogShaderUniforms(program);

    auto mesh = core::MakeShared<Mesh>(renderer);
    auto handler = WindowInputHandler::Create(window, mesh);
    window->GetInputDevice().lock()->SetInputHandler(handler);

    uint32_t color = 0;
    core::pod::Vec4<float> uniform = {0, 0, 0, 0};
    core::pod::Vec3<int> colorv = {0, 0, 0};
    core::pod::Vec3<float> rotation = {0, 0, 0};

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

    // make stuff happen
    while (window->ShouldClose() == false) {
        colorv.r = color % 255;
        colorv.g = (color / 2) % 255;
        colorv.b = (color / 3) % 255;
        color++;

        uniform.x += 0.01;
        uniform.y += 0.02;
        uniform.z += 0.03;

        const auto &transform = std::find_if(
            program->GetUniforms().begin(), program->GetUniforms().end(),
            [](const auto &uniform) { return uniform->GetName() == "mvp"; });

        if (transform != program->GetUniforms().end()) {
            glm::mat4 RotationMat(1.0f);

            RotationMat =
                glm::rotate(RotationMat, rotation.x, glm::vec3(1.f, 0.f, 0.f));
            RotationMat =
                glm::rotate(RotationMat, rotation.y, glm::vec3(0.f, 1.f, 0.f));
            RotationMat =
                glm::rotate(RotationMat, rotation.z, glm::vec3(0.f, 0.f, 1.f));

            auto mvp = ProjectionMat * ViewMat * RotationMat;
            (*transform)->SetMat4(glm::value_ptr(mvp));

            rotation.x += 0.0025f;
            rotation.y += 0.0030f;
            rotation.z += 0.0020f;
        }

        const auto &colorUniform =
            std::find_if(program->GetUniforms().begin(),
                         program->GetUniforms().end(), [](const auto &uniform) {
                             return uniform->GetName() == "colorOffset";
                         });

        if (colorUniform != program->GetUniforms().end()) {
            (*colorUniform)->Set(uniform);
        }

        const auto &someFloat =
            std::find_if(program->GetUniforms().begin(),
                         program->GetUniforms().end(), [](const auto &uniform) {
                             return uniform->GetName() == "someFloat";
                         });

        if (someFloat != program->GetUniforms().end()) {
            (*someFloat)->Set(0.7f);
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

void LogShaderUniforms(core::SharedPtr<render::IGpuProgram> const &program)
{
    for (const auto &uniform : program->GetUniforms()) {
        LogEngine(
            core::string::CFormat("Uniform [%s]", uniform->GetName().c_str()));
    }
}

core::SharedPtr<render::IWindow> CreateWindow(
    const core::SharedPtr<render::IWindowModule> &wmodule)
{
    render::SWindowDefinition wDef;
    wDef.Dimensions = {1280, 720};
    wDef.Title = "C - cube, Q - quad, T - triangle";
    wDef.DebugContext = true;

    return wmodule->CreateWindow(wDef);
}
