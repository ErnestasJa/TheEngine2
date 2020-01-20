#ifndef ENGINE_ANIMATEDMESH_H
#define ENGINE_ANIMATEDMESH_H

#include "glm/glm.hpp"
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>
#include <render/BufferDescriptor.h>
#include <utils/Math.h>

namespace render {
struct BufferDescriptor;
class IGpuBufferArrayObject;
class IRenderer;

struct Bone
{
    int32_t parent;
    core::String name;
    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;
    glm::mat4 offset;
};

template <class TValue>
struct AnimKey{
    TValue Value;
    float Time;

    TValue Interpolate(const AnimKey<TValue> & nextKey, float time) const;
};

struct BoneKeyCollection {
    uint32_t BoneIndex;
    core::Vector<AnimKey<glm::vec3>> PositionKeys;
    core::Vector<AnimKey<glm::quat>> RotationKeys;

    template <class TValue>
    bool GetInterpolatedKey(float time, const core::Vector<AnimKey<TValue>>& keys,  TValue& out){
        int size = keys.size();

        for(int i = 0; i < size; i++) {
            if (utils::math::greater(keys[i].Time, time))
                continue;

            ///if not last keyframe
            if (i < size - 1) {
                if (utils::math::lequal(keys[i + 1].Time, time))
                    continue;

                out = keys[i].Interpolate(keys[i + 1], time);
                return true;
            } else {
                // last keyframe, nothing to interpolate
                out = keys[i].Value;
                return true;
            }
        }

        return false;
    }

    bool GetPosition(float time, glm::vec3& out){
        return GetInterpolatedKey<glm::vec3>(time, PositionKeys, out);
    }

    bool GetRotation(float time, glm::quat& out){
        return GetInterpolatedKey<glm::quat>(time, RotationKeys, out);
    }
};


struct AnimationInfo
{
    core::String name;
    float fps;
    float duration;

    core::Vector<BoneKeyCollection> BoneKeys;
};


struct AnimationData
{
    core::Vector<glm::mat4>           current_frame;
    core::Vector<Bone>       bones;
    core::Vector<AnimationInfo>  animations;
    AnimationInfo* current_animation;

    AnimationData(){
        current_animation = nullptr;
    }

    bool SetAnimation(core::String name){
        for(int i = 0; i < animations.size(); i++){
            if(animations[i].name == name){
                current_animation = &animations[i];
                elog::LogInfo(core::string::format("Successfully set animation: {}", name.c_str()));
                return true;
            }
        }

        elog::LogInfo(core::string::format("Failed to set animation: {}", name.c_str()));
        current_animation = nullptr;
        return false;
    }

    bool SetAnimation(int index){
        if(index < animations.size()){
            current_animation = &animations[index];
            return true;
        }

        current_animation = nullptr;
        return false;
    }

    void Animate(float time){
        if(!current_animation){
            return;
        }

        core::Stack<int> boneIndexStack;

        for(int i = 0; i < bones.size(); i++ ){
            if(bones[i].parent < 0){
                boneIndexStack.push(i);
            }
        }

        while(!boneIndexStack.empty()){
            int index = boneIndexStack.top();
            boneIndexStack.pop();

            auto & boneData = current_animation->BoneKeys[index];
            auto & bone = bones[index];

            glm::vec3 pos;
            glm::quat rot;
            boneData.GetPosition(time,pos);
            boneData.GetRotation(time, rot);


            if(bones[index].parent < 0) {
                current_frame[index] =
                          glm::translate(glm::mat4(1), bone.pos + pos) * glm::toMat4(bone.rot * rot );
            }
            else {
                current_frame[index] = current_frame[bones[index].parent] *
                        (glm::translate(glm::mat4(1), bone.pos + pos) * glm::toMat4(bone.rot * rot));
            }

            /*if(bones[index].parent < 0) {
                current_frame[index] = glm::toMat4(rot ) *
                                       glm::translate(glm::mat4(1), pos);
            }
            else {
                current_frame[index] = current_frame[bones[index].parent] * (glm::toMat4(rot) *
                                                                             glm::translate(glm::mat4(1), pos));
            }*/

            for(int i = 0; i < bones.size(); i++){
                if(bones[i].parent == index){
                    boneIndexStack.push(i);
                }
            }
        }
    }
};

class AnimatedMesh
{
public:
    core::Vector<uint32_t> IndexBuffer;
    core::Vector<glm::vec2> UVBuffer;
    core::Vector<glm::vec3> VertexBuffer;
    core::Vector<glm::vec3> NormalBuffer;
    core::Vector<glm::vec4> BlendIndexBuffer;
    core::Vector<glm::vec4> BlendWeightBuffer;

    AnimatedMesh();

    AnimatedMesh(core::SharedPtr<IGpuBufferArrayObject> vao);

    void Upload();

    void Render();
    void Clear();

    AnimationData & GetAnimationData();

protected:
    AnimationData m_animation;
    core::SharedPtr<IGpuBufferArrayObject> m_vao;
};

}

#endif // ENGINE_ANIMATEDMESH_H
