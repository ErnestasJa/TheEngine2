#ifndef GLEXTENSIONLOADER_H
#define GLEXTENSIONLOADER_H

#include "render/IRenderExtensionLoader.h"

namespace render
{
class GLExtensionLoader : public IRenderExtensionLoader
{
public:
    GLExtensionLoader();
    virtual ~GLExtensionLoader();
    virtual bool LoadExtensions();
};
}

#endif