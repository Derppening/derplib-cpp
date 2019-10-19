#include <iostream>

namespace derplib {
namespace util {

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
            Args&& ... args) {
  ptr = new T(args...);
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

}  // namespace util
}  // namespace derplib
