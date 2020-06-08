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

namespace core {
namespace pod {
template <class T> struct Vec2
{
  union
  {
    struct
    {
      T x, y;
    };
    struct
    {
      T u, v;
    };
    struct
    {
      T w, h;
    };
  };

  Vec2()
  {
  }

  Vec2(T _x, T _y)
      : x(_x)
      , y(_y)
  {
  }
};

template <class T> Vec2<T> operator*(const Vec2<T>& first, const Vec2<T>& second)
{
  return Vec2{ first.x * second.x, first.y * second.y };
}

template <class T, class U> Vec2<T> operator*(const Vec2<T>& first, U val)
{
  return Vec2<U>{ static_cast<U>(first.x * val), static_cast<U>(first.y * val) };
}

template <class T> Vec2<T> operator-(const Vec2<T>& first, const Vec2<T>& second)
{
  return Vec2{ first.x - second.x, first.y - second.y };
}

template <class T> Vec2<T> operator+(const Vec2<T>& first, const Vec2<T>& second)
{
  return Vec2{ first.x + second.x, first.y + second.y };
}

template <class T> struct Vec3
{
  union
  {
    struct
    {
      T x, y, z;
    };
    struct
    {
      T r, g, b;
    };
  };

  Vec3()
  {
  }

  Vec3(T _x, T _y, T _z)
      : x(_x)
      , y(_y)
      , z(_z)
  {
  }
};

template <class T> struct Vec4
{
  union
  {
    struct
    {
      T x, y, z, w;
    };
    struct
    {
      T r, g, b, a;
    };
  };

  Vec4()
  {
  }

  Vec4(T _x, T _y, T _z, T _w)
      : x(_x)
      , y(_y)
      , z(_z)
      , w(_w)
  {
  }
};
} // namespace pod
} // namespace core

#endif
