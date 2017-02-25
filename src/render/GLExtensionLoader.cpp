#include "GLExtensionLoader.h"
#include "GLBindingInc.h"

namespace render {
GLExtensionLoader::GLExtensionLoader()
{
}

GLExtensionLoader::~GLExtensionLoader()
{
}

bool GLExtensionLoader::LoadExtensions()
{
    return gladLoadGL();
}

core::SharedPtr<GLExtensionLoader> CreateExtensionLoader()
{
    return core::MakeShared<GLExtensionLoader>();
}
}