#pragma once

#include <memory>
#include <type_traits>

#include "derplib/stdext/version.h"

namespace derplib {
namespace stdext {

#if defined(DERPLIB_HAS_LIB_MAKE_UNIQUE)
using std::make_unique;
#else
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args);

template<typename T>
std::unique_ptr<T> make_unique(std::size_t size);

template<typename T, typename... Args>
typename std::enable_if<std::is_array<T>::value>::type make_unique(Args&&... args) = delete;
#endif  // defined(DERPLIB_HAS_LIB_MAKE_UNIQUE)

}  // namespace stdext
}  // namespace derplib

#include "derplib/stdext/memory.ipp"
