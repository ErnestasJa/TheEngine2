#include "object/AnimatedMeshActor.h"
#include "render/AnimatedMesh.h"
#include "render/BaseMaterial.h"
#include "render/IGpuProgram.h"
#include "render/ITexture.h"
#include "render/animation/AnimationController.h"
#include "resource_management/ResourceManagementInc.h"
#include <resource_management/ResourceManager.h>

namespace res {
ResourceManager::ResourceManager(ImageLoader* imgLoader,
                                 render::IRenderer* renderer, io::IFileSystem* fileSystem,
                                 res::mesh::AssimpImport* assimpImporter)
{
  m_imageLoader       = imgLoader;
  m_renderer = renderer;
  m_fileSystem = fileSystem;
  m_assimpImporter    = assimpImporter;
}

render::ITexture* ResourceManager::LoadTexture(core::String path)
{
  if (auto it = m_textures.find(path); it != m_textures.end()) {
    return it->second.Res.get();
  }

  auto texture = m_imageLoader->LoadTexture(path);

  if (texture) {
    auto r = texture.get();
    m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(path),
                       std::forward_as_tuple(path, core::Move(texture)));
    return r;
  }

  return nullptr;
}

core::SharedPtr<material::BaseMaterial> ResourceManager::LoadMaterial(core::String path)
{
  if (auto it = m_shaders.find(path); it != m_shaders.end()) {
    return core::MakeShared<material::BaseMaterial>(it->second.Res.get());
  }

  auto shader = LoadProgram(path);

  if (shader) {
    return core::MakeShared<material::BaseMaterial>(shader);
  }

  return nullptr;
}

render::IGpuProgram* ResourceManager::LoadProgram(const core::String& path)
{
    if (auto it = m_shaders.find(path); it != m_shaders.end()) {
        return it->second.Res.get();
    }

    auto vertexSource   = LoadShaderSource(path + ".vert");
    auto fragmentSource = LoadShaderSource(path + ".frag");
    auto geometrySource = LoadShaderSource(path + ".geom");

    auto gpuProgram = m_renderer->CreateProgram(vertexSource, fragmentSource, geometrySource);

    if (gpuProgram) {
        auto gpuProgramPtr = gpuProgram.get();

        m_shaders.emplace(std::piecewise_construct, std::forward_as_tuple(path),
                          std::forward_as_tuple(path, core::Move(gpuProgram)));

        return gpuProgramPtr;
    }

    return nullptr;
}

core::String ResourceManager::LoadShaderSource(const core::String& path)
{
    core::String fileContents;

    auto file = m_fileSystem->OpenRead(path);
    if (file) {
        file->Read(fileContents);
        // elog::LogInfo(fileContents);
        elog::LogInfo("Loaded shader: " + path);
    }
    else {
        elog::LogInfo("Failed to read shader source: " + path);
    }

    return fileContents;
}


core::UniquePtr<game::obj::AnimatedMeshActor> ResourceManager::LoadAssimp(core::String meshName,
                                                                          core::String textureName,
                                                                          core::String materialName)
{
  auto texture = LoadTexture("resources/textures/" + textureName);

  if (!texture) {
    elog::LogError(core::string::format("Could not load texture: {}", textureName.c_str()));
    return nullptr;
  }

  auto material = LoadMaterial("resources/shaders/" + materialName);

  if (!material) {
    elog::LogError(core::string::format("Could not load material: {}", materialName.c_str()));
    return nullptr;
  }

  material->SetTexture(0, texture);

  auto mesh = m_assimpImporter->LoadMesh("resources/models/" + meshName);

  if (!mesh) {
    elog::LogError(core::string::format("Could not load mesh: {}", meshName.c_str()));
    return nullptr;
  }

  auto actor = core::MakeUnique<game::obj::AnimatedMeshActor>(meshName, std::move(mesh));
  actor->SetMaterial(material);
  return actor;
}

} // namespace res