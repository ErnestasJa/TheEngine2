#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "render/RenderInc.h"

#include "Include.h"

class RenderObject
{
public:
    RenderObject(
        render::IRenderer *renderer, const core::SharedPtr<Mesh> &mesh,
        const core::SharedPtr<material::BaseMaterial> &material,
        const core::Vector<core::SharedPtr<render::ITexture>> &textures)
        : m_renderer(renderer),
          m_mesh(mesh),
          m_material(material),
          m_textures(textures)
    {
        m_mesh->UpdateData(geom::CubeIndices, geom::CubeVertices,
                           geom::CubeNormals, geom::CubeUV);
    }

    void Render() const
    {
        m_renderer->SetActiveTextures(m_textures);
        m_material->Use(Transform);
        m_mesh->Render();
    }

    glm::mat4 Transform;

private:
    render::IRenderer *m_renderer;
    core::SharedPtr<Mesh> m_mesh;
    core::SharedPtr<material::BaseMaterial> m_material;
    core::Vector<core::SharedPtr<render::ITexture>> m_textures;
};

#endif