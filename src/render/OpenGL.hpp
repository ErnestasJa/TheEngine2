#ifndef OPENGLRENDER_HPP
#define OPENGLRENDER_HPP
#include "GLBindingInc.h"
#include "render/BufferDescriptor.h"
#include "render/CTexture.h"

namespace render {
namespace gl {

template <class GpuHandleType> class gpu_object
{
public:
    gpu_object(const GpuHandleType& handle)
        : m_handle(handle){

        };
    const GpuHandleType& GetHandle()
    {
        return m_handle;
    }

protected:
    GpuHandleType m_handle;
};

struct gpu_shader_handle
{
    uint32_t id;
    uint32_t vertex_program_id;
    uint32_t fragment_program_id;
    uint32_t geometry_program_id;
};

struct gpu_shader_uniform_handle
{
    uint32_t id;
    uint32_t program_id;
    uint32_t type;
    core::String name;
};

struct gpu_buffer_object_handle
{
    uint32_t id;
    uint32_t component_size;
    uint32_t component_count;
    uint32_t component_type;
    uint32_t buffer_type;
    uint32_t index;
};

struct gpu_vertex_array_object_handle
{
    uint32_t id;
};

template <class gpu_handle> inline bool IsHandleValid(const gpu_handle& handle)
{
    return handle.id != 0;
}

inline void BindHandle(const gpu_shader_handle& handle)
{
    glUseProgram(handle.id);
}

inline void BindHandle(const gpu_buffer_object_handle& handle)
{
    glBindBuffer(handle.buffer_type, handle.id);
}

inline void BindHandle(const gpu_vertex_array_object_handle& handle)
{
    glBindVertexArray(handle.id);
}

inline void FreeHandle(const gpu_shader_handle& handle)
{
    glDeleteProgram(handle.id);

    if (handle.vertex_program_id)
        glDeleteShader(handle.vertex_program_id);
    if (handle.fragment_program_id)
        glDeleteShader(handle.fragment_program_id);
    if (handle.geometry_program_id)
        glDeleteShader(handle.geometry_program_id);
}

inline void FreeHandle(const gpu_buffer_object_handle& handle)
{
    glDeleteBuffers(1, &handle.id);
}

inline void FreeHandle(const gpu_vertex_array_object_handle& handle)
{
    glDeleteVertexArrays(1, &handle.id);
}

inline bool CheckAndPrintShaderCompileStatus(uint32_t shader)
{
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint maxLength     = 1024;
        char errorLog[1024] = { '\0' };
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        printf("SHADER COMPILE STATUS: %s \n", errorLog);
        return false;
    }

    return true;
}

inline bool CheckLinkStatus(const gpu_shader_handle& handle)
{
    int32_t isLinked = 0;
    glGetProgramiv(handle.id, GL_LINK_STATUS, (int32_t*)&isLinked);

    if (isLinked == GL_FALSE) {
        GLint maxLength     = 1024;
        char errorLog[1024] = { '\0' };
        glGetProgramiv(handle.id, GL_INFO_LOG_LENGTH, &maxLength);
        glGetProgramInfoLog(handle.id, maxLength, &maxLength, &errorLog[0]);

        printf("SHADER LINK STATUS: %s \n", errorLog);
        return false;
    }

    return true;
}

inline uint32_t CreateShaderFromString(uint32_t type, const char* source)
{
    if (source && source[0] != core::string::NullTerminator) {
        uint32_t shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        if (CheckAndPrintShaderCompileStatus(shader)) {
            return shader;
        }
        else {
            glDeleteShader(shader);
        }
    }
    return 0;
}

inline bool LinkProgram(const gpu_shader_handle& handle)
{
    if (handle.vertex_program_id)
        glAttachShader(handle.id, handle.vertex_program_id);
    if (handle.fragment_program_id)
        glAttachShader(handle.id, handle.fragment_program_id);
    if (handle.geometry_program_id)
        glAttachShader(handle.id, handle.geometry_program_id);

    glLinkProgram(handle.id);

    if (CheckLinkStatus(handle) == false) {
        return false;
    }

    if (handle.vertex_program_id)
        glDetachShader(handle.id, handle.vertex_program_id);
    if (handle.fragment_program_id)
        glDetachShader(handle.id, handle.fragment_program_id);
    if (handle.geometry_program_id)
        glDetachShader(handle.id, handle.geometry_program_id);

    return true;
}

inline gpu_shader_handle CreateProgram(uint32_t vert_program, uint32_t frag_program,
                                       uint32_t geom_program)
{
    uint32_t program = glCreateProgram();

    gpu_shader_handle handle{ program, vert_program, frag_program, geom_program };

    if (LinkProgram(handle) == false) {
        FreeHandle(handle);
        return gpu_shader_handle{ 0, 0, 0, 0 };
    }

    return handle;
}

inline gpu_shader_handle CreatePipelineFromShaderStrings(const char* vertSource,
                                                         const char* fragSource,
                                                         const char* geomSource)
{
    uint32_t vs = CreateShaderFromString(GL_VERTEX_SHADER, vertSource);
    uint32_t fs = CreateShaderFromString(GL_FRAGMENT_SHADER, fragSource);
    uint32_t gs = CreateShaderFromString(GL_GEOMETRY_SHADER, geomSource);

    return CreateProgram(vs, fs, gs);
}

inline uint32_t GetUniformCount(uint32_t id)
{
    int32_t count = 0;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);
    return count < 0 ? 0 : count;
}

