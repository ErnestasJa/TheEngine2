#ifndef THEPROJECT2_RESOURCEMANAGER_H_
#define THEPROJECT2_RESOURCEMANAGER_H_

#include <render/ITexture.h>

namespace render {
class IGpuProgram;
}

namespace material {
class BaseMaterial;
}

namespace game::obj{
class AnimatedMeshActor;
}

namespace res {
class ImageLoader;
class GpuProgramManager;
namespace mesh {
class AssimpImport;
}


template <class TResource> struct Resource
{
    core::String Path;
    core::UniquePtr<TResource> Res;

    Resource(core::String path, core::UniquePtr<TResource> res){
        Path = path;
        Res = core::Move(res);
    }
};

class ResourceManager
{
public:
    ResourceManager(ImageLoader* imgLoader, res::GpuProgramManager* gpuProgramManager,
                    res::mesh::AssimpImport* assimpImporter);

    render::ITexture* LoadTexture(core::String path);
    core::SharedPtr<material::BaseMaterial> LoadMaterial(core::String path);
    core::UniquePtr<game::obj::AnimatedMeshActor> LoadAssimp(core::String meshName,
                                                             core::String textureName,
                                                             core::String materialName);

private:
    ImageLoader* m_imageLoader;
    core::UnorderedMap<core::String, Resource<render::ITexture>> m_textures;
    core::UnorderedMap<core::String, Resource<render::IGpuProgram>> m_shaders;
    GpuProgramManager* m_gpuProgramManager;
    res::mesh::AssimpImport* m_assimpImporter;
};
} // namespace res

#endif // THEPROJECT2_RESOURCEMANAGER_H_
