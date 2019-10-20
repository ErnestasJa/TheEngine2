namespace render {
class AnimatedMesh;
}

namespace io {
class IFileSystem;
class Path;
}

namespace res {
class IQMLoader
{
public:
    IQMLoader(core::SharedPtr<io::IFileSystem> fileSystem);

    virtual void Load(render::AnimatedMesh * mesh, io::Path path);
private:
    core::SharedPtr<io::IFileSystem> m_fileSystem;
};
}