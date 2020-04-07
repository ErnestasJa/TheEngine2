#ifndef THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_ANIMATEDMESH_CPP_BONEKEYCOLLECTION_H_
#define THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_ANIMATEDMESH_CPP_BONEKEYCOLLECTION_H_
#include <glm/ext/quaternion_float.hpp>
#include <utils/Math.h>

namespace render::anim {
template <class TValue> struct AnimKey
{
    TValue Value;
    float Time;

    TValue Interpolate(const AnimKey<TValue>& nextKey, float time) const;
};

struct BoneKeyCollection
{
    uint32_t BoneIndex;
    core::Vector<AnimKey<glm::vec3>> PositionKeys;
    core::Vector<AnimKey<glm::vec3>> ScaleKeys;
    core::Vector<AnimKey<glm::quat>> RotationKeys;

    template <class TValue>
    bool GetInterpolatedKey(float time, const core::Vector<AnimKey<TValue>>& keys,
                            TValue& out) const
    {
        int size = keys.size();

        for (int i = 0; i < size; i++) {
            if (utils::math::greater(keys[i].Time, time))
                continue;

            /// if not last keyframe
            if (i < size - 1) {
                if (utils::math::lequal(keys[i + 1].Time, time))
                    continue;

                out = keys[i].Interpolate(keys[i + 1], time);
                return true;
            }
            else {
                // last keyframe, nothing to interpolate
                out = keys[i].Value;
                return true;
            }
        }

        return false;
    }

    void GetTransform(float time, glm::vec3& pos, glm::vec3 & scale, glm::quat& rot) const
    {
        GetInterpolatedKey<glm::vec3>(time, PositionKeys, pos);
        GetInterpolatedKey<glm::vec3>(time, ScaleKeys, scale);
        GetInterpolatedKey<glm::quat>(time, RotationKeys, rot);
    }

    glm::mat4 GetTransform(float time) const
    {
        glm::vec3 pos, scale;
        glm::quat rot;

        bool hasAnyTransform = GetInterpolatedKey<glm::vec3>(time, PositionKeys, pos) &&
        GetInterpolatedKey<glm::vec3>(time, ScaleKeys, scale) &&
        GetInterpolatedKey<glm::quat>(time, RotationKeys, rot);

        if(!hasAnyTransform)
            return glm::mat4(1);

        return glm::translate(glm::mat4(1), pos) * glm::toMat4(rot) *
               glm::scale(glm::mat4(1), scale);
    }

    /*bool GetPosition(float time, glm::vec3& out) const
    {
        return GetInterpolatedKey<glm::vec3>(time, PositionKeys, out);
    }

    bool GetScale(float time, glm::vec3& out) const
    {
        return GetInterpolatedKey<glm::vec3>(time, ScaleKeys, out);
    }

    bool GetRotation(float time, glm::quat& out) const
    {
        return GetInterpolatedKey<glm::quat>(time, RotationKeys, out);
    }*/

};
}
#endif // THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_ANIMATEDMESH_CPP_BONEKEYCOLLECTION_H_
