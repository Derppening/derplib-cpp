#include "derplib/stdext/version.h"

#if !defined(DERPLIB_USING_COMMON_MACROS)
#define DERPLIB_USING_COMMON_MACROS

#if defined(DERPLIB_HAS_CONSTEXPR_CPP14)
#define DERPLIB_CPP14_CONSTEXPR constexpr
#else
#define DERPLIB_CPP14_CONSTEXPR
#endif  // DERPLIB_CPP14_CONSTEXPR

#if defined(DERPLIB_HAS_CONSTEXPR_CPP17)
#define DERPLIB_CPP17_CONSTEXPR constexpr
#else
#define DERPLIB_CPP17_CONSTEXPR
#endif  // DERPLIB_CPP17_CONSTEXPR

#if defined(DERPLIB_HAS_ATTRIBUTE_MAYBE_UNUSED)
#define DERPLIB_MAYBE_UNUSED [[maybe_unused]]
#elif defined(__clang__) || defined(__GNUG__)
#define DERPLIB_MAYBE_UNUSED __attribute__((unused))
#endif  // DERPLIB_MAYBE_UNUSED

#if defined(DERPLIB_HAS_ATTRIBUTE_NODISCARD)
#define DERPLIB_NODISCARD [[nodiscard]]
#elif defined(__clang__) || defined(__GNUG__)
#define DERPLIB_NODISCARD __attribute__((warn_unused_result))
#endif  // DERPLIB_NODISCARD

#endif  // !defined(DERPLIB_USING_COMMON_MACROS)
