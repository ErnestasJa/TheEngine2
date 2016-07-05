#ifndef WINFILESYSTEM_H
#define WINFILESYSTEM_H

#include "platform/IPlatformFileSystem.h"

namespace platform
{
class LinuxFileSystem : public IPlatformFileSystem
{
public:
    virtual io::Path GetExecutableDirectory();
};
}

#endif