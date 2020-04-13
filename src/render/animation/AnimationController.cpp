#include "render/animation/AnimationController.h"
#include "render/AnimatedMesh.h"

namespace render::anim {

AnimationController::AnimationController(render::AnimatedMesh* mesh)
:m_animatedMesh(mesh){
    m_currentFrame.resize(m_animatedMesh->GetArmature().GetBones().size(), glm::mat4(1));
    m_boneTransformNoOffset.resize(m_animatedMesh->GetArmature().GetBones().size(), glm::mat4(1));
}

bool AnimationController::SetAnimation(int animationIndex)
{
    /*auto& animations = m_animatedMesh->GetAnimations();

    if(animationIndex < 0){
      m_animationTime = 0;
      m_currentAnimation = nullptr;
      elog::LogInfo("Pausing animation");
    }

    if(animationIndex < animations.size()){
        m_currentAnimation = &animations[animationIndex];
        m_fps = m_currentAnimation->Fps;
        m_animationTime = 0;
        elog::LogInfo(core::string::format("Successfully set animation[{}]: {}", animationIndex, m_currentAnimation->Name.c_str()));
        return true;
    }

    elog::LogInfo(core::string::format("Failed to set animation, requested anim index: {}", animationIndex));
    m_currentAnimation = nullptr;*/
    return false;
}

bool AnimationController::SetAnimation(core::String animationName, AnimationPlaybackOptions playbackOptions)
{
    auto& animations = m_animatedMesh->GetAnimations();

    for(int i = 0; i < animations.size(); i++){
        if(animations[i].Name == animationName){
            auto animationPlayback = AnimationPlayback(&animations[i], playbackOptions);
            m_animations[playbackOptions.AnimationSlot] = animationPlayback;
            elog::LogInfo(core::string::format("Successfully set animation: {}", animationName.c_str()));
            return true;
        }
    }

    elog::LogInfo(core::string::format("Failed to set animation: {}", animationName.c_str()));
    return false;
}

const core::Vector<glm::mat4>& AnimationController::GetCurrentFrame() const
{
    return m_currentFrame;
}

namespace {
struct BoneTransform
{
    int index;
    glm::mat4 ParentTransform;
};

struct BoneTransformChannels
{
    glm::vec3 pos, scale;
    glm::quat rot;
    glm::mat4 FullTransform(){
        return glm::translate(glm::mat4(1), pos) * glm::toMat4(rot) *
            glm::scale(glm::mat4(1), scale);
    }
};
}

bool AnimationController::IsPlaying() const
{
    for(const auto& playback: m_animations){
        if(playback.IsFinished() == false){
            return true;
        }
    }

    return false;
}

void AnimationController::Animate(float deltaTimeInSeconds)
{
    core::Vector<AnimationPlayback> activeAnimationPlaybacks;

    for(auto& playback: m_animations){
        if(playback.IsFinished() == false){
            activeAnimationPlaybacks.emplace_back(playback);
        }
    }

    if(activeAnimationPlaybacks.size() == 0){
        return;
    }

    auto & armature = m_animatedMesh->GetArmature();
    auto & bones = armature.GetBones();

    for(auto& playback: activeAnimationPlaybacks){
        playback.AdvanceAnimationTime(deltaTimeInSeconds);
    }

    for(auto& playback: activeAnimationPlaybacks){
        m_animations[playback.GetPlaybackSlot()] = playback;
    }

    core::Stack<BoneTransform> boneIndexStack;

    for(int i = 0; i < bones.size(); i++ ){
        if(bones[i].parent < 0){
            boneIndexStack.push(BoneTransform {
                i,
                glm::mat4(1)
            });
        }
    }

    while(!boneIndexStack.empty()){
        BoneTransform boneInfo = boneIndexStack.top();
        boneIndexStack.pop();

        BoneTransformChannels channels;
        auto & bone = bones[boneInfo.index];

        for(auto& activePlayback: activeAnimationPlaybacks){
            auto & boneData = activePlayback.GetAnimation()->BoneKeys[boneInfo.index];
            boneData.GetTransform(activePlayback.GetCurrentTime(), channels.pos, channels.scale, channels.rot);
        }

        auto globalTransform = boneInfo.ParentTransform * channels.FullTransform();
        m_boneTransformNoOffset[boneInfo.index] = armature.GetGlobalInverseTransform() * globalTransform;
        m_currentFrame[boneInfo.index] = m_boneTransformNoOffset[boneInfo.index] * bone.offset;

        for(int i = 0; i < bones.size(); i++){
            if(bones[i].parent == boneInfo.index){
                boneIndexStack.push(BoneTransform {
                    i,
                    globalTransform
                });
            }
        }
    }
}

glm::mat4 AnimationController::GetBoneTransformation(core::String name)
{
    auto& bones = m_animatedMesh->GetArmature().GetBones();

    for(int i = 0; i < bones.size(); i++){
        auto& bone = bones[i];

        if(bone.name == name){
            return m_boneTransformNoOffset[i];
        }
    }

    elog::LogWarning("Bone transform not found: " + name);
    return glm::mat4(1);
}

bool AnimationController::IsAnimationPlaying(core::String animation) const
{
    for(const auto & anim: m_animations){
        if(anim.IsFinished() == false && anim.GetName() == animation){
            return true;
        }
    }

    return false;
}


}
