#include "derplib/stdext/string.h"

#include <cctype>
#include <algorithm>

namespace derplib {
inline namespace stdext {

namespace {
struct _to_upper_unary_op {
  using char_type = std::string::value_type;

  char_type operator()(char_type c) { return static_cast<char_type>(std::toupper(c)); }
};

struct _to_lower_unary_op {
  using char_type = std::string::value_type;

  char_type operator()(char_type c) { return static_cast<char_type>(std::tolower(c)); }
};
}  // namespace

std::string& toupper(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), _to_upper_unary_op());

  return str;
}

std::string& tolower(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), _to_lower_unary_op());

  return str;
}

}  // namespace stdext
}  // namespace derplib
