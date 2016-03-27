#ifndef OPENGLRENDER_HPP
#define OPENGLRENDER_HPP
#include "GLXW/glxw.h"

namespace render
{
namespace gl
{
    union gpu_data_access_handle {
        uint8_t* ptr;
        int8_t* as_i8;
        uint8_t* as_u8;
        int16_t* as_i16;
        uint16_t* as_u16;
        int32_t* as_i32;
        uint32_t* as_uint32_t;
        float* as_float;
        struct {
            union {
                float x;
                float u;
            };
            union {
                float y;
                float v;
            };
        } * as_vec2;
        struct {
            union {
                float x;
                float r;
            };
            union {
                float y;
                float g;
            };
            union {
                float z;
                float b;
            };
        } * as_vec3;
        struct {
            union {
                float x;
                float r;
            };
            union {
                float y;
                float g;
            };
            union {
                float z;
                float b;
            };
            union {
                float w;
                float a;
            };
        } * as_vec4;
    };

    struct gpu_shader_handle {
        uint32_t pipeline;
    };

    struct gpu_buffer_storage_handle {
        gpu_data_access_handle data;
        uint32_t bytes;
        uint32_t count;
        uint32_t format;
        uint32_t buffer_id;
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

    inline void BindHandle(const gpu_shader_handle& handle)
    {
        glBindProgramPipeline(handle.pipeline);
    }

    inline void FreeHandle(const gpu_shader_handle& handle)
    {
        glDeleteProgramPipelines(1, &handle.pipeline);
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

    inline void CreateGpuStorage(gpu_buffer_storage_handle* handles,
                                 uint32_t count)
    {
        auto buffers = core::UniquePtr<uint32_t[]>(new uint32_t[count]);
        glGenBuffers(count, buffers.get());

        for (uint32_t i = 0; i < count; i++) {
            handles[i].buffer_id = buffers.get()[i];
        }
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