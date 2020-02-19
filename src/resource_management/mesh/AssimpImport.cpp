#include "resource_management/mesh/AssimpImport.h"
#include "render/animation/Bone.h"
#include "render/animation/BoneKeyCollection.h"
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure
#include <glm/gtx/quaternion.hpp>
#include <render/AnimatedMesh.h> // Output data structure
#include <render/BaseMesh.h>     // Output data structure

namespace res::mesh {

namespace {
glm::mat4 ToGlm(const aiMatrix4x4 from)
{
    glm::mat4 to;

    to[0][0] = (float)from.a1;
    to[0][1] = (float)from.b1;
    to[0][2] = (float)from.c1;
    to[0][3] = (float)from.d1;
    to[1][0] = (float)from.a2;
    to[1][1] = (float)from.b2;
    to[1][2] = (float)from.c2;
    to[1][3] = (float)from.d2;
    to[2][0] = (float)from.a3;
    to[2][1] = (float)from.b3;
    to[2][2] = (float)from.c3;
    to[2][3] = (float)from.d3;
    to[3][0] = (float)from.a4;
    to[3][1] = (float)from.b4;
    to[3][2] = (float)from.c4;
    to[3][3] = (float)from.d4;

    return to;
}
} // namespace

template <class TPredicate> core::Optional<int> FindBone(const aiMesh* aMesh, TPredicate callable)
{
    for (auto iBone = 0; iBone < aMesh->mNumBones; iBone++) {
        auto aBone = aMesh->mBones[iBone];
        if (callable(aBone)) {
            return iBone;
        }
    }

    return {};
}

static void MapBoneWeights(const aiMesh* aMesh, render::AnimatedMesh* mesh)
{
    core::Vector<glm::vec4> boneWeights;
    core::Vector<glm::vec4> boneIndices;
    core::Vector<uint8_t> vertexWeightCount;

    boneWeights.resize(aMesh->mNumVertices);
    boneIndices.resize(aMesh->mNumVertices);
    vertexWeightCount.resize(aMesh->mNumVertices);

    auto zeroVector = [](auto& v) {
        v.x = 0;
        v.y = 0;
        v.z = 0;
        v.w = 0;
    };

    for (auto& v : boneWeights) {
        zeroVector(v);
    }

    for (auto& v : boneIndices) {
        zeroVector(v);
    }

    for (auto& v : vertexWeightCount) {
        v = 0;
    }

    for (auto iBone = 0; iBone < aMesh->mNumBones; iBone++) {
        auto aBone = aMesh->mBones[iBone];

        for (int i = 0; i < aBone->mNumWeights; i++) {
            auto vertexId      = aBone->mWeights[i].mVertexId;
            auto currentWeight = vertexWeightCount[vertexId];

            if (currentWeight > 3) {
                elog::LogWarning(core::string::format(
                    "Vertex[{}] has too many weights, expected less than 5, skipping other weights",
                    vertexId));
                continue;
            }

            vertexWeightCount[vertexId]++;

            boneWeights[vertexId][currentWeight] = aBone->mWeights[i].mWeight;
            boneIndices[vertexId][currentWeight] = iBone;
        }
    }

    mesh->BlendIndexBuffer  = boneIndices;
    mesh->BlendWeightBuffer = boneWeights;
}

static void MapBoneHierarchy(const aiMesh* aMesh, render::AnimatedMesh* mesh, const aiScene* scene)
{
    core::Vector<render::anim::Bone> bones;
    bones.resize(aMesh->mNumBones);

    auto globalInverseTransform = glm::inverse(ToGlm(scene->mRootNode->mTransformation));

    for (auto iBone = 0; iBone < aMesh->mNumBones; iBone++) {
        auto aBone = aMesh->mBones[iBone];

        aiVector3t<ai_real> pos, scale;
        aiQuaterniont<ai_real> rot;
        aBone->mOffsetMatrix.Decompose(scale, rot, pos);

        render::anim::Bone bone;
        bone.name      = aBone->mName.data;
        bone.pos       = glm::vec3(pos.x, pos.y, pos.z);
        bone.scale     = glm::vec3(scale.x, scale.y, scale.z);
        bone.rot       = glm::quat(rot.w, rot.x, rot.y, rot.z);
        bone.transform = ToGlm(aBone->mNode->mTransformation);
        bone.offset    = ToGlm(aBone->mOffsetMatrix);

        bones[iBone] = bone;
    }

    for (auto iBone = 0; iBone < aMesh->mNumBones; iBone++) {
        auto aBone = aMesh->mBones[iBone];
        if (aBone->mNode->mParent) {
            auto parentName = aBone->mNode->mParent->mName;

            auto parentBone =
                FindBone(aMesh, [&parentName](aiBone* bone) { return bone->mName == parentName; });

            auto& bone = bones[iBone];

            if (parentBone) {
                auto parentIndex = parentBone.value();
                bone.parent      = parentIndex;
                elog::LogInfo(core::string::format("Bone[{}] '{}', parent[{}]: {}", iBone,
                                                   aBone->mName.C_Str(), parentIndex,
                                                   aMesh->mBones[parentIndex]->mName.C_Str()));
            }
            else {
                bone.parent = -1;
                elog::LogInfo(
                    core::string::format("Root bone[{}] '{}'", iBone, aBone->mName.C_Str()));
            }

            core::String currentBoneName(aBone->mNode->mName.C_Str());
            for (int i = 0; i < aBone->mNode->mNumChildren; i++) {
                core::String childBoneName(aBone->mNode->mChildren[i]->mName.C_Str());
                elog::LogInfo(core::string::format("bone: '{}', child: {}",
                                                   aBone->mNode->mName.C_Str(), childBoneName));
                if (currentBoneName + "_end" == childBoneName) {
                    auto boneEndNode = aBone->mNode->mChildren[i];
                    bone.bone_end    = ToGlm(boneEndNode->mTransformation);
                    elog::LogInfo(core::string::format("Loading bone end for: '{}'",
                                                       aBone->mNode->mName.C_Str()));
                    break;
                }
            }
        }
    }

    mesh->SetArmature(render::anim::Armature(globalInverseTransform, bones));

    MapBoneWeights(aMesh, mesh);
}

static int FindBoneIndex(render::AnimatedMesh* mesh, core::String boneName)
{
    auto& bones = mesh->GetArmature().GetBones();

    for (int index = 0; index < bones.size(); index++) {
        if (bones[index].name == boneName)
            return index;
    }

    return -1;
}

static void ReadAnimations(render::AnimatedMesh* mesh, const aiScene* scene)
{
    core::Vector<render::anim::Animation> animations;
    auto & armature = mesh->GetArmature();

    for (int animIndex = 0; animIndex < scene->mNumAnimations; animIndex++) {
        const aiAnimation* pAnimation = scene->mAnimations[animIndex];

        render::anim::Animation animation;
        animation.Name     = pAnimation->mName.C_Str();
        animation.Fps      = pAnimation->mTicksPerSecond;
        animation.Duration = pAnimation->mDuration;
        animation.BoneKeys.resize(armature.GetBones().size());

        elog::LogInfo(core::string::format("Loading animation '{}', fps: {}, duration: {}",
                                           animation.Name.c_str(), animation.Fps, animation.Duration));

        for (int nodeAnimIndex = 0; nodeAnimIndex < pAnimation->mNumChannels; nodeAnimIndex++) {
            const aiNodeAnim* pNodeAnim = pAnimation->mChannels[nodeAnimIndex];
            elog::LogInfo(core::string::format("Scale key count: {}", pNodeAnim->mNumScalingKeys));
            elog::LogInfo(
                core::string::format("Position key count: {}", pNodeAnim->mNumPositionKeys));
            elog::LogInfo(
                core::string::format("Rotation key count: {}", pNodeAnim->mNumRotationKeys));

            core::String boneName(pNodeAnim->mNodeName.C_Str());
            int boneIndex = FindBoneIndex(mesh, boneName);

            if (pNodeAnim->mNumScalingKeys == 0 && pNodeAnim->mNumPositionKeys == 0 &&
                pNodeAnim->mNumRotationKeys == 0) {
                elog::LogInfo(
                    core::string::format("Has no animation keys assigned: {}", boneName.c_str()));
                continue;
            }

            render::anim::BoneKeyCollection boneKeys;

            for (int posKeyIndex = 0; posKeyIndex < pNodeAnim->mNumPositionKeys; posKeyIndex++) {
                auto key = pNodeAnim->mPositionKeys[posKeyIndex];
                render::anim::AnimKey<glm::vec3> posKey;
                posKey.Value = glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
                posKey.Time  = key.mTime;
                boneKeys.PositionKeys.push_back(posKey);
            }

            for (int rotKeyIndex = 0; rotKeyIndex < pNodeAnim->mNumPositionKeys; rotKeyIndex++) {
                auto key = pNodeAnim->mRotationKeys[rotKeyIndex];
                render::anim::AnimKey<glm::quat> rotKey;
                rotKey.Value = glm::quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
                rotKey.Time  = key.mTime;
                boneKeys.RotationKeys.push_back(rotKey);
            }

            for (int scaleKeyIndex = 0; scaleKeyIndex < pNodeAnim->mNumPositionKeys;
                 scaleKeyIndex++) {
                auto key = pNodeAnim->mScalingKeys[scaleKeyIndex];
                render::anim::AnimKey<glm::vec3> scaleKey;
                scaleKey.Value = glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
                scaleKey.Time  = key.mTime;
                boneKeys.ScaleKeys.push_back(scaleKey);
            }

            if (boneIndex < 0) {
                boneKeys.BoneIndex    = 0;
                animation.ArmatureKeys = boneKeys;
                elog::LogInfo(core::string::format("Armature bone?: {}", boneName.c_str()));
                continue;
            }

            boneKeys.BoneIndex           = boneIndex;
            animation.BoneKeys[boneIndex] = boneKeys;
            elog::LogInfo(core::string::format("Bone {} position key count: {}", boneName.c_str(),
                                               boneKeys.PositionKeys.size()));
            elog::LogInfo(core::string::format("Bone {} rotation key count: {}", boneName.c_str(),
                                               boneKeys.RotationKeys.size()));
        }

        mesh->AddAnimation(animation);
    }
}

AssimpImport::AssimpImport(io::IFileSystem* fs, render::IRenderer* renderer)
{
    m_fileSystem = fs;
    m_renderer   = renderer;
}

core::UniquePtr<render::AnimatedMesh> AssimpImport::LoadMesh(io::Path path)
{
    auto file = m_fileSystem->OpenRead(path);
    core::TByteArray array;
    file->Read(array);

    Assimp::Importer importer;

    const aiScene* scene =
        importer.ReadFileFromMemory(array.data(), array.size(),
                                    aiProcess_CalcTangentSpace | aiProcess_FlipUVs |
                                        aiProcess_Triangulate |
                                        // aiProcess_JoinIdenticalVertices  |
                                        aiProcess_PopulateArmatureData |
                                        // aiProcess_SortByPType |
                                        aiProcess_GenBoundingBoxes);

    if (!scene) {
        elog::LogError(importer.GetErrorString());
        return nullptr;
    }
    auto filename = path.AsString();

    if (scene->mNumMeshes < 1) {
        elog::LogError(
            core::string::format("File '{}' does not contain any meshes", filename.c_str()));
    }

    elog::LogInfo(core::string::format("Loading mesh from file '{}'.", filename.c_str()));

    auto mesh = m_renderer->CreateAnimatedMesh();

    elog::LogInfo(core::string::format("Scene num meshes: '{}'", scene->mNumMeshes));

    for (auto iMesh = 0; iMesh < 1 /*scene->mNumMeshes*/; iMesh++) {
        auto assimpMesh = scene->mMeshes[iMesh];

        elog::LogInfo(core::string::format("AABB: 'min: [{},{},{}]', max: [{},{},{}]'",
                                           assimpMesh->mAABB.mMin.x, assimpMesh->mAABB.mMin.y,
                                           assimpMesh->mAABB.mMin.z, assimpMesh->mAABB.mMax.x,
                                           assimpMesh->mAABB.mMax.y, assimpMesh->mAABB.mMax.z));

        elog::LogInfo(core::string::format("Mesh vertex count: '{}'", assimpMesh->mNumVertices));
        elog::LogInfo(core::string::format("Mesh face count: '{}'", assimpMesh->mNumFaces));

        for (auto iVertex = 0; iVertex < assimpMesh->mNumVertices; iVertex++) {
            auto aVertex = assimpMesh->mVertices[iVertex];
            mesh->VertexBuffer.emplace_back(aVertex.x, aVertex.y, aVertex.z);
        }

        //        for(auto iColor = 0; iColor < assimpMesh->mNumVertices; iColor++){
        //            auto aColor = glm::vec3(iColor % 3 ? 0 : 1,iColor+1 % 3 ? 0 : 1,iColor+2 % 3 ?
        //            0 : 1); mesh->ColorBuffer.emplace_back(aColor.x, aColor.y, aColor.z);
        //        }

        for (auto iNormal = 0; iNormal < assimpMesh->mNumVertices; iNormal++) {
            auto aNormal = assimpMesh->mNormals[iNormal];
            mesh->NormalBuffer.emplace_back(aNormal.x, aNormal.y, aNormal.z);
        }

        // load first channel only
        for (auto iUV = 0; iUV < assimpMesh->mNumVertices; iUV++) {
            auto aUV = assimpMesh->mTextureCoords[0][iUV];
            mesh->UVBuffer.emplace_back(aUV.x, aUV.y);
        }

        for (auto iFace = 0; iFace < assimpMesh->mNumFaces; iFace++) {
            auto aFace = assimpMesh->mFaces[iFace];

            // elog::LogInfo(core::string::format("Mesh face[{}] index count: '{}'", iFace,
            // aFace.mNumIndices));
            for (auto iIndex = 0; iIndex < aFace.mNumIndices; iIndex++) {
                mesh->IndexBuffer.push_back(aFace.mIndices[iIndex]);
            }
        }

        MapBoneHierarchy(assimpMesh, mesh.get(), scene);
        ReadAnimations(mesh.get(), scene);
    }

    mesh->Upload();
    return mesh;
}
} // namespace res::mesh