#include "ImageLoader.h"
#include "Include.h"
#include "filesystem/IFileSystem.h"
#include "platform/IPlatformFileSystem.h"
#include "render/RenderInc.h"
#include "window/WindowInc.h"

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

render::SWindowDefinition GetWindowDefinition();
core::Vector<core::SharedPtr<render::ITexture>> LoadTextures(
    render::IRenderer *renderer);

int main(int argc, char const *argv[])
{
    core::String appPath =
        platform::GetPlatformFileSystem()->GetExecutableDirectory();

    auto fileSystem = io::CreateFileSystem(appPath);
    fileSystem->AddSearchDirectory(appPath);
    fileSystem->SetWriteDirectory(appPath);

    auto engineLogStream = core::MakeShared<sutil::EngineCoutPipe>();
    elog::AddLogStream(engineLogStream);

    auto context = render::CreateContext(GetWindowDefinition());
    auto window = context->GetWindow().get();
    auto renderer = context->GetRenderer().get();
    auto dbg = renderer->GetDebugMessageMonitor();

    if (dbg)
        dbg->SetDebugging(true);
    else
        elog::Log(elog::LogSource::Engine, elog::LogSeverity::Warn,
                  "debug monitor not available.");

    auto mat = material::CreatePhongMaterial(renderer);
    auto mat2 = material::CreateCellMaterial(renderer);

    elog::Log(elog::LogSource::Engine, elog::LogSeverity::Info,
              core::string::CFormat("App path: %s", appPath.c_str()));

    ImageLoader imageLoader(fileSystem, renderer);
    auto detail = imageLoader.LoadImage(io::Path("resources/diffuse.png"));
    auto normal = imageLoader.LoadImage(io::Path("resources/normal.png"));

    auto textures =
        core::Vector<core::SharedPtr<render::ITexture>>{detail, normal};

    core::Vector<RenderObject> objs;

    for (int32_t x = 0; x < 10; x++)
        for (int32_t z = 0; z < 10; z++) {
            auto obj = RenderObject(renderer, core::MakeShared<Mesh>(renderer),
                                    z % 2 ? mat : mat2, textures);
            obj.Transform = glm::translate(glm::vec3(x * 2, 0, z * 2));
            objs.push_back(obj);
        }

    auto cam = core::MakeShared<Camera>((float)window->GetDimensions().w /
                                        (float)window->GetDimensions().h);

    window->GetInputDevice().lock()->SetInputHandler(
        CamInputHandler::Create(cam));

    renderer->SetClearColor({125, 125, 225});
    window->SetCursorMode(render::CursorMode::HiddenCapture);

    while (window->ShouldClose() == false) {
        renderer->Clear();

        material::SharedUniforms.View = cam->GetView();
        material::SharedUniforms.Projection = cam->GetProjection();

        for (const auto &obj : objs) obj.Render();

        window->SwapBuffers();
        window->PollEvents();
        sutil::LogDebugMessagesAndFlush(dbg);
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