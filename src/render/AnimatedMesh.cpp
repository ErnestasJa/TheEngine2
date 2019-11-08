#include "render/AnimatedMesh.h"
#include "render/IGpuBufferArrayObject.h"
#include "render/IGpuBufferObject.h"
#include "utils/Math.h"

namespace render {
void Animation::set_frame(uint32_t frame)
{
    const uint32_t offset = (frame%this->frames.size());

    for (std::size_t i = 0; i < this->bones.size(); i++)
    {
        if (this->bones[i].parent >= 0) {
            current_frame[i] = current_frame[this->bones[i].parent] * frames[offset][i];
        }
        else {
            current_frame[i] = frames[offset][i];
        }
    }

    for(int i = 0; i < bones.size(); i++){
        current_frame[i] = glm::transpose(current_frame[i]);
    }
}

void Animation::set_interp_frame(float f)
{
    if (this->frames.size() <= 0) return;

    int frame1 = (int)floor(f),
        frame2 = frame1 + 1;

    float frameoffset = f - frame1;
    frame1 %= this->frames.size();
    frame2 %= this->frames.size();
    core::Vector<glm::mat4> &mat1 = frames[frame1],
        &mat2 = frames[frame2];
    // Interpolate matrixes between the two closest frames and concatenate with parent matrix if necessary.
    // Concatenate the result with the inverse of the base pose.
    // You would normally do animation blending and inter-frame blending here in a 3D engine.

    //elog::LogInfo(core::string::format("Interpolating between frames: {} -> {}", frame1, frame2));

    for (std::size_t i = 0; i < bones.size(); i++)
    {
        glm::mat4 mat = mat1[i] * (1 - frameoffset) + mat2[i] * frameoffset;
        if (bones[i].parent >= 0) {
            current_frame[i] = current_frame[bones[i].parent] * mat;
        }
        else {
            current_frame[i] = mat;
        }
    }

    for(int i = 0; i < bones.size(); i++){
        current_frame[i] = glm::transpose(current_frame[i]);
    }
}

AnimatedMesh::AnimatedMesh()
{
}

AnimatedMesh::AnimatedMesh(core::SharedPtr<IGpuBufferArrayObject> vao)
{
    m_vao = vao;
}

void AnimatedMesh::Upload()
{
    m_vao->GetBufferObject(0)->UpdateBuffer(IndexBuffer.size(), IndexBuffer.data());
    m_vao->GetBufferObject(1)->UpdateBuffer(UVBuffer.size(), UVBuffer.data());
    m_vao->GetBufferObject(2)->UpdateBuffer(VertexBuffer.size(), VertexBuffer.data());
    m_vao->GetBufferObject(3)->UpdateBuffer(NormalBuffer.size(), NormalBuffer.data());
    m_vao->GetBufferObject(4)->UpdateBuffer(BlendIndexBuffer.size(), BlendIndexBuffer.data());
    m_vao->GetBufferObject(5)->UpdateBuffer(BlendWeightBuffer.size(), BlendWeightBuffer.data());
}

void AnimatedMesh::Render()
{
    m_vao->Render(IndexBuffer.size());
}

Animation & AnimatedMesh::GetAnimationData(){
    return m_animation;
}

}
