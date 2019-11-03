#include <glm/glm.hpp>
#include "resource_management/mesh/MBDLoader.h"
#include "filesystem/IFileSystem.h"

namespace res::mbd
{
    struct Header {
        char magic[4];
        int text_offset, text_num;
        int bone_offset, bone_num;
    };

    struct RawBone {
        int name;
        float head[3];
        float tail[3];
    };

    core::Vector<Bone> MBDLoader::LoadMBD(io::IFileSystem *fs, io::Path path) {
        auto file = fs->OpenRead(path);
        if(!file){
            return core::Vector<Bone>();
        }

        core::TByteArray contents;
        if(!file->Read(contents)){
            return core::Vector<Bone>();
        }

        const uint8_t * data = contents.data();

        mbd::Header header;
        memcpy((void*)&header, (void*)data, sizeof(header));

        auto texts = (const char*)&data[header.text_offset];
        auto rawBones = (mbd::RawBone*)&data[header.bone_offset];

        auto bones = core::Vector<mbd::Bone>();


        for (uint32_t i = 0; i < header.bone_num; i++)
        {
            auto& rawBone = rawBones[i];
            bones.push_back(mbd::Bone {
                &texts[rawBone.name],
                {rawBone.head[0], rawBone.head[1], rawBone.head[2]},
                {rawBone.tail[0], rawBone.tail[1], rawBone.tail[2]},
            });
        }

        elog::LogInfo(core::string::format("\nMBD Header \n{{\n"
                "magic = '{}'\n"
                "text_offset = {}\n"
                "text_num = {}\n"
                "bone_offset = {}\n"
                "bone_num = {}\n"
                "}}\n",
                header.magic,
                header.text_offset, header.text_num,
                header.bone_offset, header.bone_num));

        for(auto& bone : bones) {
            elog::LogInfo(core::string::format("Bone [{}] = head: {}, tail: {}",
                    bone.name.c_str(),
                    bone.head,
                    bone.tail
                    ));
        }

        return bones;
    }
}