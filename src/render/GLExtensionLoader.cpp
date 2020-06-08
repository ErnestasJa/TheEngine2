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

core::UniquePtr<GLExtensionLoader> CreateExtensionLoader()
{
  return core::MakeUnique<GLExtensionLoader>();
}
} // namespace render