#ifndef ENGINE_POD_INC_H
#define ENGINE_POD_INC_H

/* ######## Motivation #########
We need composite POD data types in many places in project.
glm (or w/e math library of choice) would fit those places, but not everywhere
it makes sense to have all the functionality of math
library. Sometimes all we want is to transfer some data, this is where these
types come in. If in need for maths operations we can use conversions and
utilize math library functionality.
This way we also reduce the coupling of our library code and other libraries.
*/

namespace core
{
namespace pod
{
    template <class T>
    struct Vec2 {
        union {
            struct {
                T x, y;
            };
            struct {
                T u, v;
            };
            T val[2];
        };
    };

    template <class T>
    struct Vec3 {
        union {
            struct {
                T x, y, z;
            };
            struct {
                T r, g, b;
            };
            T val[3];
        };
    };

    template <class T>
    struct Vec4 {
        union {
            struct {
                T x, y, z, w;
            };
            struct {
                T r, g, b, a;
            };
            T val[4];
        };
    };
}
}

#endif