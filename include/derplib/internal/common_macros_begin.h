#include "derplib/stdext/version.h"

#if !defined(DERPLIB_USING_COMMON_MACROS)
#define DERPLIB_USING_COMMON_MACROS

#if defined(DERPLIB_HAS_CPP14_SUPPORT)
#define DERPLIB_CPP14_CONSTEXPR constexpr
#else
#define DERPLIB_CPP14_CONSTEXPR
#endif  // DERPLIB_CPP14_CONSTEXPR

#if defined(DERPLIB_HAS_CPP17_SUPPORT)
#define DERPLIB_MAYBE_UNUSED [[maybe_unused]]
#elif defined(__clang__) || defined(__GNUG__)
#define DERPLIB_MAYBE_UNUSED __attribute__((unused))
#endif  // DERPLIB_MAYBE_UNUSED

#if defined(DERPLIB_HAS_CPP17_SUPPORT)
#define DERPLIB_NODISCARD [[nodiscard]]
#elif defined(__clang__) || defined(__GNUG__)
#define DERPLIB_NODISCARD __attribute__((warn_unused_result))
#endif  // DERPLIB_NODISCARD

#endif  // !defined(DERPLIB_USING_COMMON_MACROS)
