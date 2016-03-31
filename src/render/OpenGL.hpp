#ifndef OPENGLRENDER_HPP
#define OPENGLRENDER_HPP
#include "GLXW/glxw.h"
#include "render/BufferDescriptor.h"

namespace render
{
namespace gl
{
    struct gpu_shader_handle {
        uint32_t pipeline;
    };

    struct gpu_buffer_object_handle {
        core::generic_data_access_handle data;
        uint32_t component_size;
        uint32_t component_count;
        uint32_t component_type;
        uint32_t buffer_size;
        uint32_t buffer_id;
        uint32_t buffer_type;
        uint32_t index;
    };

    struct gpu_vertex_array_object_handle {
        uint32_t vao_id;
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

    inline bool IsHandleValid(const gpu_buffer_object_handle& handle)
    {
        return handle.buffer_id != 0;
    }

    inline bool IsHandleValid(const gpu_vertex_array_object_handle& handle)
    {
        return handle.vao_id != 0;
    }

    inline void BindHandle(const gpu_shader_handle& handle)
    {
        glBindProgramPipeline(handle.pipeline);
    }

    inline void BindHandle(const gpu_buffer_object_handle& handle)
    {
        glBindBuffer(handle.buffer_type, handle.buffer_id);
    }

    inline void BindHandle(const gpu_vertex_array_object_handle& handle)
    {
        glBindVertexArray(handle.vao_id);
    }

    inline void FreeHandle(const gpu_shader_handle& handle)
    {
        glDeleteProgramPipelines(1, &handle.pipeline);
    }

    inline void FreeHandle(const gpu_buffer_object_handle& handle)
    {
        glDeleteBuffers(1, &handle.buffer_id);
    }

    inline void FreeHandle(const gpu_vertex_array_object_handle& handle)
    {
        glDeleteVertexArrays(1, &handle.vao_id);
    }

    inline uint32_t CreateShaderFromString(uint32_t type, const char* source)
    {
        printf("Shader source:\n'\n%s\n'\n", source);
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

    inline core::Vector<gpu_buffer_object_handle> CreateGpuStorages(
        uint32_t count)
    {
        auto handles = core::Vector<gpu_buffer_object_handle>();
        auto buffers = core::UniquePtr<uint32_t[]>(new uint32_t[count]);
        glGenBuffers(count, buffers.get());

        for (uint32_t i = 0; i < count; i++) {
            gpu_buffer_object_handle handle;
            handle.buffer_id = buffers[i];
            handles.push_back(handle);
        }

        return handles;
    }

    inline void UpdateBufferObject(const gpu_buffer_object_handle& handle)
    {
        glBufferData(
            handle.buffer_type,
            handle.buffer_size * handle.component_count * handle.component_size,
            (void*)handle.data.ptr, GL_STATIC_DRAW);
    }

    inline void EnableVertexArrayBuffer(const gpu_buffer_object_handle& handle)
    {
        if (handle.buffer_type == GL_ARRAY_BUFFER) {
            glEnableVertexAttribArray(handle.index);
            glVertexAttribPointer(handle.index, handle.buffer_size,
                                  handle.component_type, GL_FALSE, 0, 0);
        }
    }

    inline gpu_vertex_array_object_handle CreateVertexArrayObject()
    {
        gpu_vertex_array_object_handle handle;
        glGenVertexArrays(1, &handle.vao_id);
        return handle;
    }

    inline void Render(const gpu_buffer_object_handle& handle)
    {
        glDrawElements(GL_TRIANGLES, handle.buffer_size, handle.component_type,
                       0);
    }

    inline void SetClearColor(const core::pod::Vec3<int32_t>& color)
    {
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1);
    }

    inline void Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline uint32_t GetGLBufferObjectType(render::BufferObjectType type)
    {
        return type == render::BufferObjectType::index ? GL_ELEMENT_ARRAY_BUFFER
                                                       : GL_ARRAY_BUFFER;
    }

    inline void ProcessHandle(const BufferDescriptor& desc,
                              gl::gpu_buffer_object_handle& handle)
    {
        auto GetByteCount = [&]() {
            switch (desc.component_type) {
                case BufferComponentDataType::uint8:
                case BufferComponentDataType::int8:
                    return 1;
                case BufferComponentDataType::uint16:
                case BufferComponentDataType::int16:
                    return 2;
                case BufferComponentDataType::uint32:
                case BufferComponentDataType::int32:
                case BufferComponentDataType::float32:
                    return 4;
                default:
                    return 0;
            }
        };

        auto GetDataType = [&]() {
            switch (desc.component_type) {
                case BufferComponentDataType::int8:
                    return GL_BYTE;
                case BufferComponentDataType::int16:
                    return GL_SHORT;
                case BufferComponentDataType::int32:
                    return GL_INT;
                case BufferComponentDataType::uint8:
                    return GL_UNSIGNED_BYTE;
                case BufferComponentDataType::uint16:
                    return GL_UNSIGNED_SHORT;
                case BufferComponentDataType::uint32:
                    return GL_UNSIGNED_INT;
                case BufferComponentDataType::float32:
                    return GL_FLOAT;
                default:
                    return 0;
            }
        };

        handle.buffer_size = desc.size;
        handle.buffer_type = desc.type == BufferObjectType::index
                                 ? GL_ELEMENT_ARRAY_BUFFER
                                 : GL_ARRAY_BUFFER;
        handle.component_size = GetByteCount();
        handle.component_type = GetDataType();
        handle.component_count = desc.component_count;
        handle.data = desc.data;
        handle.index = desc.layout_location;
    }
}
}

#endif