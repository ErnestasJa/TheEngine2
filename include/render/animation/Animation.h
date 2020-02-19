#ifndef THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ANIMATIONDATA_H_
#define THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ANIMATIONDATA_H_
#include "BoneKeyCollection.h"

namespace render::anim {
struct Animation
{
    core::String Name;
    float Fps;
    float Duration;

    BoneKeyCollection ArmatureKeys;
    core::Vector<BoneKeyCollection> BoneKeys;
};
}

#endif // THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ANIMATIONDATA_H_
