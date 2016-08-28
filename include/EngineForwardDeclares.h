#ifndef FORWARD_DECLARES_H
#define FORWARD_DECLARES_H

/*
*	All core classes should be forward declared, aliased in this file,
*		which is included as 'precomp' header in each cpp.
*/

/* ===== Forward declares  ===== */
namespace io {
class Path;
class IFileReader;
}

/* ===== Aliases  ===== */
namespace core {
using TByteArray                = std::vector<uint8_t>;
using String                    = std::string;
template <class T> using Vector = std::vector<T>;
template <class T, uint32_t N> using Array = std::array<T, N>;
template <class T> using SharedPtr = std::shared_ptr<T>;
template <class T> using WeakPtr   = std::weak_ptr<T>;
template <class T> using UniquePtr = std::unique_ptr<T>;

template <class T, class... U> SharedPtr<T> MakeShared(U&&... u)
{
    return std::make_shared<T>(std::forward<U>(u)...);
}

template <class T, class... U> UniquePtr<T> MakeUnique(U&&... u)
{
    return std::make_unique<T>(std::forward<U>(u)...);
}

template <class U> inline auto Move(U&& u)
{
    return std::move(u);
}

template <class U, typename Pred> bool Any(const U& container, Pred predicate)
{
    for (const auto& value : container) {
        if (predicate(value) == false)
            return false;
    }
    return true;
}

class InputHandler;
class IInputDevice;
class IInputDeviceModule;
}

#endif
