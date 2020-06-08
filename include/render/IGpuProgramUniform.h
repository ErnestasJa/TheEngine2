#ifndef IGPUPROGRAMUNIFORM_H
#define IGPUPROGRAMUNIFORM_H

namespace render {
class IGpuProgramUniform
{
  public:
  virtual const core::String& GetName()                                             = 0;
  virtual const void Set(int value)                                                 = 0;
  virtual const void Set(const int size, float* vec)                                = 0;
  virtual const void Set(float value)                                               = 0;
  virtual const void Set(const core::pod::Vec2<float>& value)                       = 0;
  virtual const void Set(const core::pod::Vec3<float>& value)                       = 0;
  virtual const void Set(const core::pod::Vec4<float>& value)                       = 0;
  virtual const void SetMat4(float* value)                                          = 0;
  virtual const void SetMat3(float* value)                                          = 0;
  virtual const void SetMat3x4(float* value, int count)                             = 0;
  virtual const void SetMat4(const float* value, int count, bool transpose = false) = 0;
};
} // namespace render

#endif