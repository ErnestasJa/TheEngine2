#include "GLExtensionLoader.h"

#include "GLXW/glxw.h"

namespace render
{
GLExtensionLoader::GLExtensionLoader()
{
}

GLExtensionLoader::~GLExtensionLoader()
{
}

bool GLExtensionLoader::LoadExtensions()
{
    return glxwInit() == 0;
}

core::SharedPtr<GLExtensionLoader> CreateExtensionLoader()
{
    return core::MakeShared<GLExtensionLoader>();
}
}