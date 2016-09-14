#ifndef SHADERS_H
#define SHADERS_H

#include "log/LogInc.h"

namespace shader {
struct ShaderSource
{
    core::String Vertex;
    core::String Fragment;
    core::String Geometry;
};

core::String ReadShader(io::IFileSystem* fs, const core::String& name)
{
    core::String fileContents;

    auto file = fs->OpenRead(name);
    if (file) {
        file->Read(fileContents);
        elog::LogInfo(fileContents);
    }
    else {
        elog::LogInfo("Failed to read shader file: " + name);
    }

    return fileContents;
}

ShaderSource LoadShaderSource(io::IFileSystem* fs, const core::String& vertShader,
                              const core::String& fragShader, const core::String& geomShader)
{
    ShaderSource source;
    source.Vertex   = ReadShader(fs, vertShader);
    source.Fragment = ReadShader(fs, fragShader);
    source.Geometry = ReadShader(fs, geomShader);
    return source;
}

ShaderSource LoadShaderSource(io::IFileSystem* fs, const core::String& shaderName)
{
    return LoadShaderSource(fs, shaderName + ".vert", shaderName + ".frag", shaderName + ".geom");
}
}

#endif