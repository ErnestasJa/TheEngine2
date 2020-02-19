#ifndef THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_ANIMATEDMESH_CPP_BONE_H_
#define THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_ANIMATEDMESH_CPP_BONE_H_

namespace render::anim {
struct Bone
{
    int32_t parent;
    core::String name;
    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;
    glm::mat4 offset;
    glm::mat4 bone_end;
    glm::mat4 transform;
};
}

#endif // THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_ANIMATEDMESH_CPP_BONE_H_
