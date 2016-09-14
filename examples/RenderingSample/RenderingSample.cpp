#include "ImageLoader.h"
#include "Include.h"
#include "filesystem/IFileSystem.h"
#include "platform/IPlatformFileSystem.h"
#include "render/RenderInc.h"
#include "window/WindowInc.h"

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

render::SWindowDefinition GetWindowDefinition();
core::Vector<core::SharedPtr<render::ITexture>> LoadTextures(render::IRenderer* renderer);
core::Vector<RenderObject> BuildScene(
    render::IRenderer* renderer,
    const core::Vector<core::SharedPtr<material::BaseMaterial>>& materials,
    const core::Vector<core::SharedPtr<render::ITexture>>& textures);

int main(int argc, char const* argv[])
{
    auto appPath = platform::GetPlatformFileSystem()->GetExecutableDirectory();

    auto fsPtr      = io::CreateFileSystem(appPath);
    auto fileSystem = fsPtr.get();
    fileSystem->AddSearchDirectory(appPath);
    fileSystem->SetWriteDirectory(appPath);

    auto engineLogStream = core::MakeShared<sutil::EngineCoutPipe>();
    elog::AddLogStream(engineLogStream);

    auto context  = render::CreateContext(GetWindowDefinition());
    auto window   = context->GetWindow().get();
    auto renderer = context->GetRenderer().get();
    auto dbg      = renderer->GetDebugMessageMonitor();

    if (dbg)
        dbg->SetDebugging(true);
    else
        elog::LogWarning("debug monitor not available.");

    auto phongProgram =
        material::CreateMaterialShader(renderer, fileSystem, "resources/shaders/phong");
    auto cellProgram =
        material::CreateMaterialShader(renderer, fileSystem, "resources/shaders/cell");

    auto materials = core::Vector<core::SharedPtr<material::BaseMaterial>>{
        core::SharedPtr<material::BaseMaterial>(new material::PhongMaterial(phongProgram)),
        core::SharedPtr<material::BaseMaterial>(new material::CellMaterial(cellProgram))
    };

    ImageLoader imageLoader(fsPtr, renderer);
    auto detail   = imageLoader.LoadImage(io::Path("resources/diffuse.png"));
    auto normal   = imageLoader.LoadImage(io::Path("resources/normal.png"));
    auto textures = core::Vector<core::SharedPtr<render::ITexture>>{ detail, normal };

    core::Vector<RenderObject> objs = BuildScene(renderer, materials, textures);

    auto cam = core::MakeShared<Camera>((float)window->GetDimensions().w /
                                        (float)window->GetDimensions().h);

    window->GetInputDevice().lock()->SetInputHandler(CamInputHandler::Create(cam));
    window->SetCursorMode(render::CursorMode::HiddenCapture);

    renderer->SetClearColor({ 125, 125, 225 });
    while (window->ShouldClose() == false) {
        renderer->Clear();

        material::SharedUniforms.View       = cam->GetView();
        material::SharedUniforms.Projection = cam->GetProjection();

        for (const auto& obj : objs)
            obj.Render();

        window->SwapBuffers();
        window->PollEvents();
        sutil::LogDebugMessagesAndFlush(dbg);
    }

    return 0;
}

core::Vector<RenderObject> BuildScene(
    render::IRenderer* renderer,
    const core::Vector<core::SharedPtr<material::BaseMaterial>>& materials,
    const core::Vector<core::SharedPtr<render::ITexture>>& textures)
{
    core::Vector<RenderObject> objs;

    for (int32_t y = 0; y < 10; y++) {
        float chanceForCube = glm::pow(100.0f, (20 - y * 2) * 0.1) / 100.0f;

        for (int32_t x = 0; x < 10; x++) {
            for (int32_t z = 0; z < 10; z++) {
                bool addCube = rand() % 100 < chanceForCube;

                if (addCube) {
                    auto material = materials[rand() % 2];

                    auto obj = RenderObject(renderer, core::MakeShared<Mesh>(renderer), material,
                                            textures);

                    obj.Transform = glm::translate(glm::vec3(x * 2, y * 2, z * 2));
                    objs.push_back(obj);
                }
            }
        }
    }

    return objs;
}

render::SWindowDefinition GetWindowDefinition()
{
    render::SWindowDefinition wDef;
    wDef.Dimensions        = { 1280, 720 };
    wDef.ForwardCompatible = true;
    wDef.Title             = "Rendering sample";
    wDef.DebugContext      = true;

    return wDef;
}