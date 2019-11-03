#include "glm/gtx/norm.hpp"
namespace utils {

inline bool CheckBit(uint8_t b, uint8_t b2) { return (b & b2) == b2; }

inline glm::mat3x4 mul(const glm::mat3x4& m1, const glm::mat3x4& m2)
{
    glm::vec4 v1(m2[0] * m1[0].x + m2[1] * m1[0].y + m2[2] * m1[0].z);
    v1.w += m1[0].w;

    glm::vec4 v2(m2[0] * m1[1].x + m2[1] * m1[1].y + m2[2] * m1[1].z);
    v2.w += m1[1].w;

    glm::vec4 v3(m2[0] * m1[2].x + m2[1] * m1[2].y + m2[2] * m1[2].z);
    v3.w += m1[2].w;

    glm::mat3x4 out;
    out[0] = v1;
    out[1] = v2;
    out[2] = v3;

    return out;
}

inline void ConvertQuat(glm::mat3x4 & mat, const glm::quat & q)
{
    float x = q.x, y = q.y, z = q.z, w = q.w,
        tx = 2 * x, ty = 2 * y, tz = 2 * z,
        txx = tx*x, tyy = ty*y, tzz = tz*z,
        txy = tx*y, txz = tx*z, tyz = ty*z,
        twx = w*tx, twy = w*ty, twz = w*tz;
    mat[0] = glm::vec4(1.0f - (tyy + tzz), txy - twz, txz + twy, 0);
    mat[1] = glm::vec4(txy + twz, 1.0f - (txx + tzz), tyz - twx, 0);
    mat[2] = glm::vec4(txz - twy, tyz + twx, 1.0f - (txx + tyy), 0);
}

inline void Scale(glm::vec4 & v, const glm::vec3& s)
{
    v *= glm::vec4(s,0);
}

inline void MakeJointMatrix(glm::mat3x4 & mat, const glm::quat & rot, const glm::vec3& pos, const glm::vec3& s)
{
    ConvertQuat(mat, rot);

    Scale(mat[0], s);
    Scale(mat[1], s);
    Scale(mat[2], s);

    mat[0].w = pos.x;
    mat[1].w = pos.y;
    mat[2].w = pos.z;
}

inline void Invert(glm::mat3x4 & dest, const glm::mat3x4 & o)
{
    glm::mat3x3 invrot(
            glm::vec3(o[0].x, o[1].x, o[2].x),
            glm::vec3(o[0].y, o[1].y, o[2].y),
            glm::vec3(o[0].z, o[1].z, o[2].z)
            );

    invrot[0] /= glm::dot(invrot[0],invrot[0]);
    invrot[1] /= glm::dot(invrot[1],invrot[1]);
    invrot[2] /= glm::dot(invrot[2],invrot[2]);
    glm::vec3 trans(o[0].w, o[1].w, o[2].w);

    dest[0] = glm::vec4(invrot[0], -glm::dot(invrot[0], trans));
    dest[1] = glm::vec4(invrot[1], -glm::dot(invrot[1], trans));
    dest[2] = glm::vec4(invrot[2], -glm::dot(invrot[2], trans));
}
}