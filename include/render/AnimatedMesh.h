#ifndef ENGINE_ANIMATEDMESH_H
#define ENGINE_ANIMATEDMESH_H

#include "animation/Bone.h"
#include "render/animation/Animation.h"
#include "render/animation/Armature.h"
#include "render/animation/BoneKeyCollection.h"
#include <render/BaseMesh.h>
#include <render/BufferDescriptor.h>
#include <utils/Math.h>
#include "glm/glm.hpp"
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>

namespace render {
struct BufferDescriptor;
class IGpuBufferArrayObject;
class IRenderer;

class AnimatedMesh: public BaseMesh
{
public:
    core::Vector<glm::vec4> BlendIndexBuffer;
    core::Vector<glm::vec4> BlendWeightBuffer;

    AnimatedMesh();

    AnimatedMesh(core::SharedPtr<IGpuBufferArrayObject> vao);

    void Upload();

    void Render();
    void Clear();

    void SetArmature(const render::anim::Armature& armature);

    const render::anim::Armature& GetArmature(){
      return m_armature;
    }

    void AddAnimation(const render::anim::Animation & animation){
        m_animations.push_back(animation);
    }

    const core::Vector<render::anim::Animation>& GetAnimations(){
        return m_animations;
    }

protected:

    render::anim::Armature m_armature;
    core::Vector<render::anim::Animation> m_animations;
};

}

#endif // ENGINE_ANIMATEDMESH_H
