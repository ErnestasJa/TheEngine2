#include "LinuxFileSystem.h"
#include "filesystem/Path.h"
#include <unistd.h>

namespace platform {
const uint32_t FS_PATH_MAX = 1024;
io::Path LinuxFileSystem::GetExecutableDirectory()
{
  char path[FS_PATH_MAX];
  int32_t bytesUsed = readlink("/proc/self/exe", path, FS_PATH_MAX);

  if (bytesUsed == -1) {
    return io::Path();
  }
  else {
    path[bytesUsed] = '\0';
    return io::Path(path).GetParentDirectory();
  }
}

core::UniquePtr<IPlatformFileSystem> GetPlatformFileSystem()
{
  return core::MakeUnique<LinuxFileSystem>();
}
} // namespace platform
