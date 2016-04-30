#ifndef IGPUPROGRAMUNIFORM_H
#define IGPUPROGRAMUNIFORM_H

namespace render
{
class IGpuProgramUniform
{
public:
    virtual const core::String& GetName() = 0;
    virtual const void Set(float value) = 0;
    virtual const void Set(const core::pod::Vec2<float>& value) = 0;
    virtual const void Set(const core::pod::Vec3<float>& value) = 0;
    virtual const void Set(const core::pod::Vec4<float>& value) = 0;
    virtual const void SetMat4(float* value) = 0;
    virtual const void SetMat3(float* value) = 0;
};
}

#endif