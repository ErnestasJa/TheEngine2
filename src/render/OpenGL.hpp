#ifndef OPENGLRENDER_HPP
#define OPENGLRENDER_HPP
#include "GLXW/glxw.h"

namespace render
{
namespace gl
{
    struct gpu_shader_handle {
        uint32_t pipeline;
    };

    template <class HandleType>
    bool IsHandleValid(const HandleType& handle);
    template <class HandleType>
    void BindHandle(const HandleType& handle);
    template <class HandleType>
    void FreeHandle(const HandleType& handle);
    uint32_t CreateShaderFromString(uint32_t type, const char* source);
    uint32_t CreateProgramPipeline(uint32_t vert_program = 0,
                                   uint32_t frag_program = 0,
                                   uint32_t geom_program = 0);
    gpu_shader_handle CreatePipelineFromShaderStrings(
        const char* vertSource = nullptr, const char* fragSource = nullptr,
        const char* geomSource = nullptr);
    void SetClearColor(const core::pod::Vec3<int32_t>& color);

    /// Implementation

    inline bool IsHandleValid(const gpu_shader_handle& handle)
    {
        return handle.pipeline != 0;
    }

    inline uint32_t CreateShaderFromString(uint32_t type, const char* source)
    {
        if (source && source[0] != core::string::NullTerminator)
            return glCreateShaderProgramv(type, 1, (const char**)&source);
        else
            return 0;
    }

    inline uint32_t CreateProgramPipeline(uint32_t vert_program,
                                          uint32_t frag_program,
                                          uint32_t geom_program)
    {
        uint32_t ppo;
        glCreateProgramPipelines(1, &ppo);

        if (vert_program)
            glUseProgramStages(ppo, GL_VERTEX_SHADER_BIT, vert_program);
        if (frag_program)
            glUseProgramStages(ppo, GL_FRAGMENT_SHADER_BIT, frag_program);
        if (geom_program)
            glUseProgramStages(ppo, GL_GEOMETRY_SHADER_BIT, geom_program);

        return ppo;
    }

    inline gpu_shader_handle CreatePipelineFromShaderStrings(
        const char* vertSource, const char* fragSource, const char* geomSource)
    {
        return gpu_shader_handle{
            .pipeline = CreateProgramPipeline(
                CreateShaderFromString(GL_VERTEX_SHADER, vertSource),
                CreateShaderFromString(GL_FRAGMENT_SHADER, fragSource),
                CreateShaderFromString(GL_GEOMETRY_SHADER, geomSource))};
    }

    inline void BindHandle(const gpu_shader_handle& handle)
    {
        glBindProgramPipeline(handle.pipeline);
    }

    inline void FreeHandle(const gpu_shader_handle& handle)
    {
        glDeleteProgramPipelines(1, &handle.pipeline);
    }
    inline void SetClearColor(const core::pod::Vec3<int32_t>& color)
    {
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1);
    }

    inline void Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
}

#endif