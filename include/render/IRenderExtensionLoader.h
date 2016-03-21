#ifndef IRENDEREXTENSIONLOADER_H
#define IRENDEREXTENSIONLOADER_H

namespace render
{
class IRenderExtensionLoader
{
public:
    virtual ~IRenderExtensionLoader()
    {
    }

    virtual bool LoadExtensions() = 0;
};

core::SharedPtr<IRenderExtensionLoader> CreateExtensionLoader();
}

#endif