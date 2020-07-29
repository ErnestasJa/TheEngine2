#ifndef THEPROJECT2_RESOURCEMANAGER_H_
#define THEPROJECT2_RESOURCEMANAGER_H_

namespace render {
class IGpuProgram;
class ITexture;
}

namespace material {
class BaseMaterial;
}

namespace game::obj {
class AnimatedMeshActor;
}

namespace res {
class ImageLoader;

namespace mesh {
class AssimpImport;
}

template <class TResource> struct Resource
{
  core::String Path;
  core::UniquePtr<TResource> Res;

  Resource(core::String path, core::UniquePtr<TResource> res)
  {
    Path = path;
    Res  = core::Move(res);
  }

  Resource() = delete;
  Resource(const Resource&) = delete;
  Resource & operator=(const Resource &) = delete;
};

class ResourceManager
{
public:
  ResourceManager(ImageLoader* imgLoader,
                  render::IRenderer* renderer, io::IFileSystem* fileSystem,
                  res::mesh::AssimpImport* assimpImporter);

  ~ResourceManager() = default;

  render::ITexture* LoadTexture(core::String path);
  /// todo: this should return UniquePtr.
  core::SharedPtr<material::BaseMaterial> LoadMaterial(core::String path);
  core::UniquePtr<game::obj::AnimatedMeshActor> LoadAssimp(core::String meshName,
                                                           core::String textureName,
                                                           core::String materialName);

private:
    core::String LoadShaderSource(const core::String& path);
    render::IGpuProgram* LoadProgram(const core::String& path);

private:
  ImageLoader* m_imageLoader;
  core::UnorderedMap<core::String, Resource<render::ITexture>> m_textures;
  core::UnorderedMap<core::String, Resource<render::IGpuProgram>> m_shaders;
  render::IRenderer* m_renderer;
  io::IFileSystem* m_fileSystem;
  res::mesh::AssimpImport* m_assimpImporter;
};
} // namespace res

#endif // THEPROJECT2_RESOURCEMANAGER_H_
