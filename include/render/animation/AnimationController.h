#ifndef THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ANIMATIONCONTROLLER_H_
#define THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ANIMATIONCONTROLLER_H_

#include "Armature.h"
#include "Animation.h"
#include "render/RenderFwd.h"

namespace render::anim {

class AnimationController
{
public:
    AnimationController(render::AnimatedMesh * mesh);
    ~AnimationController(){}
    const core::Vector<glm::mat4> & GetCurrentFrame() const;
    bool SetAnimation(int animationIndex);
    bool SetAnimation(core::String animationName);
    void OverrideFps(float fps);
    const render::anim::Animation* GetCurrentAnimation();

    void Animate(float deltaTimeInSeconds);
    glm::mat4 GetBoneTransformation(core::String name);

protected:
    core::Vector<glm::mat4> m_currentFrame;
    core::Vector<glm::mat4> m_boneTransformNoOffset;
    float m_animationTime;
    float m_fps;
    render::AnimatedMesh* m_animatedMesh;
    const render::anim::Animation* m_currentAnimation;
};

}

#endif // THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ANIMATIONCONTROLLER_H_
