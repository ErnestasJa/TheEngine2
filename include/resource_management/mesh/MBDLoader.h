#ifndef THEPROJECT2_MBDLOADER_H
#define THEPROJECT2_MBDLOADER_H

#include "MBDBone.h"

namespace res::mbd {

class MBDLoader
{
  public:
  core::Vector<Bone> LoadMBD(io::IFileSystem* fs, io::Path path);
};
} // namespace res::mbd

#endif // THEPROJECT2_MBDLOADER_H
