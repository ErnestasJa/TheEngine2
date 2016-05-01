#ifndef SAMPLE_MESH_H
#define SAMPLE_MESH_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "render/RenderInc.h"

class Mesh
{
public:
    std::vector<render::BufferDescriptor> BufferDescriptors;
    core::Vector<glm::vec2> UVBuffer;
    core::Vector<glm::vec3> VertexBuffer;
    core::Vector<glm::vec3> NormalBuffer;
    core::Vector<glm::vec3> TangentBuffer;
    core::Vector<glm::vec3> BiTangentBuffer;

    core::Vector<uint32_t> IndexBuffer;
    core::SharedPtr<render::IGpuBufferArrayObject> vao;

    Mesh()
    {
    }

    Mesh(render::IRenderer* ptr)
    {
        BufferDescriptors.push_back(render::BufferDescriptor{
            1, render::BufferObjectType::index,
            render::BufferComponentDataType::uint32, 0});

        BufferDescriptors.push_back(render::BufferDescriptor{
            2, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 0});

        BufferDescriptors.push_back(render::BufferDescriptor{
            3, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 1});

        BufferDescriptors.push_back(render::BufferDescriptor{
            3, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 2});

        BufferDescriptors.push_back(render::BufferDescriptor{
            3, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 3});

        BufferDescriptors.push_back(render::BufferDescriptor{
            3, render::BufferObjectType::vertex,
            render::BufferComponentDataType::float32, 4});

        vao = ptr->CreateBufferArrayObject(BufferDescriptors);
    }

    void Upload()
    {
        // clang-format off
        vao->GetBufferObject(0)->UpdateBuffer(IndexBuffer.size(), IndexBuffer.data());
        vao->GetBufferObject(1)->UpdateBuffer(UVBuffer.size(), UVBuffer.data());
        vao->GetBufferObject(2)->UpdateBuffer(VertexBuffer.size(), VertexBuffer.data());
        vao->GetBufferObject(3)->UpdateBuffer(NormalBuffer.size(), NormalBuffer.data());
        vao->GetBufferObject(4)->UpdateBuffer(TangentBuffer.size(), TangentBuffer.data());
        vao->GetBufferObject(5)->UpdateBuffer(BiTangentBuffer.size(), BiTangentBuffer.data());
        // clang-format on
    }

    void UpdateData(const core::Vector<uint32_t>& indices,
                    const core::Vector<glm::vec3>& vertices,
                    const core::Vector<glm::vec3>& normals,
                    const core::Vector<glm::vec2>& uvs)
    {
        VertexBuffer = vertices;
        NormalBuffer = normals;
        IndexBuffer = indices;
        UVBuffer = uvs;
        ComputeTangentsAndBiTangents(IndexBuffer, VertexBuffer, UVBuffer,
                                     TangentBuffer, BiTangentBuffer);
        Upload();
    }

    void Render()
    {
        vao->Render(IndexBuffer.size());
    }

private:
    static void ComputeTangentsAndBiTangents(
        // inputs
        const core::Vector<uint32_t>& indices,
        const core::Vector<glm::vec3>& vertices,
        const core::Vector<glm::vec2>& uvs,
        // outputs
        core::Vector<glm::vec3>& tangents, core::Vector<glm::vec3>& bitangents);
};

void Mesh::ComputeTangentsAndBiTangents(
    // inputs
    const core::Vector<uint32_t>& indices,
    const core::Vector<glm::vec3>& vertices, const core::Vector<glm::vec2>& uvs,
    // outputs
    core::Vector<glm::vec3>& tangents, core::Vector<glm::vec3>& bitangents)
{
    tangents.clear();
    bitangents.clear();
    for (uint32_t i = 0; i < indices.size(); i += 3) {
        // Shortcuts for vertices
        int i0 = indices[i + 0];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        const glm::vec3& v0 = vertices[i0];
        const glm::vec3& v1 = vertices[i1];
        const glm::vec3& v2 = vertices[i2];

        // Shortcuts for UVs
        const glm::vec2& uv0 = uvs[i0];
        const glm::vec2& uv1 = uvs[i1];
        const glm::vec2& uv2 = uvs[i2];

        // Edges of the triangle : postion delta
        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent =
            (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent =
            (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        // Same thing for binormals
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }
}

#endif