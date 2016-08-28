#ifndef IGPUBUFFEROBJECT_H
#define IGPUBUFFEROBJECT_H

#include "BufferDescriptor.h"

namespace render {
class IGpuBufferObject
{
public:
    virtual ~IGpuBufferObject()
    {
    }

    virtual void Bind() = 0;
    virtual void UpdateBuffer(uint32_t count, void* data) = 0;
};
}

#endif