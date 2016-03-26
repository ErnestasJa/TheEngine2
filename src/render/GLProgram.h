#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "render/IProgram.h"
#include "OpenGL.hpp"

namespace render
{
class GLProgram : public IProgram
{
public:
    GLProgram(const gl::gpu_shader_handle& handle);
    virtual ~GLProgram();

    virtual void Bind();

private:
    gl::gpu_shader_handle m_handle;
};
}

#endif