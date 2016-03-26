#ifndef IRENDERER_H
#define IRENDERER_H

namespace render
{
using Vec3i = core::pod::Vec3<int32_t>;
using Vec4i = core::pod::Vec4<int32_t>;

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
    virtual void SetClearColor(const Vec3i& color) = 0;
    virtual void Clear() = 0;
};

core::SharedPtr<IRenderer> CreateRenderer();
}

#endif