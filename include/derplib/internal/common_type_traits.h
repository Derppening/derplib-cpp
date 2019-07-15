#pragma once

// TODO(Derppening): Move to derplib::stdext library as type_traits.h

#include <derplib/stdext/version.h>

namespace derplib {
namespace internal {

#if defined(DERPLIB_HAS_LIB_IS_SWAPPABLE)
template<typename T>
using is_nothrow_swappable = typename std::is_nothrow_swappable<T>;
#else
template<typename T>
using is_nothrow_swappable = std::false_type;
#endif  // defined(DERPLIB_HAS_LIB_IS_SWAPPABLE)

#if defined(DERPLIB_HAS_LIB_TYPE_TRAIT_VARIABLE_TEMPLATES)
#define DERPLIB_IS_ARITHMETIC_V(T) std::is_arithmetic_v<T>
#else
#define DERPLIB_IS_ARITHMETIC_V(T) std::is_arithmetic<T>::value
#endif  // __cplusplus > 201402L

#if defined(DERPLIB_HAS_LIB_TYPE_TRAIT_VARIABLE_TEMPLATES)
template<typename T>
using decay_t = std::decay_t<T>;
using std::enable_if_t;
#else
template<typename T>
using decay_t = typename std::decay<T>::type;
template<bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;
#endif  // defined(DERPLIB_HAS_LIB_TYPE_TRAIT_VARIABLE_TEMPLATES)

#if defined(DERPLIB_HAS_LIB_IS_INVOCABLE)
template<typename Fn, typename... ArgTypes>
using enable_if_invocable = typename std::enable_if<std::is_invocable<Fn, ArgTypes...>::value>;
#else
template<typename Fn, typename... ArgTypes>
using enable_if_invocable = std::true_type;
#endif  // defined(DERPLIB_HAS_LIB_IS_INVOCABLE)

}  // namespace internal
}  // namespace derplib
