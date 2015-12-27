#ifndef FORWARD_DECLARES_H
#define FORWARD_DECLARES_H

/*
*	All core classes should be forward declared, aliased in this file,
*		which is included as 'precomp' header in each cpp.
*/

/* ===== Forward declares  ===== */
namespace io
{
class Path;
class IFileReader;
}

/* ===== Aliases  ===== */
namespace core
{
using TByteArray = std::vector<uint8_t>;
template <class T>
using Vector = std::vector<T>;
template <class T>
using SharedPtr = std::shared_ptr<T>;
template <class T>
using WeakPtr = std::weak_ptr<T>;
}

#endif
