#include "window/WindowInc.h"
#include "render/RenderInc.h"
#include "CommonUtil.h"

core::SharedPtr<render::IWindow> CreateWindow(
    const core::SharedPtr<render::IWindowModule> &wmodule);

const char *vertSource =
    R"(#version 330

void main(void)
{
    gl_Position = vec4(1,1,1,1);
}
)";

const char *fragSource =
    R"(#version 330
out vec4 FragColor;

void main()
{
    FragColor = vec4(1,1,1,1);
}
)";

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<sutil::EngineCoutPipe>();
    elog::AddLogStream(engineLogStream);

    auto wmodule = render::CreateDefaultWindowModule();
    wmodule->Initialize();

    auto window = CreateWindow(wmodule);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    sutil::LoadExtensions();
    auto debugMonitor = sutil::GetDebugMessageMonitor();
    auto renderer = render::CreateRenderer();
    auto program = renderer->CreateProgram(vertSource, fragSource);
    auto program2 = renderer->CreateProgram(core::String(vertSource) + "fail",
                                            core::String(fragSource) + "fail");

    if (!program) {
        elog::Log(elog::LogSource::Engine, elog::LogSeverity::Warn,
                  "Failed to load program");
    } else {
        program->Bind();
    }

    if (!program2) {
        elog::Log(elog::LogSource::Engine, elog::LogSeverity::Warn,
                  "Failed to load program2");
    }

    uint32_t color = 0;

    while (window->ShouldClose() == false) {
        render::Vec3i colorv;

        colorv.r = color % 255;
        colorv.g = (color / 2) % 255;
        colorv.b = (color / 3) % 255;

        renderer->SetClearColor(colorv);
        renderer->Clear();
        color++;

        sutil::LogDebugMessagesAndFlush(debugMonitor);

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
