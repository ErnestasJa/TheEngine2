#include <glm/fwd.hpp>
namespace material {
class BaseMaterial;
}
namespace render {
class BaseMesh;

namespace debug {
///todo: remove from engine.
class DebugLineMesh {
public:
    DebugLineMesh(core::SharedPtr<BaseMesh> mesh, core::SharedPtr<material::BaseMaterial> material);
    void AddLine(glm::vec3 start, glm::vec3 end, glm::tvec3<uint8_t> color = glm::tvec3<uint8_t>(0,0,0));
    void AddGrid(int count, int spacing, glm::tvec3<uint8_t> color);
    void Clear();

    void Upload();

    BaseMesh * GetMesh() { return m_mesh.get(); }
    material::BaseMaterial * GetMaterial() { return m_material.get(); }
private:
    core::SharedPtr<BaseMesh> m_mesh;
    core::SharedPtr<material::BaseMaterial> m_material;
    bool m_isUploaded;
};
}
}