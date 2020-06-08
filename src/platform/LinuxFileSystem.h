#ifndef LINUXFILESYSTEM_H
#define LINUXFILESYSTEM_H

#include "platform/IPlatformFileSystem.h"

namespace platform {
class LinuxFileSystem : public IPlatformFileSystem
{
  public:
  virtual io::Path GetExecutableDirectory();
};
} // namespace platform

#endif