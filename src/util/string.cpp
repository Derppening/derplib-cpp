#include "derplib/util/string.h"

#include <cctype>
#include <algorithm>

namespace derplib {
namespace util {

namespace {
struct ToUpperUnaryOp {
  unsigned char operator()(const unsigned char& c) {
    return static_cast<unsigned char>(std::toupper(c));
  }
};

struct ToLowerUnaryOp {
  unsigned char operator()(const unsigned char& c) {
    return static_cast<unsigned char>(std::tolower(c));
  }
};
}

std::string& toupper(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), ToUpperUnaryOp());

  return str;
}

std::string& tolower(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), ToLowerUnaryOp());

  return str;
}

}  // namespace util
}  // namespace derplib
