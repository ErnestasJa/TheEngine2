#ifndef GLEXTENSIONLOADER_H
#define GLEXTENSIONLOADER_H

namespace render {
class GLExtensionLoader
{
  public:
  GLExtensionLoader();
  virtual ~GLExtensionLoader();
  virtual bool LoadExtensions();
};
core::UniquePtr<GLExtensionLoader> CreateExtensionLoader();
} // namespace render
#endif