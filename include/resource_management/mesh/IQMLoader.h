#ifndef IQMLOADER_H
#define IQMLOADER_H

namespace render {
class AnimatedMesh;
}

namespace io {
class IFileSystem;
class Path;
} // namespace io

namespace res {
class IQMLoader
{
  public:
  IQMLoader(io::IFileSystem* fileSystem);

  virtual void Load(render::AnimatedMesh* mesh, io::Path path);

  private:
  io::IFileSystem* m_fileSystem;
};
} // namespace res

#endif // IQMLOADER_H