#ifndef ENGINE_ANIMATEDMESH_H
#define ENGINE_ANIMATEDMESH_H

#include "glm/glm.hpp"
#include <glm/detail/type_quat.hpp>
#include <render/BufferDescriptor.h>

namespace render {
struct BufferDescriptor;
class IGpuBufferArrayObject;
class IRenderer;

struct Bone
{
    int32_t parent;
    std::string name;
    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;
};

struct AnimationInfo
{
    std::string name;
    uint32_t start, num;
    float framerate;
    bool loop;
};

struct Animation
{
    core::Vector<core::Vector<glm::mat3x4>>      frames;
    core::Vector<glm::mat3x4>           current_frame;
    core::Vector<Bone>       bones;
    core::Vector<AnimationInfo>  info;
    core::Vector<glm::vec3> bone_colors;


    void set_frame(uint32_t frame);
    void set_interp_frame(float frame);
    void AssignBoneColors();
};

struct sub_mesh
{
    std::string name;
    std::string material_name;
    uint32_t start, num_indices;
};

class AnimatedMesh
{
public:
    core::Vector<uint32_t> IndexBuffer;
    core::Vector<glm::vec2> UVBuffer;
    core::Vector<glm::vec3> VertexBuffer;
    core::Vector<glm::vec3> NormalBuffer;
    core::Vector<glm::tvec4<uint8_t>> BlendIndexBuffer;
    core::Vector<glm::tvec4<uint8_t>> BlendWeightBuffer;

    AnimatedMesh();

    AnimatedMesh(core::SharedPtr<IGpuBufferArrayObject> vao);

    void Upload();

    void Render();

    Animation & GetAnimationData();

protected:
    Animation m_animation;
    core::SharedPtr<IGpuBufferArrayObject> m_vao;
};

}

#endif // ENGINE_ANIMATEDMESH_H
