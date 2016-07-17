#include "Windows.h"
#include "WindowsFileSystem.h"
#include "filesystem/Path.h"


namespace
{
const uint32_t APP_PATH_MAX = 1024;
}

namespace platform
{
io::Path WindowsFileSystem::GetExecutableDirectory()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	char path[APP_PATH_MAX];
	auto bytesUsed = GetModuleFileNameA(hModule, path, APP_PATH_MAX);

    if (bytesUsed == -1) {
        return io::Path();
    } else {
        path[bytesUsed] = '\0';
        return io::Path(path).GetParentDirectory();
    }
}

core::UniquePtr<IPlatformFileSystem> GetPlatformFileSystem()
{
    return core::MakeUnique<WindowsFileSystem>();
}
}