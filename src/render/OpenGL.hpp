#ifndef OPENGLRENDER_HPP
#define OPENGLRENDER_HPP
#include "GLXW/glxw.h"
#include "render/BufferDescriptor.h"
#include "render/CTexture.h"

namespace render {
namespace gl {
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

struct gpu_texture_handle
{
    uint32_t id;
    uint32_t type;
    uint32_t format;
    uint32_t wrap_s;
    uint32_t wrap_t;
    uint32_t filter_min;
    uint32_t filter_mag;
};

template <class T> inline bool IsHandleValid(const T& handle) DFUNC
{
    return handle.id != 0;
}

inline void BindHandle(const gpu_shader_handle& handle) DFUNC
{
    glUseProgram(handle.id);
}

inline void BindHandle(const gpu_buffer_object_handle& handle) DFUNC
{
    glBindBuffer(handle.buffer_type, handle.id);
}

inline void BindHandle(const gpu_vertex_array_object_handle& handle) DFUNC
{
    glBindVertexArray(handle.id);
}

inline void BindHandle(const gpu_texture_handle& handle) DFUNC
{
    glBindTexture(handle.type, handle.id);
}

inline void FreeHandle(const gpu_shader_handle& handle) DFUNC
{
    glDeleteProgram(handle.id);

    if (handle.vertex_program_id)
        glDeleteShader(handle.vertex_program_id);
    if (handle.fragment_program_id)
        glDeleteShader(handle.fragment_program_id);
    if (handle.geometry_program_id)
        glDeleteShader(handle.geometry_program_id);
}

inline void FreeHandle(const gpu_buffer_object_handle& handle) DFUNC
{
    glDeleteBuffers(1, &handle.id);
}

inline void FreeHandle(const gpu_vertex_array_object_handle& handle) DFUNC
{
    glDeleteVertexArrays(1, &handle.id);
}

inline void FreeHandle(const gpu_texture_handle& handle) DFUNC
{
    glDeleteTextures(1, &handle.id);
}

inline void SetUnpackAlignment(render::TextureDataFormat format) DFUNC
{
    switch (format) {
    case render::TextureDataFormat::RGB:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        break;

    case render::TextureDataFormat::RGBA:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        break;

    default:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}

inline gpu_texture_handle CreateTexture(const render::TextureDescriptor& descriptor) DFUNC
{
    auto GetType = [&]() {
        switch (descriptor.type) {
        case render::TextureType::T2D:
            return GL_TEXTURE_2D;
        case render::TextureType::T3D:
            return GL_TEXTURE_3D;
        case render::TextureType::CUBEMAP:
            return GL_TEXTURE_CUBE_MAP;
        default:
            return GL_TEXTURE_2D;
        }
    };

    auto GetFormat = [&]() {
        switch (descriptor.internalFormat) {
        case render::TextureInternalDataFormat::RGB:
            return GL_RGB;
        case render::TextureInternalDataFormat::RGBA:
            return GL_RGBA;
        default:
            return GL_RGBA;
        }
    };

    auto GetWrapMode = [](render::TextureWrapMode mode) {
        switch (mode) {
        case render::TextureWrapMode::REPEAT:
            return GL_REPEAT;
        default:
            return GL_REPEAT;
        }
    };

    auto GetMinFilterMode = [](render::TextureFilterMode mode) {
        switch (mode) {
        case render::TextureFilterMode::NEAREST:
            return GL_NEAREST;
        case render::TextureFilterMode::BILINEAR:
            return GL_LINEAR;
        case render::TextureFilterMode::TRILINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            return GL_LINEAR;
        }
    };

    auto GetMagFilterMode = [](render::TextureFilterMode mode) {
        switch (mode) {
        case render::TextureFilterMode::NEAREST:
            return GL_NEAREST;
        case render::TextureFilterMode::BILINEAR:
            return GL_LINEAR;
        case render::TextureFilterMode::TRILINEAR:
            return GL_LINEAR;
        default:
            return GL_LINEAR;
        }
    };

    gpu_texture_handle handle;
    handle.type       = GetType();
    handle.format     = GetFormat();
    handle.wrap_s     = GetWrapMode(descriptor.wrapModeS);
    handle.wrap_t     = GetWrapMode(descriptor.wrapModeT);
    handle.filter_min = GetMinFilterMode(descriptor.filterMode);
    handle.filter_mag = GetMagFilterMode(descriptor.filterMode);

    glGenTextures(1, &handle.id);
    BindHandle(handle);
    glTexParameteri(handle.type, GL_TEXTURE_WRAP_S, handle.wrap_s);
    glTexParameteri(handle.type, GL_TEXTURE_WRAP_T, handle.wrap_t);
    glTexParameteri(handle.type, GL_TEXTURE_MIN_FILTER, handle.filter_min);
    glTexParameteri(handle.type, GL_TEXTURE_MAG_FILTER, handle.filter_mag);
    glTexParameteri(handle.type, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    return handle;
}

inline void SetTextureActiveBindingSlot(uint32_t active_binding_slot) DFUNC
{
    glActiveTexture(GL_TEXTURE0 + active_binding_slot);
}

inline bool TextureShouldUseMipMaps(const gpu_texture_handle& handle) DFUNC
{
    return handle.filter_min == GL_LINEAR_MIPMAP_LINEAR;
}

inline void UploadTextureData(const gpu_texture_handle& handle,
                              const TextureDataDescriptor& descriptor) DFUNC
{
    auto GetFormat = [&]() {
        switch (descriptor.format) {
        case render::TextureDataFormat::RGB:
            return GL_RGB;
        case render::TextureDataFormat::RGBA:
            return GL_RGBA;
        default:
            return GL_RGBA;
        }
    };

    uint32_t imageFormat = GetFormat();
    glTexImage2D(handle.type, 0, handle.format, descriptor.size.x, descriptor.size.y, 0,
                 imageFormat, GL_UNSIGNED_BYTE, descriptor.data);

    if (TextureShouldUseMipMaps(handle)) {
        glGenerateMipmap(handle.type);
    }
}

inline bool CheckAndPrintShaderCompileStatus(uint32_t shader) DFUNC
{
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint maxLength     = 1024;
        char errorLog[1024] = {'\0'};
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        printf("SHADER COMPILE STATUS: %s \n", errorLog);
        return false;
    }

    return true;
}

inline bool CheckLinkStatus(const gpu_shader_handle& handle) DFUNC
{
    int32_t isLinked = 0;
    glGetProgramiv(handle.id, GL_LINK_STATUS, (int32_t*)&isLinked);

    if (isLinked == GL_FALSE) {
        GLint maxLength     = 1024;
        char errorLog[1024] = {'\0'};
        glGetProgramiv(handle.id, GL_INFO_LOG_LENGTH, &maxLength);
        glGetProgramInfoLog(handle.id, maxLength, &maxLength, &errorLog[0]);

        printf("SHADER LINK STATUS: %s \n", errorLog);
        return false;
    }

    return true;
}

inline uint32_t CreateShaderFromString(uint32_t type, const char* source) DFUNC
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

inline bool LinkProgram(const gpu_shader_handle& handle) DFUNC
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
                                       uint32_t geom_program) DFUNC
{
    uint32_t program = glCreateProgram();

    gpu_shader_handle handle{program, vert_program, frag_program, geom_program};

    if (LinkProgram(handle) == false) {
        FreeHandle(handle);
        return gpu_shader_handle{0, 0, 0, 0};
    }

    return handle;
}

inline gpu_shader_handle CreatePipelineFromShaderStrings(const char* vertSource,
                                                         const char* fragSource,
                                                         const char* geomSource) DFUNC
{
    uint32_t vs = CreateShaderFromString(GL_VERTEX_SHADER, vertSource);
    uint32_t fs = CreateShaderFromString(GL_FRAGMENT_SHADER, fragSource);
    uint32_t gs = CreateShaderFromString(GL_GEOMETRY_SHADER, geomSource);

    return CreateProgram(vs, fs, gs);
}

inline uint32_t GetUniformCount(uint32_t id) DFUNC
{
    int32_t count = 0;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);
    return count < 0 ? 0 : count;
}

inline gpu_shader_uniform_handle GetUniform(uint32_t id, uint32_t index) DFUNC
{
    int32_t name_len = -1, size = -1;
    uint32_t type = -1;
    char name[100];

    glGetActiveUniform(id, index, sizeof(name) - 1, &name_len, &size, &type, name);
    int32_t location = glGetUniformLocation(id, name);

    ASSERT(location != -1);

    return gpu_shader_uniform_handle{(uint32_t)location, id, type, name};
}

inline void SetUniform(const gpu_shader_uniform_handle& handle, float value) DFUNC
{
    glUniform1f(handle.id, value);
}

inline void SetUniform(const gpu_shader_uniform_handle& handle,
                       const core::pod::Vec2<float>& value) DFUNC
{
    glUniform2fv(handle.id, 1, &value.x);
}

inline void SetUniform(const gpu_shader_uniform_handle& handle,
                       const core::pod::Vec3<float>& value) DFUNC
{
    glUniform3fv(handle.id, 1, &value.x);
}

inline void SetUniform(const gpu_shader_uniform_handle& handle,
                       const core::pod::Vec4<float>& value) DFUNC
{
    glUniform4fv(handle.id, 1, &value.x);
}

inline void SetUniformMat4(const gpu_shader_uniform_handle& handle, float* value,
                           bool transpose = false) DFUNC
{
    glUniformMatrix4fv(handle.id, 1, transpose, value);
}

inline void SetUniformMat3(const gpu_shader_uniform_handle& handle, float* value,
                           bool transpose = false) DFUNC
{
    glUniformMatrix3fv(handle.id, 1, transpose, value);
}

inline core::Vector<gpu_buffer_object_handle> CreateGpuStorages(uint32_t count) DFUNC
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
                               void* data) DFUNC
{
    glBufferData(handle.buffer_type, buffer_size * handle.component_count * handle.component_size,
                 data, GL_STATIC_DRAW);
}

inline void EnableVertexArrayBuffer(const gpu_buffer_object_handle& handle) DFUNC
{
    if (handle.buffer_type == GL_ARRAY_BUFFER) {
        glEnableVertexAttribArray(handle.index);
        glVertexAttribPointer(handle.index, handle.component_count, handle.component_type, GL_FALSE,
                              0, 0);
    }
}

inline gpu_vertex_array_object_handle CreateVertexArrayObject() DFUNC
{
    gpu_vertex_array_object_handle handle;
    glGenVertexArrays(1, &handle.id);
    return handle;
}

inline void Render(const gpu_buffer_object_handle& handle, uint32_t count) DFUNC
{
    glDrawElements(GL_TRIANGLES, count, handle.component_type, 0);
}

inline void SetClearColor(const core::pod::Vec3<int32_t>& color) DFUNC
{
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1);
}

inline void Clear() DFUNC
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline uint32_t GetGLBufferObjectType(render::BufferObjectType type) DFUNC
{
    return type == render::BufferObjectType::index ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
}

inline void ProcessHandle(const BufferDescriptor& desc, gpu_buffer_object_handle& handle) DFUNC
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