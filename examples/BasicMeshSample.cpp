#include "window/WindowInc.h"
#include "render/RenderInc.h"
#include "input/InputInc.h"
#include "CommonUtil.h"
#include "Geometry.h"
#include "Textures.h"

// clang-format off
const char *quadVertSource = R"V0G0N(
#version 330
#extension GL_ARB_shading_language_420pack : enable
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) in vec2 vertexUV;
layout(location = 1) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec3 vertexTangent_modelspace;
layout(location = 4) in vec3 vertexBitangent_modelspace;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 LightDirection_tangentspace;
out vec3 EyeDirection_tangentspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform vec3 LightPosition_worldspace;

void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;
    vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
    
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
    
    vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
    
    UV = vertexUV;

    // model to camera = ModelView
    vec3 vertexTangent_cameraspace = MV3x3 * vertexTangent_modelspace;
    vec3 vertexBitangent_cameraspace = MV3x3 * vertexBitangent_modelspace;
    vec3 vertexNormal_cameraspace = MV3x3 * vertexNormal_modelspace;
    
    mat3 TBN = transpose(mat3(
        vertexTangent_cameraspace,
        vertexBitangent_cameraspace,
        vertexNormal_cameraspace    
    ));

    LightDirection_tangentspace = TBN * LightDirection_cameraspace;
    EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;
}

)V0G0N";

const char *quadFragSource = R"V0G0N(
#version 330
#extension GL_ARB_shading_language_420pack : enable
#extension GL_ARB_explicit_uniform_location : enable

in vec2 UV;
in vec3 Position_worldspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 LightDirection_tangentspace;
in vec3 EyeDirection_tangentspace;

out vec3 color;

layout(binding=0) uniform sampler2D DiffuseTextureSampler;
layout(binding=1) uniform sampler2D NormalTextureSampler;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform vec3 LightPosition_worldspace;

void main(){
    vec3 LightColor = vec3(1,1,1);
    float LightPower = 40.0;
    
    // Material properties
    vec3 MaterialDiffuseColor = texture( DiffuseTextureSampler, UV ).rgb;
    vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
    vec3 MaterialSpecularColor = vec3(0.05,0.05,0.05);

    vec3 TextureNormal_tangentspace = normalize(texture( NormalTextureSampler, vec2(UV.x,UV.y) ).rgb*2.0 - 1.0);
    
    float distance = length( LightPosition_worldspace - Position_worldspace );

    vec3 n = TextureNormal_tangentspace;
    vec3 l = normalize(LightDirection_tangentspace);
    float cosTheta = clamp( dot( n,l ), 0,1 );
    vec3 E = normalize(EyeDirection_tangentspace);
    vec3 R = reflect(-l,n);
    float cosAlpha = clamp( dot( E,R ), 0,1 );
    
    color = 
        MaterialAmbientColor +
        MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
        MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);

}
)V0G0N";
// clang-format on

struct Mesh {
    std::vector<render::BufferDescriptor> BufferDescriptors;
    core::Vector<glm::vec2> UVBuffer;
    core::Vector<glm::vec3> VertexBuffer;
    core::Vector<glm::vec3> NormalBuffer;
    core::Vector<glm::vec3> TangentBuffer;
    core::Vector<glm::vec3> BiTangentBuffer;

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
            2, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 0});

        BufferDescriptors.push_back(render::BufferDescriptor{
            3, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 1});

        BufferDescriptors.push_back(render::BufferDescriptor{
            3, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 2});

        BufferDescriptors.push_back(render::BufferDescriptor{
            3, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 3});

        BufferDescriptors.push_back(render::BufferDescriptor{
            3, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 4});

        vao = ptr->CreateBufferArrayObject(BufferDescriptors);
    }

    void UploadBuffers()
    {
        // clang-format off
        vao->GetBufferObject(0)->UpdateBuffer(IndexBuffer.size(), IndexBuffer.data());
        vao->GetBufferObject(1)->UpdateBuffer(UVBuffer.size(), UVBuffer.data());
        vao->GetBufferObject(2)->UpdateBuffer(VertexBuffer.size(), VertexBuffer.data());
        vao->GetBufferObject(3)->UpdateBuffer(NormalBuffer.size(), NormalBuffer.data());
        vao->GetBufferObject(4)->UpdateBuffer(TangentBuffer.size(), TangentBuffer.data());
        vao->GetBufferObject(5)->UpdateBuffer(BiTangentBuffer.size(), BiTangentBuffer.data());
        // clang-format on
    }

    void UploadCube()
    {
        VertexBuffer = geom::CubeVertices;
        NormalBuffer = geom::CubeNormals;
        IndexBuffer = geom::CubeIndices;
        UVBuffer = geom::CubeUV;
        geom::ComputeTangentsAndBiTangents(IndexBuffer, VertexBuffer, UVBuffer,
                                           TangentBuffer, BiTangentBuffer);
        UploadBuffers();
    }

    void Render()
    {
        vao->Render(IndexBuffer.size());
    }
};

