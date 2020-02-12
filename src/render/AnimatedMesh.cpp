#include "render/AnimatedMesh.h"
#include "render/IGpuBufferArrayObject.h"
#include "render/IGpuBufferObject.h"
#include "utils/Math.h"

namespace render {

template<>
glm::vec3 AnimKey<glm::vec3>::Interpolate(const AnimKey<glm::vec3> & nextKey, float animTime) const {
    float delta = nextKey.Time - Time;
    float interpFactor = (animTime - Time) /  delta;
    return Value + interpFactor * (nextKey.Value - Value);
}

template<>
glm::quat AnimKey<glm::quat>::Interpolate(const AnimKey<glm::quat> & nextKey, float animTime) const {
    float delta = nextKey.Time - Time;
    float interpFactor = (animTime - Time) /  delta;
    return glm::mix(Value, nextKey.Value, interpFactor);
}

AnimatedMesh::AnimatedMesh()
{
}

AnimatedMesh::AnimatedMesh(core::SharedPtr<IGpuBufferArrayObject> vao)
{
    m_vao = vao;
}

void dump_buffer(core::String name, const core::Vector<glm::vec4> & buffer){
    core::String formattedBuffer;

    formattedBuffer += "Buffer " + name + ": ";

    int x = 0;
    for(auto & v : buffer){

        formattedBuffer += core::string::format("v[{}] = [{:03.2f},{:03.2f},{:03.2f},{:03.2f}]\n", x, v.x, v.y, v.z, v.w);
        x++;
    }
    formattedBuffer += "\n\n";
    elog::LogInfo(formattedBuffer);
}

void AnimatedMesh::Upload()
{
    m_vao->GetBufferObject(0)->UpdateBuffer(IndexBuffer.size(), IndexBuffer.data());
    m_vao->GetBufferObject(1)->UpdateBuffer(UVBuffer.size(), UVBuffer.data());
    m_vao->GetBufferObject(2)->UpdateBuffer(VertexBuffer.size(), VertexBuffer.data());
    m_vao->GetBufferObject(3)->UpdateBuffer(NormalBuffer.size(), NormalBuffer.data());
    m_vao->GetBufferObject(4)->UpdateBuffer(BlendIndexBuffer.size(), BlendIndexBuffer.data());
    m_vao->GetBufferObject(5)->UpdateBuffer(BlendWeightBuffer.size(), BlendWeightBuffer.data());

    //dump_buffer("BlendIndexBuffer", BlendIndexBuffer);
    //dump_buffer("BlendWeightBuffer", BlendWeightBuffer);
}


void AnimatedMesh::Render()
{
    m_vao->Render(IndexBuffer.size());
}

AnimationData & AnimatedMesh::GetAnimationData(){
    return m_animation;
}

void AnimatedMesh::Clear()
{
    IndexBuffer.clear();
    UVBuffer.clear();
    VertexBuffer.clear();
    NormalBuffer.clear();
    BlendIndexBuffer.clear();
    BlendWeightBuffer.clear();
    Upload();
}



void AnimationData::Animate(float time) {
    if(!current_animation){
        return;
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

        auto & boneData = current_animation->BoneKeys[boneInfo.index];
        auto & bone = bones[boneInfo.index];

        auto transform = boneData.GetTransform(time);

        auto globalTransform = boneInfo.ParentTransform * transform;
        current_frame[boneInfo.index] = GlobalInverseTransform * globalTransform * bone.offset;

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
}
