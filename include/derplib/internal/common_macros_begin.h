#if !defined(DERPLIB_USING_COMMON_MACROS)
#define DERPLIB_USING_COMMON_MACROS

#if __cplusplus > 199711L
#define DERPLIB_NOEXCEPT noexcept
#else
#define DERPLIB_NOEXCEPT throw()
#endif  // DERPLIB_NOEXCEPT

#if __cplusplus > 199711L
#define DERPLIB_CPP11_CONSTEXPR constexpr
#else
#define DERPLIB_CPP11_CONSTEXPR
#endif  // DERPLIB_CPP11_CONSTEXPR

#if __cplusplus > 201103L
#define DERPLIB_CPP14_CONSTEXPR constexpr
#else
#define DERPLIB_CPP14_CONSTEXPR
#endif  // DERPLIB_CPP14_CONSTEXPR

#if __cplusplus > 201402L
#define DERPLIB_MAYBE_UNUSED [[maybe_unused]]
#elif defined(__clang__) || defined(__GNUG__)
#define DERPLIB_MAYBE_UNUSED __attribute__((unused))
#endif  // DERPLIB_MAYBE_UNUSED

#if __cplusplus > 201402L
#define DERPLIB_NODISCARD [[nodiscard]]
#elif defined(__clang__) || defined(__GNUG__)
#define DERPLIB_NODISCARD __attribute__((warn_unused_result))
#endif  // DERPLIB_NODISCARD

#endif  // !defined(DERPLIB_USING_COMMON_MACROS)
