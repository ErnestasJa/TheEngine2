#include "render/animation/AnimationController.h"
#include "render/AnimatedMesh.h"

namespace render::anim {

AnimationController::AnimationController(render::AnimatedMesh* mesh)
:m_animatedMesh(mesh), m_currentAnimation(nullptr), m_animationTime(0) {
    m_currentFrame.resize(m_animatedMesh->GetArmature().GetBones().size(), glm::mat4(1));
    m_boneTransformNoOffset.resize(m_animatedMesh->GetArmature().GetBones().size(), glm::mat4(1));
}

bool AnimationController::SetAnimation(int animationIndex)
{
    auto& animations = m_animatedMesh->GetAnimations();

    if(animationIndex < animations.size()){
        m_currentAnimation = &animations[animationIndex];
        m_animationTime = 0;
        elog::LogInfo(core::string::format("Successfully set animation[{}]: {}", animationIndex, m_currentAnimation->Name.c_str()));
        return true;
    }

    elog::LogInfo(core::string::format("Failed to set animation, requested anim index: {}", animationIndex));
    m_currentAnimation = nullptr;
    return false;
}

bool AnimationController::SetAnimation(core::String animationName)
{
    auto& animations = m_animatedMesh->GetAnimations();

    for(int i = 0; i < animations.size(); i++){
        if(animations[i].Name == animationName){
            m_currentAnimation = &animations[i];
            m_animationTime = 0;
            elog::LogInfo(core::string::format("Successfully set animation: {}", animationName.c_str()));
            return true;
        }
    }

    elog::LogInfo(core::string::format("Failed to set animation: {}", animationName.c_str()));
    m_currentAnimation = nullptr;
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
}

void AnimationController::Animate(float deltaTimeInSeconds)
{
    if(!m_currentAnimation){
        return;
    }
    auto & armature = m_animatedMesh->GetArmature();
    auto & bones = armature.GetBones();

    m_animationTime += deltaTimeInSeconds * m_currentAnimation->Fps;
    m_animationTime = glm::mod(m_animationTime, m_currentAnimation->Duration);


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

        auto & boneData = m_currentAnimation->BoneKeys[boneInfo.index];
        auto & bone = bones[boneInfo.index];

        auto transform = boneData.GetTransform(m_animationTime);

        auto globalTransform = boneInfo.ParentTransform * transform;
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


}
