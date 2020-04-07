#ifndef THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ANIMATIONCONTROLLER_H_
#define THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ANIMATIONCONTROLLER_H_

#include "Animation.h"
#include "Armature.h"
#include "render/RenderFwd.h"

namespace render::anim {

struct AnimationPlaybackOptions
{
    bool Loop;
    int Fps;
    int AnimationSlot;

    AnimationPlaybackOptions(bool loop = true, int fps = -1)
    {
        Loop          = loop;
        Fps           = fps;
        AnimationSlot = 0;
    }
};

class AnimationPlayback
{
public:
    float CurrentTime;

public:
    AnimationPlayback(){
        m_animation = nullptr;
    }

    AnimationPlayback(Animation* animation, AnimationPlaybackOptions playbackOptions)
        : m_animation(animation)
        , CurrentTime(0)
        , m_playbackOptions(playbackOptions)
    {
        if (m_playbackOptions.Fps == -1) {
            m_playbackOptions.Fps = m_animation->Fps;
        }
    }

    AnimationPlayback(const AnimationPlayback& other)
        : m_animation(other.m_animation)
        , CurrentTime(other.CurrentTime)
        , m_playbackOptions(m_playbackOptions)
    {
    }

    void AdvanceAnimationTime(float deltaTimeInSeconds)
    {
        CurrentTime += deltaTimeInSeconds * m_playbackOptions.Fps;
        CurrentTime = glm::mod(CurrentTime, GetDuration());
    }

    const core::String& GetName() const
    {
        return m_animation->Name;
    }

    float GetDuration() const
    {
        return m_animation->Duration;
    }

    int GetFps() const
    {
        return m_playbackOptions.Fps;
    }

    bool WillLoop() const
    {
        return m_playbackOptions.Loop;
    }

    float GetCurrentTime() const
    {
        return CurrentTime;
    }

    bool IsFinished() const
    {
        return !m_animation || utils::math::gequal(GetCurrentTime(), GetDuration());
    }

private:
    render::anim::Animation* m_animation;
    AnimationPlaybackOptions m_playbackOptions;
};

class AnimationController
{
public:
    AnimationController(render::AnimatedMesh* mesh);
    ~AnimationController()
    {
    }
    const core::Vector<glm::mat4>& GetCurrentFrame() const;

    bool SetAnimation(int animationIndex);
    bool SetAnimation(core::String animationName, AnimationPlaybackOptions playbackOptions = render::anim::AnimationPlaybackOptions());
    void OverrideFps(float fps);
    const render::anim::Animation* GetCurrentAnimation();

    void Animate(float deltaTimeInSeconds);
    glm::mat4 GetBoneTransformation(core::String name);

protected:
    bool IsPlaying() const;

protected:
    core::Vector<glm::mat4> m_currentFrame;
    core::Vector<glm::mat4> m_boneTransformNoOffset;
    float m_animationTime;
    float m_fps;
    render::AnimatedMesh* m_animatedMesh;
    const render::anim::Animation* m_currentAnimation;
    core::Array<AnimationPlayback, 4> m_animations;
};

} // namespace render::anim

#endif // THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ANIMATIONCONTROLLER_H_
