#pragma once

#if __cplusplus > 201703L
#include <version>
#endif  // __cplusplus > 201703L

#define DERPLIB_CPP11 201103L
#define DERPLIB_CPP14 201402L
#define DERPLIB_CPP17 201703L

#if __cplusplus >= DERPLIB_CPP11
#define DERPLIB_HAS_CPP11_SUPPORT 1
#if __cplusplus == DERPLIB_CPP11
#define DERPLIB_IS_CPP11 1
#endif
#endif

#if __cplusplus >= DERPLIB_CPP14
#define DERPLIB_HAS_CPP14_SUPPORT 1
#if __cplusplus == DERPLIB_CPP14
#define DERPLIB_IS_CPP14 1
#endif
#endif

#if __cplusplus >= DERPLIB_CPP17
#define DERPLIB_HAS_CPP17_SUPPORT 1
#if __cplusplus == DERPLIB_CPP17
#define DERPLIB_IS_CPP17 1
#endif
#endif

// TODO(Derppening): Temporary C++20 checks
#if __cplusplus > DERPLIB_CPP17
#define DERPLIB_HAS_CPP2A_SUPPORT 1
#endif

#if (defined(DERPLIB_HAS_CPP2A_SUPPORT) && __has_cpp_attribute(maybe_unused)) || defined(DERPLIB_HAS_CPP17_SUPPORT)
#define DERPLIB_HAS_ATTRIBUTE_MAYBE_UNUSED 1
#endif

#if (defined(DERPLIB_HAS_CPP2A_SUPPORT) && __has_cpp_attribute(nodiscard)) || defined(DERPLIB_HAS_CPP17_SUPPORT)
#define DERPLIB_HAS_ATTRIBUTE_NODISCARD 1
#endif

#if __cpp_constexpr >= 201304L || defined(DERPLIB_HAS_CPP14_SUPPORT)
#define DERPLIB_HAS_CONSTEXPR_CPP14 1
#endif

#if __cpp_constexpr >= 201603L || defined(DERPLIB_HAS_CPP17_SUPPORT)
#define DERPLIB_HAS_CONSTEXPR_CPP17 1
#endif

#if (defined(DERPLIB_HAS_CPP2A_SUPPORT) && defined(__cpp_lib_is_invocable)) || defined(DERPLIB_HAS_CPP17_SUPPORT)
#define DERPLIB_HAS_LIB_IS_INVOCABLE 1
#endif

#if (defined(DERPLIB_HAS_CPP2A_SUPPORT) && defined(__cpp_lib_is_swappable)) || defined(DERPLIB_HAS_CPP17_SUPPORT)
#define DERPLIB_HAS_LIB_IS_SWAPPABLE 1
#endif

#if (defined(DERPLIB_HAS_CPP2A_SUPPORT) && defined(__cpp_lib_optional)) || defined(DERPLIB_HAS_CPP17_SUPPORT)
#define DERPLIB_HAS_LIB_OPTIONAL 1
#endif

#if (defined(DERPLIB_HAS_CPP2A_SUPPORT) && defined(__cpp_lib_type_trait_variable_templates)) \
    || defined(DERPLIB_HAS_CPP17_SUPPORT)
#define DERPLIB_HAS_LIB_TYPE_TRAIT_VARIABLE_TEMPLATES 1
#endif
