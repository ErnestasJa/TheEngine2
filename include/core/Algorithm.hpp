#ifndef ENGINE_ALGORITHM_H
#define ENGINE_ALGORITHM_H
#include <algorithm>

namespace core {
namespace alg {
template <class ContainerType, class Predicate>
auto find_if(const ContainerType& container, const Predicate& pred)
{
    return std::find_if(container.begin(), container.end(), pred);
}
}
}

#endif