#ifndef IGPUBUFFEROBJECT_H
#define IGPUBUFFEROBJECT_H

#include "BufferDescriptor.h"

namespace render
{
class IGpuBufferObject
{
public:
    virtual ~IGpuBufferObject()
    {
    }

    virtual void Bind() = 0;
    virtual void UpdateBuffer() = 0;
};
}

#endif