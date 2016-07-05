#ifndef IPLATFORMFILESYSTEM_H
#define IPLATFORMFILESYSTEM_H

class IPlatformFileSystem
{
public:
    virtual core::String GetExecutableDirectory() = 0;
};

#endif