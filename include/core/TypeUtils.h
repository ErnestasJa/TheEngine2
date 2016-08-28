#ifndef TYPEUTILITIES_H
#define TYPEUTILITIES_H

namespace core {
union generic_data_access_handle
{
    uint8_t* ptr;
    int8_t* as_int8_t;
    uint8_t* as_uint8_t;
    int16_t* as_int16_t;
    uint16_t* as_uint16_t;
    int32_t* as_int32_t;
    uint32_t* as_uint32_t;
    float* as_float;
    struct
    {
        union
        {
            float x;
            float u;
        };
        union
        {
            float y;
            float v;
        };
    } * as_vec2;
    struct
    {
        union
        {
            float x;
            float r;
        };
        union
        {
            float y;
            float g;
        };
        union
        {
            float z;
            float b;
        };
    } * as_vec3;
    struct
    {
        union
        {
            float x;
            float r;
        };
        union
        {
            float y;
            float g;
        };
        union
        {
            float z;
            float b;
        };
        union
        {
            float w;
            float a;
        };
    } * as_vec4;
};
}

#endif