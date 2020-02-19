#ifndef GLGPUSHADERPROGRAMUNIFORM_H
#define GLGPUSHADERPROGRAMUNIFORM_H

#include "OpenGL.hpp"
#include "render/IGpuProgramUniform.h"

namespace render {
class GLGpuShaderProgramUniform : public IGpuProgramUniform
{
public:
    GLGpuShaderProgramUniform(const gl::gpu_shader_uniform_handle& handle);
    virtual ~GLGpuShaderProgramUniform();

    virtual const core::String& GetName();
    virtual const void Set(int value);
    virtual const void Set(const int size, float* vec);
    virtual const void Set(float value);
    virtual const void Set(const core::pod::Vec2<float>& value);
    virtual const void Set(const core::pod::Vec3<float>& value);
    virtual const void Set(const core::pod::Vec4<float>& value);
    virtual const void SetMat4(float* value);
    virtual const void SetMat4(const float* value, int count, bool transpose = false);
    virtual const void SetMat3(float* value);
    virtual const void SetMat3x4(float* value, int count);

private:
    gl::gpu_shader_uniform_handle m_handle;
};
}

#endif