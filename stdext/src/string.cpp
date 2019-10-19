#include "derplib/stdext/string.h"

#include <cctype>
#include <algorithm>

namespace derplib {
inline namespace stdext {

namespace {
struct ToUpperUnaryOp {
  unsigned char operator()(const unsigned char& c) { return static_cast<unsigned char>(std::toupper(c)); }
};

struct ToLowerUnaryOp {
  unsigned char operator()(const unsigned char& c) { return static_cast<unsigned char>(std::tolower(c)); }
};
}  // namespace

std::string& toupper(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), ToUpperUnaryOp());

  return str;
}

std::string& tolower(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), ToLowerUnaryOp());

  return str;
}

}  // namespace stdext
}  // namespace derplib
