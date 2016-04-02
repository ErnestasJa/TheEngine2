#ifndef BUFFERDESCRIPTOR_H
#define BUFFERDESCRIPTOR_H

namespace render
{
enum class BufferComponentDataType {
    uint8,
    int8,
    uint16,
    int16,
    uint32,
    int32,
    float32,
};

enum class BufferObjectType {
    vertex,
    index,
};

struct BufferDescriptor {
    int32_t component_count;
    BufferObjectType type;
    BufferComponentDataType component_type;
    uint32_t layout_location;
};
}

#endif