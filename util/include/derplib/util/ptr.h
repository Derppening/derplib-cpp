#pragma once

#include <iostream>

#define DERPLIB_PTR_ASSERT_NULL(ptr) derplib::util::ptr_assert_null((ptr), __FILE__, __func__, __LINE__, #ptr)

#define DERPLIB_PTR_NEW(type, ptr) derplib::util::ptr_new<type>((ptr), __FILE__, __func__, __LINE__, #type, #ptr)
#define DERPLIB_PTR_NEW_ARGS(type, ptr, ...) derplib::util::ptr_new<type>((ptr), __FILE__, __func__, __LINE__, #type, #ptr, __VA_ARGS__)
#define DERPLIB_PTR_DELETE(ptr) derplib::util::ptr_delete((ptr), __FILE__, __func__, __LINE__, #ptr)

#define DERPLIB_PTR_ARRAY_NEW(type, ptr, num_of_elements) derplib::util::ptr_array_new<type>((ptr), __FILE__, __func__, __LINE__, #type, #ptr, num_of_elements)
#define DERPLIB_PTR_ARRAY_DELETE(ptr) derplib::util::ptr_array_delete((ptr), __FILE__, __func__, __LINE__, #ptr)

namespace derplib {
namespace util {

/**
 * \brief Type definition for pointers.
 */
template<typename T>
using raw_ptr = T*;

/**
 * \brief Checks whether a pointer is `nullptr` during runtime.
 *
 * It is recommended to use `DERPLIB_PTR_ASSERT_NULL` to wrap all required arguments.
 *
 * \param ptr pointer to check
 * \param file `__FILE__`
 * \param func `__func__`
 * \param line `__LINE__`
 * \param var variable name
 */
inline void ptr_assert_null(void* ptr, const char* file, const char* func, std::size_t line, const char* var);

/**
 * \brief Wrapper for `new` operator in C++, logging the pointer information.
 *
 * It is recommended to use `DERPLIB_PTR_NEW` and `DERPLIB_PTR_NEW_ARGS` to wrap all required arguments.
 *
 * \tparam T type of pointer
 * \tparam Args type arguments to constructor of `T`
 * \param ptr pointer to `new` into
 * \param file `__FILE__`
 * \param func `__func__`
 * \param line `__LINE__`
 * \param type type name of the variable
 * \param var variable name
 * \param args arguments to pass to the constructor of `T`
 * \return `ptr`
 */
template<typename T, typename... Args>
inline T*& ptr_new(T*& ptr,
                   const char* file,
                   const char* func,
                   std::size_t line,
                   const char* type,
                   const char* var,
                   Args&& ... args);

/**
 * \brief Wrapper for `delete` operator in C++, logging the pointer information.
 *
 * It is recommended to use `DEERPLIB_PTR_DELETE` to wrap all required arguments.
 *
 * \tparam T type of pointer
 * \param ptr pointer to `delete`
 * \param file `__FILE__`
 * \param func `__func__`
 * \param line `__LINE__`
 * \param var variable name
 * \return `ptr`
 */
template<typename T>
inline T*& ptr_delete(T*& ptr, const char* file, const char* func, std::size_t line, const char* var);

/**
 * \brief Wrapper for `new[]` operator in C++, logging the pointer information.
 *
 * It is recommended to use `DERPLIB_PTR_ARRAY_NEW` to wrap all required arguments.
 *
 * \tparam T type of pointer
 * \param ptr pointer to `new[]` into
 * \param file `__FILE__`
 * \param func `__func__`
 * \param line `__LINE__`
 * \param type type name of the variable
 * \param var variable name
 * \param size size of array to allocate
 * \return `ptr`
 */
template<typename T>
inline T*& ptr_array_new(T*& ptr,
                         const char* file,
                         const char* func,
                         std::size_t line,
                         const char* type,
                         const char* var,
                         std::size_t size);

/**
 * \brief Wrapper for `delete[]` operator in C++, logging the pointer information.
 *
 * It is recommended to use `DEERPLIB_PTR_ARRAY_DELETE` to wrap all required arguments.
 *
 * \tparam T type of pointer
 * \param ptr pointer to `delete[]`
 * \param file `__FILE__`
 * \param func `__func__`
 * \param line `__LINE__`
 * \param var variable name
 * \return `ptr`
 */
template<typename T>
inline T*& ptr_array_delete(T*& ptr, const char* file, const char* func, std::size_t line, const char* var);

}  // namespace util
}  // namespace derplib

#include "ptr.ipp"
