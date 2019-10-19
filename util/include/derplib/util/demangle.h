// Utilities for demangling.

#pragma once

#include <string>
#include <cxxabi.h>

namespace derplib {
inline namespace util {

#include <derplib/internal/common_macros_begin.h>

/**
 * Returns the demangled name of a type.
 *
 * Code based on https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
 *
 * \tparam T Type to retrieve demangled name. For C++11 or above, you may use \c decltype(v) to retrieve the type of a
 * variable.
 * \param legacy_naming return name as specified by C++98 standard, i.e. templates with ">>" tokens are displayed as
 * "> >".
 * \return Demangled name
 */
template<typename T>
std::string type_name(bool legacy_naming = false);

/**
 * Returns the demangled name of the type of a variable.
 *
 * This has been tested on both GCC and Clang compilers.
 *
 * Code based on https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
 *
 * \tparam T Type of the variable
 * \param var Variable to retrieve demangled type name
 * \param legacy_naming return name as specified by C++98 standard, i.e. templates with ">>" tokens are displayed as
 * "> >".
 * \return Demangled name of variable type
 */
template<typename T>
std::string type_name(DERPLIB_MAYBE_UNUSED const T& var, bool legacy_naming = false) {
  return type_name<T>(legacy_naming);
}

template<typename T>
std::string type_name(bool legacy_naming) {
  int status;
  std::string tname;
  char* const demangled_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
  if (status == 0) {
    tname = demangled_name;
  }
  if (demangled_name != nullptr) {
    std::free(demangled_name);
  }

  if (!legacy_naming) {
    std::size_t pos;
    while ((pos = tname.find("> >")) != std::string::npos) {
      tname.replace(pos, 3, ">>");
    }
  }

  return tname;
}

#include <derplib/internal/common_macros_end.h>

}  // namespace util
}  // namespace derplib
