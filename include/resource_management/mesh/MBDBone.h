#ifndef THEPROJECT2_MBDBONE_H
#define THEPROJECT2_MBDBONE_H
namespace res::mbd {
struct Bone
{
    int32_t index;
    int32_t parent;
    glm::vec3 head, tail;
    core::String name;
};
}
#endif // THEPROJECT2_MBDBONE_H
