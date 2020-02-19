#include "render/BaseMaterial.h"
#include "render/AnimatedMesh.h"
#include "render/animation/AnimationController.h"
#include "render/IGpuProgram.h"
#include "render/ITexture.h"
#include "resource_management/ResourceManagementInc.h"
#include "object/AnimatedMeshActor.h"
#include <resource_management/ResourceManager.h>

namespace res {
ResourceManager::ResourceManager(ImageLoader* imgLoader, res::GpuProgramManager* gpuProgramManager,
                                 res::mesh::AssimpImport* assimpImporter)
{
    m_imageLoader       = imgLoader;
    m_gpuProgramManager = gpuProgramManager;
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
        m_textures.emplace(std::piecewise_construct,
                          std::forward_as_tuple(path),
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

    auto shader = m_gpuProgramManager->LoadProgram(path);

    if (shader) {
        m_shaders.emplace(std::piecewise_construct,
                          std::forward_as_tuple(path),
                          std::forward_as_tuple(path, core::UniquePtr<render::IGpuProgram>(shader)));
        return core::MakeShared<material::BaseMaterial>(shader);
    }

    return nullptr;
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