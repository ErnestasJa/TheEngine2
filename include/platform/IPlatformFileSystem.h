#ifndef IPLATFORMFILESYSTEM_H
#define IPLATFORMFILESYSTEM_H

namespace io {
class Path;
}

namespace platform {
class IPlatformFileSystem
{
  public:
  virtual io::Path GetExecutableDirectory() = 0;
  virtual ~IPlatformFileSystem() = default;
};

core::UniquePtr<IPlatformFileSystem> GetPlatformFileSystem();
} // namespace platform

#endif