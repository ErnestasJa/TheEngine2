#include "resource_management/mesh/IQMLoader.h"
#include "IQM.h"
#include "filesystem/IFileSystem.h"
#include "filesystem/Path.h"
#include <cstring>
#include <render/AnimatedMesh.h>
#include <utils/Math.h>

namespace res {
bool load_header(const uint8_t * data, iqm::iqmheader& header);
void load_mesh(render::AnimatedMesh * mesh, const core::TByteArray & data, const iqm::iqmheader& header);

IQMLoader::IQMLoader(core::SharedPtr<io::IFileSystem> fileSystem)
: m_fileSystem(fileSystem)
{

}
void IQMLoader::Load(render::AnimatedMesh* mesh, io::Path path)
{
    auto file = m_fileSystem->OpenRead(path);
    if(!file){

        return;
    }

    core::TByteArray contents;
    if(!file->Read(contents)){
        return;
    }

    iqm::iqmheader header;
    load_header(contents.data(), header);
    load_mesh(mesh, contents, header);
}

bool load_header(const uint8_t * data, iqm::iqmheader& header)
{
    memcpy((void*)&header, (void*)data, sizeof(header));

    if (!std::strcmp(iqm::IQM_MAGIC, header.magic) && header.version == iqm::IQM_VERSION && header.filesize > 0){
        elog::LogWarning(core::string::CFormat("IQM File appears to be correct and not empty. (MAGIC:%s,VERSION:%i)", header.magic, header.version));
        return false;
    }

    elog::LogInfo("IQM header OK");
    return true;
}

template <class T>
void LoadArray(const core::TByteArray & data, core::Vector<T>& bufferOut, uint32_t start, uint32_t count){
    bufferOut.resize(count);
    std::copy((T*)&data[start], (T*)&data[start + count * sizeof(T)], &bufferOut[0]);
}

void InvertFaceWinding(core::Vector<uint32_t>& indexBuffer){
    for(int i = 0; i < indexBuffer.size(); i+=3){
        std::swap(indexBuffer[i], indexBuffer[i+2]);
    }
}

void load_animation(render::Animation& animOut, const core::TByteArray & data, const iqm::iqmheader & header);

void load_mesh(render::AnimatedMesh * mesh, const core::TByteArray & data, const iqm::iqmheader& header){
    const uint8_t * texts = (const uint8_t *)&data[header.ofs_text];
    iqm::iqmmesh* submeshes = (iqm::iqmmesh*)&data[header.ofs_meshes];
    iqm::iqmvertexarray* vertexarrays = (iqm::iqmvertexarray*)&data[header.ofs_vertexarrays];

    LoadArray(data, mesh->IndexBuffer, header.ofs_triangles, header.num_triangles*3);
    InvertFaceWinding(mesh->IndexBuffer);

    for (uint32_t i = 0; i < header.num_vertexarrays; i++) {
        iqm::iqmvertexarray va = vertexarrays[i];

        switch(va.Type){
            case iqm::IQM_POSITION:
            {
                LoadArray(data, mesh->VertexBuffer, va.offset, header.num_vertexes);
                break;
            }
            case iqm::IQM_TEXCOORD:
            {
                LoadArray(data, mesh->UVBuffer, va.offset, header.num_vertexes);
                break;
            }
            case iqm::IQM_NORMAL:
            {
                LoadArray(data, mesh->NormalBuffer, va.offset, header.num_vertexes);
                break;
            }
            case iqm::IQM_BLENDINDEXES:
            {
                LoadArray(data, mesh->BlendIndexBuffer, va.offset, header.num_vertexes);
                break;
            }
            case iqm::IQM_BLENDWEIGHTS:
            {
                LoadArray(data, mesh->BlendWeightBuffer, va.offset, header.num_vertexes);
                break;
            }
        }
    }

    load_animation(mesh->GetAnimationData(), data, header);

    elog::LogInfo(core::string::CFormat("IndexBuffer size:       %u", mesh->IndexBuffer.size()));
    elog::LogInfo(core::string::CFormat("VertexBuffer size:      %u", mesh->VertexBuffer.size()));
    elog::LogInfo(core::string::CFormat("UVBuffer size:          %u", mesh->UVBuffer.size()));
    elog::LogInfo(core::string::CFormat("NormalBuffer size:      %u", mesh->NormalBuffer.size()));
    elog::LogInfo(core::string::CFormat("BlendIndexBuffer size:  %u", mesh->BlendIndexBuffer.size()));
    elog::LogInfo(core::string::CFormat("BlendWeightBuffer size: %u", mesh->BlendWeightBuffer.size()));

    mesh->Upload();
}

void load_animation(render::Animation& animOut, const core::TByteArray & data, const iqm::iqmheader & header)
{
    elog::LogInfo(core::string::CFormat(
        "\nnum_anims: %i\n"
        "num_frames: %i\n"
        "num_poses: %i\n"
        "num_joints: %i\n"
        , header.num_anims
        , header.num_frames
        , header.num_poses
        , header.num_joints
    ));


    if (header.num_poses != header.num_joints){
        elog::LogInfo(core::string::CFormat("Joint/pose mismatch. Poses: %i, Joints: %i", header.num_poses, header.num_joints ));
        return;
    }

    const char* texts = (const char*)&data[header.ofs_text];
    iqm::iqmjoint*   joints = (iqm::iqmjoint*)&data[header.ofs_joints];
    iqm::iqmpose *   poses = (iqm::iqmpose*)&data[header.ofs_poses];
    iqm::iqmanim *   anims = (iqm::iqmanim*)&data[header.ofs_anims];
    uint16_t * frame_data = (uint16_t *)&data[header.ofs_frames];

    animOut.frames.resize(header.num_frames);

    for (uint32_t i = 0; i < header.num_frames; i++)
        animOut.frames[i].resize(header.num_joints);

    animOut.current_frame.resize(header.num_joints);
    animOut.bones.resize(header.num_joints);
    animOut.info.resize(header.num_anims);

    auto base_frame = new glm::mat3x4[header.num_joints];
    auto inverse_base_frame = new glm::mat3x4[header.num_joints];

    for (uint32_t i = 0; i < header.num_joints; i++)
    {
        iqm::iqmjoint &j = joints[i];

        render::Bone & b = animOut.bones[i];
        b.name = &texts[j.name];
        b.parent = j.parent;
        b.pos = glm::vec3(j.translate[0], j.translate[1], j.translate[2]);
        b.scale = glm::vec3(j.scale[0], j.scale[1], j.scale[2]);
        b.rot = glm::normalize(glm::quat(j.rotate[3], j.rotate[0], j.rotate[1], j.rotate[2]));

        utils::MakeJointMatrix(base_frame[i],
                        glm::normalize(glm::quat(j.rotate[3], j.rotate[0], j.rotate[1], j.rotate[2])),
                        glm::vec3(j.translate[0], j.translate[1], j.translate[2]),
                        glm::vec3(j.scale[0], j.scale[1], j.scale[2]));

        utils::Invert(inverse_base_frame[i], base_frame[i]);

        if (j.parent >= 0)
        {
            base_frame[i] = utils::mul(base_frame[j.parent], base_frame[i]);
            inverse_base_frame[i] = utils::mul(inverse_base_frame[i], inverse_base_frame[j.parent]);
        }
    }

    for (int i = 0; i < (int)header.num_frames; i++)
    {
        for (int j = 0; j < (int)header.num_poses; j++)
        {
            iqm::iqmpose &p = poses[j];
            glm::quat rotate;
            glm::vec3 translate, scale;
            translate.x = p.channeloffset[0];
            if (p.mask & 0x01) translate.x += *frame_data++ * p.channelscale[0];
            translate.y = p.channeloffset[1];
            if (p.mask & 0x02) translate.y += *frame_data++ * p.channelscale[1];
            translate.z = p.channeloffset[2];
            if (p.mask & 0x04) translate.z += *frame_data++ * p.channelscale[2];
            rotate.x = p.channeloffset[3];
            if (p.mask & 0x08) rotate.x += *frame_data++ * p.channelscale[3];
            rotate.y = p.channeloffset[4];
            if (p.mask & 0x10) rotate.y += *frame_data++ * p.channelscale[4];
            rotate.z = p.channeloffset[5];
            if (p.mask & 0x20) rotate.z += *frame_data++ * p.channelscale[5];
            rotate.w = p.channeloffset[6];
            if (p.mask & 0x40) rotate.w += *frame_data++ * p.channelscale[6];
            scale.x = p.channeloffset[7];
            if (p.mask & 0x80) scale.x += *frame_data++ * p.channelscale[7];
            scale.y = p.channeloffset[8];
            if (p.mask & 0x100) scale.y += *frame_data++ * p.channelscale[8];
            scale.z = p.channeloffset[9];
            if (p.mask & 0x200) scale.z += *frame_data++ * p.channelscale[9];

            /// Concatenate each pose with the inverse base pose to avoid doing this at animation time.
            /// If the joint has a parent, then it needs to be pre-concatenated with its parent's base pose.
            /// Thus it all negates at animation time like so:
            ///   (parentPose * parentInverseBasePose) * (parentBasePose * childPose * childInverseBasePose) =>
            ///   parentPose * (parentInverseBasePose * parentBasePose) * childPose * childInverseBasePose =>
            ///   parentPose * childPose * childInverseBasePose

            glm::mat3x4 mat;
            utils::MakeJointMatrix(mat, glm::normalize(rotate), translate, scale);

            if (p.parent >= 0) animOut.frames[i][j] = utils::mul(utils::mul(base_frame[p.parent], mat), inverse_base_frame[j]);
            else animOut.frames[i][j] = utils::mul(mat, inverse_base_frame[j]);
        }
    }

    for (int i = 0; i < (int)header.num_anims; i++)
    {
        render::AnimationInfo & ai = animOut.info[i];
        iqm::iqmanim &a = anims[i];
        ai.name = &texts[a.name];
        ai.start = a.first_frame;
        ai.num = a.num_frames;
        ai.loop = utils::CheckBit(a.flags, iqm::IQM_LOOP);


        elog::LogInfo(core::string::CFormat("anim name: %s, start: %i, end: %i", ai.name.c_str(), ai.start, ai.start+ai.num));
    }
    elog::LogInfo(core::string::CFormat("Total frames: %i", animOut.frames.size()));
}


}