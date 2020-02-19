#include "render/animation/BoneKeyCollection.h"

namespace render::anim {
template <>
glm::vec3 AnimKey<glm::vec3>::Interpolate(const AnimKey<glm::vec3>& nextKey, float animTime) const
{
    float delta        = nextKey.Time - Time;
    float interpFactor = (animTime - Time) / delta;
    return Value + interpFactor * (nextKey.Value - Value);
}

template <>
glm::quat AnimKey<glm::quat>::Interpolate(const AnimKey<glm::quat>& nextKey, float animTime) const
{
    float delta        = nextKey.Time - Time;
    float interpFactor = (animTime - Time) / delta;
    return glm::mix(Value, nextKey.Value, interpFactor);
}
}