class BaseMaterial
{
public:
    static glm::mat4 View;
    static glm::mat4 Projection;

public:
    BaseMaterial()
    {
    }

    BaseMaterial(const core::SharedPtr<render::IGpuProgram> &shader)
    {
#define LOOKUP_UNIFORM(uniform_name) \
    uniform_name = shader->GetUniform(#uniform_name)

        LOOKUP_UNIFORM(MVP);
        LOOKUP_UNIFORM(V);
        LOOKUP_UNIFORM(M);
        LOOKUP_UNIFORM(MV3x3);
        LOOKUP_UNIFORM(LightPosition_worldspace);
    }

    void Use(const glm::mat4 &model)
    {
        glm::mat4 m = model;
        glm::mat4 mvp = Projection * View * model;
        glm::mat3 mv3x3(View * model);

        MVP->SetMat4(glm::value_ptr(mvp));
        V->SetMat4(glm::value_ptr(View));
        M->SetMat4(glm::value_ptr(m));
        MV3x3->SetMat3(glm::value_ptr(mv3x3));
        LightPosition_worldspace->Set(core::pod::Vec3<float>{-8.f, 5.f, -4.f});
    }

private:
    render::IGpuProgramUniform *MVP;
    render::IGpuProgramUniform *V;
    render::IGpuProgramUniform *M;
    render::IGpuProgramUniform *MV3x3;
    render::IGpuProgramUniform *LightPosition_worldspace;
};
glm::mat4 BaseMaterial::View;
glm::mat4 BaseMaterial::Projection;

class RenderObject
{
public:
    RenderObject()
    {
    }

    RenderObject(
        render::IRenderer *renderer, const core::SharedPtr<Mesh> &mesh,
        const BaseMaterial &material,
        const core::Vector<core::SharedPtr<render::ITexture>> &textures)
        : m_renderer(renderer),
          m_mesh(mesh),
          m_material(material),
          m_textures(textures)
    {
    }

    void Render()
    {
        m_renderer->SetActiveTextures(m_textures);
        m_material.Use(Transform);
        m_mesh->Render();
    }

    glm::mat4 Transform;

private:
    render::IRenderer *m_renderer;
    core::SharedPtr<Mesh> m_mesh;
    BaseMaterial m_material;
    core::Vector<core::SharedPtr<render::ITexture>> m_textures;
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
        } else if (key == input::Keys::W) {
            BaseMaterial::View =
                glm::translate(BaseMaterial::View, glm::vec3(0, 0, +1));
        } else if (key == input::Keys::S) {
            BaseMaterial::View =
                glm::translate(BaseMaterial::View, glm::vec3(0, 0, -1));
        } else if (key == input::Keys::A) {
            BaseMaterial::View =
                glm::translate(BaseMaterial::View, glm::vec3(-1, 0, 0));
        } else if (key == input::Keys::D) {
            BaseMaterial::View =
                glm::translate(BaseMaterial::View, glm::vec3(+1, 0, 0));
        }

        return false;
    }

private:
    core::SharedPtr<Mesh> m_mesh;
    render::IWindow *m_window;
};

render::SWindowDefinition GetWindowDefinition();

auto LoadTextures(render::IRenderer *renderer)
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

    auto program = renderer->CreateProgram(quadVertSource, quadFragSource);

    if (!program) {
        elog::Log(elog::LogSource::Engine, elog::LogSeverity::Warn,
                  "Failed to load program");
        sutil::LogDebugMessagesAndFlush(dbg);
        return -1;
    }

    program->Bind();
    sutil::LogShaderUniforms(program);

    auto mesh = core::MakeShared<Mesh>(renderer);
    auto handler = WindowInputHandler::Create(window, mesh);
    window->GetInputDevice().lock()->SetInputHandler(handler);

    glm::vec3 rotation = {0, 0, 0};
    glm::vec4 colorOffset = {0, 0, 0, 0};

    // matrix setup
    BaseMaterial::View = glm::lookAt(glm::vec3(0, 0, -8),  // position
                                     glm::vec3(0, 0, 1),   // look at
                                     glm::vec3(0, 1, 0)    // up vec
                                     );

    BaseMaterial::Projection =
        glm::perspective(45.0f,             // FoV
                         1280.0f / 720.0f,  // Aspect Ratio.
                         0.1f,              // Near clipping plane.
                         100.0f             // Far clipping plane.
                         );

    BaseMaterial material(program);
    auto textures = LoadTextures(renderer);

    auto obj = RenderObject(renderer, mesh, material, textures);

    renderer->SetClearColor({125, 125, 225});
    // make stuff happen
    while (window->ShouldClose() == false) {
        renderer->Clear();

        obj.Transform = sutil::BuildRotation(rotation);
        obj.Render();

        rotation += glm::vec3(0.0025f, 0.0030f, 0.0020f);
        colorOffset += glm::vec4(0.01f, 0.015f, 0.02f, 0.f);

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
