#include "LinuxFileSystem.h"
#include <unistd.h>
#include "filesystem/Path.h"

namespace
{
const uint32_t PATH_MAX = 1024;
}

namespace platform
{
io::Path LinuxFileSystem::GetExecutableDirectory()
{
    char path[PATH_MAX];
    int32_t bytesUsed = readlink("/proc/self/exe", path, PATH_MAX);

    if (bytesUsed == -1) {
        return io::Path();
    } else {
        path[bytesUsed] = '\0';
        return io::Path(path).GetParentDirectory();
    }
}

core::UniquePtr<IPlatformFileSystem> GetPlatformFileSystem()
{
    return core::MakeUnique<LinuxFileSystem>();
}
}