#ifndef ENGINE_BASEMATERIAL_H
#define ENGINE_BASEMATERIAL_H

#include <glm/fwd.hpp>
#include <utility>

namespace render {
class IGpuProgram;
class IGpuProgramUniform;
class ITexture;
}

namespace material {
//static struct
//{
//    glm::mat4 View, Projection;
//} SharedUniforms;

class BaseMaterial
{
public:
    BaseMaterial(render::IGpuProgram*  shader);
    virtual void Use();

    void SetI(const core::String & name, int i);
    void SetVec3(const core::String & name, glm::vec3 uniformValue);
    void SetMat3(const core::String & name, glm::mat3 uniformValue);
    void SetMat4(const core::String & name, glm::mat4 uniformValue);
    void SetTexture(int slot, render::ITexture * texture);
    const core::Array<render::ITexture*, 8>& GetTextures();

protected:
    core::Array<render::ITexture*,8> m_textures;
    render::IGpuProgram* m_shader;
    bool m_textureListNeedsRebuild;
};
}


#endif // ENGINE_BASEMATERIAL_H
