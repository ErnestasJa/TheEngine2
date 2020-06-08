#ifndef IGPUBUFFERARRAYOBJECT
#define IGPUBUFFERARRAYOBJECT

namespace render {
class IGpuBufferObject;
struct BufferDescriptor;
class IGpuBufferArrayObject
{
  public:
  virtual ~IGpuBufferArrayObject()
  {
  }
  virtual void Bind()                                                         = 0;
  virtual const core::Vector<core::UniquePtr<IGpuBufferObject>>& GetBuffers() = 0;
  virtual IGpuBufferObject* GetBufferObject(uint32_t index)                   = 0;
  virtual uint32_t GetBufferObjectCount()                                     = 0;
  virtual void Render(uint32_t count)                                         = 0;
  virtual void RenderLines(uint32_t count)                                    = 0;
};
} // namespace render

#endif