inline gpu_shader_uniform_handle GetUniform(uint32_t id, uint32_t index)
{
    int32_t name_len = -1, size = -1;
    uint32_t type = -1;
    char name[100];

    glGetActiveUniform(id, index, sizeof(name) - 1, &name_len, &size, &type, name);
    int32_t location = glGetUniformLocation(id, name);

    ASSERT(location != -1);

    return gpu_shader_uniform_handle{ (uint32_t)location, id, type, name };
}

inline void SetUniform(const gpu_shader_uniform_handle& handle, int value)
{
    glUniform1i(handle.id, value);
}

inline void SetUniform(const gpu_shader_uniform_handle& handle, float value)
{
    glUniform1f(handle.id, value);
}

inline void SetUniform(const gpu_shader_uniform_handle& handle, const core::pod::Vec2<float>& value)
{
    glUniform2fv(handle.id, 1, &value.x);
}

inline void SetUniform(const gpu_shader_uniform_handle& handle, const core::pod::Vec3<float>& value)
{
    glUniform3fv(handle.id, 1, &value.x);
}

inline void SetUniform(const gpu_shader_uniform_handle& handle, const int vecSize, float * vec)
{
    switch(vecSize) {
    case 1:
        glUniform1fv(handle.id, 1, vec);
        break;
    case 2:
        glUniform2fv(handle.id, 1, vec);
        break;
    case 3:
        glUniform3fv(handle.id, 1, vec);
        break;
    case 4:
        glUniform4fv(handle.id, 1, vec);
        break;
    }
}

inline void SetUniform(const gpu_shader_uniform_handle& handle, const core::pod::Vec4<float>& value)
{
    glUniform4fv(handle.id, 1, &value.x);
}

inline void SetUniformMat4(const gpu_shader_uniform_handle& handle, float* value,
                           bool transpose = false)
{
    glUniformMatrix4fv(handle.id, 1, transpose, value);
}

inline void SetUniformMat3(const gpu_shader_uniform_handle& handle, float* value,
                           bool transpose = false)
{
    glUniformMatrix3fv(handle.id, 1, transpose, value);
}

inline void SetUniformMat3x4(const gpu_shader_uniform_handle& handle, float* value, int count,
                           bool transpose = false)
{
    glUniformMatrix3x4fv(handle.id, count, transpose, value);
}

inline void SetUniformMat4(const gpu_shader_uniform_handle& handle, float* value, int count,
                             bool transpose = false)
{
    glUniformMatrix4fv(handle.id, count, transpose, value);
}

inline core::Vector<gpu_buffer_object_handle> CreateGpuStorages(uint32_t count)
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

inline void UpdateBufferObject(const gpu_buffer_object_handle& handle, uint32_t buffer_size,
                               void* data)
{
    glBufferData(handle.buffer_type, buffer_size * handle.component_count * handle.component_size,
                 data, GL_STATIC_DRAW);
}

inline void EnableVertexArrayBuffer(const gpu_buffer_object_handle& handle)
{
    if (handle.buffer_type == GL_ARRAY_BUFFER) {
        glEnableVertexAttribArray(handle.index);
        (glVertexAttribPointer)(handle.index, handle.component_count, handle.component_type, GL_FALSE,
                              0, 0);
    }
}

inline gpu_vertex_array_object_handle CreateVertexArrayObject()
{
    gpu_vertex_array_object_handle handle;
    glGenVertexArrays(1, &handle.id);
    return handle;
}

inline void Render(const gpu_buffer_object_handle& handle, uint32_t count)
{
    glDrawElements(GL_TRIANGLES, count, handle.component_type, 0);
}

inline void RenderLines(const gpu_buffer_object_handle& handle, uint32_t count)
{
    glDrawElements(GL_LINES, count, handle.component_type, 0);
}

inline void SetClearColor(const core::pod::Vec3<int32_t>& color)
{
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1);
}

inline void Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void SetDepthTest(bool enabled){
    if(enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

inline uint32_t GetGLBufferObjectType(render::BufferObjectType type)
{
    return type == render::BufferObjectType::index ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
}

inline void ProcessHandle(const BufferDescriptor& desc, gpu_buffer_object_handle& handle)
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

    handle.buffer_type =
        desc.type == BufferObjectType::index ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
    handle.component_size  = GetByteCount();
    handle.component_type  = GetDataType();
    handle.component_count = desc.component_count;
    handle.index           = desc.layout_location;
}
}
}

#endif
