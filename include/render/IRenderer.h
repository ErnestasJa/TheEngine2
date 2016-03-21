#ifndef IRENDERER_H
#define IRENDERER_H

namespace render
{
class IProgram;
class IRenderer
{
public:
    virtual ~IRenderer()
    {
    }
    virtual core::SharedPtr<IProgram> CreateProgram(
        const core::String& vertSource = "",
        const core::String& fragSource = "",
        const core::String& geomSource = "") = 0;
};

core::SharedPtr<IRenderer> CreateRenderer();
}

#endif