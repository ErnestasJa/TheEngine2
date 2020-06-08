#ifndef ENGINE_MATH_UTILS_H
#define ENGINE_MATH_UTILS_H

#include "glm/gtx/norm.hpp"

namespace utils::math {

inline static bool CheckBit(uint8_t b, uint8_t b2)
{
  return (b & b2) == b2;
}

inline static bool equal(float a, float b)
{
  return glm::abs(a - b) <= 0.00001f;
}

inline static bool lequal(float a, float b)
{
  return a < b || equal(a, b);
}

inline static bool gequal(float a, float b)
{
  return a > b || equal(a, b);
}

inline static bool less(float a, float b)
{
  return a < b && !equal(a, b);
}

inline static bool greater(float a, float b)
{
  return a > b && !equal(a, b);
}
} // namespace utils::math

#endif