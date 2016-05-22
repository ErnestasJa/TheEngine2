#include "Include.h"
#include "render/RenderInc.h"
#include "window/WindowInc.h"

render::SWindowDefinition GetWindowDefinition();
core::Vector<core::SharedPtr<render::ITexture>> LoadTextures(
    render::IRenderer *renderer);

int main(int argc, char const *argv[])
{
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

    auto mat = material::BaseMaterial::CreateMaterial(renderer);
    if (!mat) {
        sutil::LogDebugMessagesAndFlush(renderer->GetDebugMessageMonitor());
    }

    auto textures = LoadTextures(renderer);

    auto obj =
        RenderObject(renderer, core::MakeShared<Mesh>(renderer), mat, textures);
    obj.Transform = glm::translate(glm::vec3(0, 0, 10));

    auto cam = core::MakeShared<Camera>((float)window->GetDimensions().w /
                                        (float)window->GetDimensions().h);

    window->GetInputDevice().lock()->SetInputHandler(
        CamInputHandler::Create(cam));

    renderer->SetClearColor({125, 125, 225});
    window->SetCursorMode(render::CursorMode::HiddenCapture);

    // make stuff happen
    while (window->ShouldClose() == false) {
        renderer->Clear();

        material::SharedUniforms.View = cam->GetView();
        material::SharedUniforms.Projection = cam->GetProjection();
        obj.Render();

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

core::Vector<core::SharedPtr<render::ITexture>> LoadTextures(
    render::IRenderer *renderer)
{
    auto texture = renderer->CreateTexture(render::TextureDescriptor());
    texture->UploadData(render::TextureDataDescriptor{
        (void *)texture::diffuse_texture_bytes.pixel_data,
        render::TextureDataFormat::RGB,
        core::pod::Vec2<int32_t>{
            (int32_t)texture::diffuse_texture_bytes.width,
            (int32_t)texture::diffuse_texture_bytes.height}});

    auto texture2 = renderer->CreateTexture(render::TextureDescriptor());
    texture2->UploadData(render::TextureDataDescriptor{
        (void *)texture::normal_map_texture_bytes.pixel_data,
        render::TextureDataFormat::RGB,
        core::pod::Vec2<int32_t>{
            (int32_t)texture::normal_map_texture_bytes.width,
            (int32_t)texture::normal_map_texture_bytes.height}});

    return core::Vector<decltype(texture)>{texture, texture2};
}