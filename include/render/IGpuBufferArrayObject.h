#ifndef IGPUBUFFERARRAYOBJECT
#define IGPUBUFFERARRAYOBJECT

namespace render
{
class IGpuBufferObject;
struct BufferDescriptor;
class IGpuBufferArrayObject
{
public:
    virtual ~IGpuBufferArrayObject()
    {
    }
    virtual void Bind() = 0;
    virtual IGpuBufferObject* GetBufferObject(uint32_t index) = 0;
    virtual uint32_t GetBufferObjectCount() = 0;
};
}

#endif