#include "resource_management/mesh/IQMLoader.h"
#include "IQM.h"
#include "filesystem/IFileSystem.h"
#include "filesystem/Path.h"
#include <cstring>
#include <random>
#include <render/AnimatedMesh.h>
#include <utils/Math.h>

namespace res {
bool load_header(const uint8_t* data, iqm::iqmheader& header);
void load_mesh(render::AnimatedMesh* mesh, const core::TByteArray& data,
               const iqm::iqmheader& header);

IQMLoader::IQMLoader(io::IFileSystem* fileSystem)
    : m_fileSystem(fileSystem)
{
}
void IQMLoader::Load(render::AnimatedMesh* mesh, io::Path path)
{
  auto file = m_fileSystem->OpenRead(path);
  if (!file) {
    return;
  }

  core::TByteArray contents;
  if (!file->Read(contents)) {
    return;
  }

  iqm::iqmheader header;
  load_header(contents.data(), header);
  load_mesh(mesh, contents, header);
}

bool load_header(const uint8_t* data, iqm::iqmheader& header)
{
  memcpy((void*)&header, (void*)data, sizeof(header));

  if (!std::strcmp(iqm::IQM_MAGIC, header.magic) && header.version == iqm::IQM_VERSION &&
      header.filesize > 0) {
    elog::LogWarning(
        core::string::format("IQM File appears to be correct and not empty. (MAGIC:%s,VERSION:%i)",
                             header.magic, header.version));
    return false;
  }

  elog::LogInfo("IQM header OK");
  return true;
}

template <class T>
void LoadArray(const core::TByteArray& data, core::Vector<T>& bufferOut, uint32_t start,
               uint32_t count)
{
  bufferOut.resize(count);
  std::copy((T*)&data[start], (T*)&data[start + count * sizeof(T)], &bufferOut[0]);
}

void InvertFaceWinding(core::Vector<uint32_t>& indexBuffer)
{
  for (int i = 0; i < indexBuffer.size(); i += 3) {
    std::swap(indexBuffer[i], indexBuffer[i + 2]);
  }
}

void load_animation(render::Animation& animOut, const core::TByteArray& data,
                    const iqm::iqmheader& header);

void load_mesh(render::AnimatedMesh* mesh, const core::TByteArray& data,
               const iqm::iqmheader& header)
{
  const uint8_t* texts              = (const uint8_t*)&data[header.ofs_text];
  iqm::iqmmesh* submeshes           = (iqm::iqmmesh*)&data[header.ofs_meshes];
  iqm::iqmvertexarray* vertexarrays = (iqm::iqmvertexarray*)&data[header.ofs_vertexarrays];

  LoadArray(data, mesh->IndexBuffer, header.ofs_triangles, header.num_triangles * 3);
  InvertFaceWinding(mesh->IndexBuffer);

  for (uint32_t i = 0; i < header.num_vertexarrays; i++) {
    iqm::iqmvertexarray va = vertexarrays[i];

    switch (va.Type) {
    case iqm::IQM_POSITION: {
      LoadArray(data, mesh->VertexBuffer, va.offset, header.num_vertexes);
      break;
    }
    case iqm::IQM_TEXCOORD: {
      LoadArray(data, mesh->UVBuffer, va.offset, header.num_vertexes);
      break;
    }
    case iqm::IQM_NORMAL: {
      LoadArray(data, mesh->NormalBuffer, va.offset, header.num_vertexes);
      break;
    }
    case iqm::IQM_BLENDINDEXES: {
      LoadArray(data, mesh->BlendIndexBuffer, va.offset, header.num_vertexes);
      break;
    }
    case iqm::IQM_BLENDWEIGHTS: {
      LoadArray(data, mesh->BlendWeightBuffer, va.offset, header.num_vertexes);
      break;
    }
    }
  }

  load_animation(mesh->GetAnimationData(), data, header);

  elog::LogInfo(core::string::format("IndexBuffer size:       {}", mesh->IndexBuffer.size()));
  elog::LogInfo(core::string::format("VertexBuffer size:      {}", mesh->VertexBuffer.size()));
  elog::LogInfo(core::string::format("UVBuffer size:          {}", mesh->UVBuffer.size()));
  elog::LogInfo(core::string::format("NormalBuffer size:      {}", mesh->NormalBuffer.size()));
  elog::LogInfo(core::string::format("BlendIndexBuffer size:  {}", mesh->BlendIndexBuffer.size()));
  elog::LogInfo(core::string::format("BlendWeightBuffer size: {}", mesh->BlendWeightBuffer.size()));

  mesh->Upload();
}

void assign_bone_colors(render::Animation& animOut)
{
  auto randomDevice = std::random_device();
  std::uniform_int_distribution<int> randomDistribution(0, 255);

  animOut.bone_colors.resize(animOut.bones.size());

  for (auto& color : animOut.bone_colors) {
    color = glm::tvec3<uint8_t>(randomDistribution(randomDevice), randomDistribution(randomDevice),
                                randomDistribution(randomDevice));
  }
}

void load_animation(render::Animation& animOut, const core::TByteArray& data,
                    const iqm::iqmheader& header)
{
  elog::LogInfo(core::string::format("\nnum_anims: {}\n"
                                     "num_frames: {}\n"
                                     "num_poses: {}\n"
                                     "num_joints: {}\n",
                                     header.num_anims, header.num_frames, header.num_poses,
                                     header.num_joints));


  if (header.num_poses != header.num_joints) {
    elog::LogInfo(core::string::format("Joint/pose mismatch. Poses: {}, Joints: {}",
                                       header.num_poses, header.num_joints));
    return;
  }

  auto texts      = (const char*)&data[header.ofs_text];
  auto joints     = (iqm::iqmjoint*)&data[header.ofs_joints];
  auto poses      = (iqm::iqmpose*)&data[header.ofs_poses];
  auto anims      = (iqm::iqmanim*)&data[header.ofs_anims];
  auto frame_data = (uint16_t*)&data[header.ofs_frames];

  animOut.frames.resize(header.num_frames);

  for (uint32_t i = 0; i < header.num_frames; i++)
    animOut.frames[i].resize(header.num_joints);

  animOut.current_frame.resize(header.num_joints);
  animOut.bones.resize(header.num_joints);
  animOut.info.resize(header.num_anims);

  auto base_frame_ptr         = core::UniquePtr<glm::mat4[]>(new glm::mat4[header.num_joints]);
  auto base_frame             = base_frame_ptr.get();
  auto inverse_base_frame_ptr = core::UniquePtr<glm::mat4[]>(new glm::mat4[header.num_joints]);
  auto inverse_base_frame     = inverse_base_frame_ptr.get();

  for (uint32_t i = 0; i < header.num_joints; i++) {
    iqm::iqmjoint& j = joints[i];

    render::Bone& bone = animOut.bones[i];
    bone.name          = &texts[j.name];
    bone.parent        = j.parent;
    bone.pos           = glm::vec3(j.translate[0], j.translate[1], j.translate[2]);
    bone.scale         = glm::vec3(j.scale[0], j.scale[1], j.scale[2]);
    bone.rot = glm::normalize(glm::quat(j.rotate[3], j.rotate[0], j.rotate[1], j.rotate[2]));

    auto boneTransform = glm::mat4(1.f);
    boneTransform      = glm::translate(boneTransform, bone.pos) * glm::toMat4(bone.rot) *
                    glm::scale(boneTransform, bone.scale);

    base_frame[i]         = boneTransform;
    inverse_base_frame[i] = glm::inverse(base_frame[i]);

    if (j.parent >= 0) {
      base_frame[i]         = base_frame[j.parent] * base_frame[i];
      inverse_base_frame[i] = inverse_base_frame[i] * inverse_base_frame[j.parent];
    }
  }

  for (int frameIndex = 0; frameIndex < (int)header.num_frames; frameIndex++) {
    for (int poseIndex = 0; poseIndex < (int)header.num_poses; poseIndex++) {
      iqm::iqmpose& p = poses[poseIndex];
      glm::quat rotate;
      glm::vec3 translate, scale;
      translate.x = p.channeloffset[0];
      if (p.mask & 0x01u)
        translate.x += *frame_data++ * p.channelscale[0];
      translate.y = p.channeloffset[1];
      if (p.mask & 0x02u)
        translate.y += *frame_data++ * p.channelscale[1];
      translate.z = p.channeloffset[2];
      if (p.mask & 0x04u)
        translate.z += *frame_data++ * p.channelscale[2];
      rotate.x = p.channeloffset[3];
      if (p.mask & 0x08u)
        rotate.x += *frame_data++ * p.channelscale[3];
      rotate.y = p.channeloffset[4];
      if (p.mask & 0x10u)
        rotate.y += *frame_data++ * p.channelscale[4];
      rotate.z = p.channeloffset[5];
      if (p.mask & 0x20u)
        rotate.z += *frame_data++ * p.channelscale[5];
      rotate.w = p.channeloffset[6];
      if (p.mask & 0x40u)
        rotate.w += *frame_data++ * p.channelscale[6];
      scale.x = p.channeloffset[7];
      if (p.mask & 0x80u)
        scale.x += *frame_data++ * p.channelscale[7];
      scale.y = p.channeloffset[8];
      if (p.mask & 0x100u)
        scale.y += *frame_data++ * p.channelscale[8];
      scale.z = p.channeloffset[9];
      if (p.mask & 0x200u)
        scale.z += *frame_data++ * p.channelscale[9];

      /// Concatenate each pose with the inverse base pose to avoid doing this at animation
      /// time. If the joint has a parent, then it needs to be pre-concatenated with its
      /// parent's base pose. Thus it all negates at animation time like so:
      ///   (parentPose * parentInverseBasePose) * (parentBasePose * childPose *
      ///   childInverseBasePose) => parentPose * (parentInverseBasePose * parentBasePose) *
      ///   childPose * childInverseBasePose => parentPose * childPose * childInverseBasePose

      glm::mat4 mat = glm::mat4(1.f);
      mat           = glm::translate(mat, translate) * glm::toMat4(glm::normalize(rotate)) *
            glm::scale(mat, scale);

      if (p.parent >= 0) {
        animOut.frames[frameIndex][poseIndex] =
            base_frame[p.parent] * mat * inverse_base_frame[poseIndex];
      }
      else {
        animOut.frames[frameIndex][poseIndex] = mat * inverse_base_frame[poseIndex];
      }
    }
  }

  for (int i = 0; i < (int)header.num_anims; i++) {
    render::AnimationInfo& ai = animOut.info[i];
    iqm::iqmanim& a           = anims[i];
    ai.name                   = &texts[a.name];
    ai.start                  = a.first_frame;
    ai.num                    = a.num_frames;
    ai.loop                   = utils::CheckBit(a.flags, iqm::IQM_LOOP);

    elog::LogInfo(core::string::format("anim name: {}, start: {}, end: {}", ai.name.c_str(),
                                       ai.start, ai.start + ai.num));
  }

  assign_bone_colors(animOut);
  elog::LogInfo(core::string::format("Total frames: {}", animOut.frames.size()));
}

} // namespace res