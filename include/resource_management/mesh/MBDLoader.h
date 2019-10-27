#ifndef THEPROJECT2_MBDLOADER_H
#define THEPROJECT2_MBDLOADER_H

namespace res::mbd {
        struct Bone {
            core::String name;
            glm::vec3 head, tail;
        };

        class MBDLoader {
        public:
            core::Vector<Bone> LoadMBD(io::IFileSystem *fs, io::Path path);
        };
    }

#endif //THEPROJECT2_MBDLOADER_H
