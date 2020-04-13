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

    AnimationPlaybackOptions(bool loop = true, int fps = -1, int animationSlot = 0)
    {
        Loop          = loop;
        Fps           = fps;
        AnimationSlot = animationSlot;
    }
};

class AnimationPlayback
{
public:
    AnimationPlayback(){
        m_animation = nullptr;
        Done = true;

    }

    AnimationPlayback(Animation* animation, AnimationPlaybackOptions playbackOptions)
        : m_playbackOptions(playbackOptions)
        , m_animation(animation)
        , CurrentTime(0)
        , Done(false)
    {
        if (m_playbackOptions.Fps == -1) {
            m_playbackOptions.Fps = m_animation->Fps;
        }
    }

    AnimationPlayback(const AnimationPlayback& other)
        : m_animation(other.m_animation)
        , CurrentTime(other.CurrentTime)
        , m_playbackOptions(other.m_playbackOptions)
    {
        if (m_playbackOptions.Fps == -1) {
            m_playbackOptions.Fps = m_animation->Fps;
        }
    }

    void AdvanceAnimationTime(float deltaTimeInSeconds)
    {
        CurrentTime += deltaTimeInSeconds * m_playbackOptions.Fps;
        Done = m_playbackOptions.Loop == false && utils::math::gequal(GetCurrentTime(), GetDuration());
        CurrentTime = glm::mod(CurrentTime, GetDuration());
    }

    Animation* GetAnimation() const {
        return m_animation;
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
        return !m_animation || Done;
    }

    int GetPlaybackSlot(){
        return m_playbackOptions.AnimationSlot;
    }

private:
    AnimationPlaybackOptions m_playbackOptions;
    render::anim::Animation* m_animation;
    float CurrentTime;
    bool Done = false;
};

class AnimationController
{
public:
    AnimationController(render::AnimatedMesh* mesh);
    ~AnimationController()
    {
    }
    [[nodiscard]] const core::Vector<glm::mat4>& GetCurrentFrame() const;

    bool SetAnimation(int animationIndex);
    bool SetAnimation(core::String animationName, AnimationPlaybackOptions playbackOptions = render::anim::AnimationPlaybackOptions());

    [[nodiscard]] bool IsAnimationPlaying(core::String animation) const;

    void Animate(float deltaTimeInSeconds);
    glm::mat4 GetBoneTransformation(core::String name);

protected:
    [[nodiscard]] bool IsPlaying() const;

protected:
    core::Vector<glm::mat4> m_currentFrame;
    core::Vector<glm::mat4> m_boneTransformNoOffset;
    render::AnimatedMesh* m_animatedMesh;
    core::Array<AnimationPlayback, 4> m_animations;
};

} // namespace render::anim

#endif // THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ANIMATIONCONTROLLER_H_
