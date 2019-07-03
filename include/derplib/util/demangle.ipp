#include <string>
#include <cxxabi.h>

namespace derplib {
namespace util {

#include "derplib/internal/common_macros_begin.h"

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

template<typename T>
std::string type_name(DERPLIB_MAYBE_UNUSED const T& var, bool legacy_naming) {
  return type_name<T>(legacy_naming);
}

#include "derplib/internal/common_macros_end.h"

}  // namespace util
}  // namespace derplib
