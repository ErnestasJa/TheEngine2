#ifndef THEPROJECT2_ASSIMPIMPORT_H
#define THEPROJECT2_ASSIMPIMPORT_H
#include "render/RenderFwd.h"
#include <render/IRenderer.h>

namespace res::mesh {
class AssimpImport
{
  public:
  AssimpImport(io::IFileSystem* fs, render::IRenderer* renderer);
  core::UniquePtr<render::AnimatedMesh> LoadMesh(io::Path path);

  private:
  io::IFileSystem* m_fileSystem;
  render::IRenderer* m_renderer;
};
} // namespace res::mesh


#endif // THEPROJECT2_ASSIMPIMPORT_H
