#ifndef SWINDOWDEFINITION_H
#define SWINDOWDEFINITION_H

namespace render {
struct SWindowDefinition
{
    int32_t ContextMajorVersion;
    int32_t ContextMinorVersion;
    int32_t DepthBits;
    core::pod::Vec4<int32_t> ColorFramebufferBits;
    core::pod::Vec4<int32_t> AccumulationBufferBits;
    core::pod::Vec2<int32_t> Position;
    core::pod::Vec2<int32_t> Dimensions;
    std::string Title;

    bool ForwardCompatible;
    bool DebugContext;
    bool Resizeable;
    bool Fullscreen;

    SWindowDefinition()
        : ForwardCompatible(true), DebugContext(false), Resizeable(false), Fullscreen(false), ContextMajorVersion(3),
          ContextMinorVersion(3), DepthBits(24), ColorFramebufferBits({8, 8, 8, 8}),
          AccumulationBufferBits({0, 0, 0, 0}), Position({0, 0}), Dimensions({640, 480})
    {
    }
};
}

#endif