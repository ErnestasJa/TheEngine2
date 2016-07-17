#ifndef WINDOWSFILESYSTEM_H
#define WINDOWSFILESYSTEM_H

#include "platform/IPlatformFileSystem.h"

namespace platform
{
class WindowsFileSystem : public IPlatformFileSystem
{
public:
    virtual io::Path GetExecutableDirectory();
};
}

#endif