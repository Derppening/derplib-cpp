#pragma once

#include <iostream>

#define DERPLIB_PTR_ASSERT_NULL(ptr) derplib::stdext::ptr_assert_null((ptr), __FILE__, __func__, __LINE__, #ptr)

#define DERPLIB_PTR_NEW(type, ptr) derplib::stdext::ptr_new<type>((ptr), __FILE__, __func__, __LINE__, #type, #ptr)
#define DERPLIB_PTR_NEW_ARGS(type, ptr, ...) \
  derplib::stdext::ptr_new<type>((ptr), __FILE__, __func__, __LINE__, #type, #ptr, __VA_ARGS__)
#define DERPLIB_PTR_DELETE(ptr) derplib::stdext::ptr_delete((ptr), __FILE__, __func__, __LINE__, #ptr)

#define DERPLIB_PTR_ARRAY_NEW(type, ptr, num_of_elements) \
  derplib::stdext::ptr_array_new<type>((ptr), __FILE__, __func__, __LINE__, #type, #ptr, num_of_elements)
#define DERPLIB_PTR_ARRAY_DELETE(ptr) derplib::stdext::ptr_array_delete((ptr), __FILE__, __func__, __LINE__, #ptr)

namespace derplib {
inline namespace stdext {

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
                   Args&&... args);

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

// TODO(Derppening): Move to .cpp file
void ptr_assert_null(void* ptr, const char* file, const char* func, std::size_t line, const char* var) {
  if (ptr == nullptr) {
    std::cerr << file << ": In function " << func << '\n'
              << file << ":" << line << ": assert: " << var << " is a runtime nullptr" << std::endl;
  }
}

template<typename T, typename... Args>
T*& ptr_new(T*& ptr,
            const char* file,
            const char* func,
            std::size_t line,
            const char* type,
            const char* var,
            Args&&... args) {
  ptr = new T{args...};
  std::cout << file << ": In function " << func << '\n'
            << file << ":" << line << ": new " << type << ": " << var << " " << static_cast<void*>(ptr) << std::endl;
  return ptr;
}

template<typename T>
T*& ptr_delete(T*& ptr, const char* file, const char* func, std::size_t line, const char* var) {
  std::cout << file << ": In function " << func << '\n'
            << file << ":" << line << ": delete: " << var << " " << static_cast<void*>(ptr) << std::endl;
  delete ptr;
  ptr = nullptr;
  return ptr;
}

template<typename T>
T*& ptr_array_new(T*& ptr,
                  const char* file,
                  const char* func,
                  std::size_t line,
                  const char* type,
                  const char* var,
                  std::size_t size) {
  ptr = new T[size];
  std::cout << file << ": In function " << func << '\n'
            << file << ":" << line << ": new " << type << "[" << size << "]: " << var << " " << static_cast<void*>(ptr)
            << std::endl;
  return ptr;
}

template<typename T>
T*& ptr_array_delete(T*& ptr, const char* file, const char* func, std::size_t line, const char* var) {
  std::cout << file << ": In function " << func << '\n'
            << file << ":" << line << ": delete[]: " << var << " " << static_cast<void*>(ptr) << std::endl;
  delete[] ptr;
  ptr = nullptr;
  return ptr;
}

}  // namespace stdext
}  // namespace derplib
