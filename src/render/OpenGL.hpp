#ifndef OPENGLRENDER_HPP
#define OPENGLRENDER_HPP
#include "GLXW/glxw.h"
#include "render/BufferDescriptor.h"

namespace render
{
namespace gl
{
    struct gpu_shader_handle {
        uint32_t id;
        uint32_t vertex_program_id;
        uint32_t fragment_program_id;
        uint32_t geometry_program_id;
    };

    struct gpu_shader_uniform_handle {
        uint32_t id;
        uint32_t program_id;
        uint32_t type;
        core::String name;
    };

    struct gpu_buffer_object_handle {
        uint32_t id;
        uint32_t component_size;
        uint32_t component_count;
        uint32_t component_type;
        uint32_t buffer_type;
        uint32_t index;
    };

    struct gpu_vertex_array_object_handle {
        uint32_t vao_id;
    };

    inline bool IsHandleValid(const gpu_shader_handle& handle)
    {
        return handle.id != 0;
    }

    inline bool IsHandleValid(const gpu_buffer_object_handle& handle)
    {
        return handle.id != 0;
    }

    inline bool IsHandleValid(const gpu_vertex_array_object_handle& handle)
    {
        return handle.vao_id != 0;
    }

    inline void BindHandle(const gpu_shader_handle& handle)
    {
        glBindProgramPipeline(handle.id);
    }

    inline void BindHandle(const gpu_buffer_object_handle& handle)
    {
        glBindBuffer(handle.buffer_type, handle.id);
    }

    inline void BindHandle(const gpu_vertex_array_object_handle& handle)
    {
        glBindVertexArray(handle.vao_id);
    }

    inline void FreeHandle(const gpu_shader_handle& handle)
    {
        glDeleteProgramPipelines(1, &handle.id);
    }

    inline void FreeHandle(const gpu_buffer_object_handle& handle)
    {
        glDeleteBuffers(1, &handle.id);
    }

    inline void FreeHandle(const gpu_vertex_array_object_handle& handle)
    {
        glDeleteVertexArrays(1, &handle.vao_id);
    }

    inline uint32_t CreateShaderFromString(uint32_t type, const char* source)
    {
        if (source && source[0] != core::string::NullTerminator)
            return glCreateShaderProgramv(type, 1, &source);
        else
            return 0;
    }

    inline uint32_t CreateProgramPipeline(uint32_t vert_program,
                                          uint32_t frag_program,
                                          uint32_t geom_program)
    {
        uint32_t ppo;
        glCreateProgramPipelines(1, &ppo);
        glBindProgramPipeline(ppo);

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
        uint32_t vs = CreateShaderFromString(GL_VERTEX_SHADER, vertSource);
        uint32_t fs = CreateShaderFromString(GL_FRAGMENT_SHADER, fragSource);
        uint32_t gs = CreateShaderFromString(GL_GEOMETRY_SHADER, geomSource);

        return gpu_shader_handle{CreateProgramPipeline(vs, fs, gs),
                                 vs,
                                 fs,
                                 gs};
    }

    inline bool IsProgramPipelineLinked(const gpu_shader_handle& handle)
    {
        int32_t isLinked = 0;
        glGetProgramiv(handle.id, GL_LINK_STATUS, &isLinked);
        return isLinked != 0;
    }

    inline uint32_t GetUniformCount(uint32_t id)
    {
        int32_t count = 0;
        glGetProgramInterfaceiv(id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &count);
        return count < 0 ? 0 : count;
    }

    inline gpu_shader_uniform_handle GetUniform(uint32_t id, uint32_t index)
    {
        const uint32_t properties[4] = {GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH,
                                        GL_LOCATION};

        char name[100];
        int32_t values[4];
        glGetProgramResourceiv(id, GL_UNIFORM, index, 4, properties, 4, NULL,
                               values);

        glGetProgramResourceName(id, GL_UNIFORM, index, 100, NULL, name);

        return gpu_shader_uniform_handle{(uint32_t)values[3],
                                         id,
                                         (uint32_t)values[1],
                                         name};
    }

    inline void SetUniform(const gpu_shader_uniform_handle& handle, float value)
    {
        glProgramUniform1f(handle.program_id, handle.id, value);
    }

    inline void SetUniform(const gpu_shader_uniform_handle& handle,
                           const core::pod::Vec2<float>& value)
    {
        glProgramUniform2fv(handle.program_id, handle.id, 1, &value.x);
    }

    inline void SetUniform(const gpu_shader_uniform_handle& handle,
                           const core::pod::Vec3<float>& value)
    {
        glProgramUniform3fv(handle.program_id, handle.id, 1, &value.x);
    }

    inline void SetUniform(const gpu_shader_uniform_handle& handle,
                           const core::pod::Vec4<float>& value)
    {
        glProgramUniform4fv(handle.program_id, handle.id, 1, &value.x);
    }

    inline void SetUniformMat4(const gpu_shader_uniform_handle& handle,
                               float* value, bool transpose = false)
    {
        glProgramUniformMatrix4fv(handle.program_id, handle.id, 1, transpose,
                                  value);
    }

    inline core::Vector<gpu_buffer_object_handle> CreateGpuStorages(
        uint32_t count)
    {
        auto handles = core::Vector<gpu_buffer_object_handle>();
        auto buffers = core::UniquePtr<uint32_t[]>(new uint32_t[count]);
        glGenBuffers(count, buffers.get());

        for (uint32_t i = 0; i < count; i++) {
            gpu_buffer_object_handle handle;
            handle.id = buffers[i];
            handles.push_back(handle);
        }

        return handles;
    }

    inline void UpdateBufferObject(const gpu_buffer_object_handle& handle,
                                   uint32_t buffer_size, void* data)
    {
        glBufferData(handle.buffer_type, buffer_size * handle.component_count *
                                             handle.component_size,
                     data, GL_STATIC_DRAW);
    }

    inline void EnableVertexArrayBuffer(const gpu_buffer_object_handle& handle)
    {
        if (handle.buffer_type == GL_ARRAY_BUFFER) {
            glEnableVertexAttribArray(handle.index);
            glVertexAttribPointer(handle.index, handle.component_count,
                                  handle.component_type, GL_FALSE, 0, 0);
        }
    }

    inline gpu_vertex_array_object_handle CreateVertexArrayObject()
    {
        gpu_vertex_array_object_handle handle;
        glGenVertexArrays(1, &handle.vao_id);
        return handle;
    }

    inline void Render(const gpu_buffer_object_handle& handle, uint32_t count)
    {
        glDrawElements(GL_TRIANGLES, count, handle.component_type, 0);
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
                              gpu_buffer_object_handle& handle)
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

        handle.buffer_type = desc.type == BufferObjectType::index
                                 ? GL_ELEMENT_ARRAY_BUFFER
                                 : GL_ARRAY_BUFFER;
        handle.component_size = GetByteCount();
        handle.component_type = GetDataType();
        handle.component_count = desc.component_count;
        handle.index = desc.layout_location;
    }
}
}

#endif