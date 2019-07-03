#pragma once

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "derplib/util/string.h"

namespace derplib {

#include "derplib/internal/common_macros_begin.h"

class semver {
 public:
  class format_error final : public std::runtime_error {
   public:
    explicit format_error(const char* what_arg);
    explicit format_error(const std::string& what_arg);

    format_error(const format_error&);
    format_error(format_error&&) noexcept;

    format_error& operator=(const format_error&);
    format_error& operator=(format_error&&) noexcept;

    ~format_error() final;
  };

  /**
   * \brief Constructs a `semver` object using version information.
   *
   * \param major major version number.
   * \param minor minor version number.
   * \param patch patch version number.
   * \param prerelease prerelease string.
   * \param build build tag.
   */
  explicit semver(int major, int minor = 0, int patch = 0, std::string prerelease = "", std::string build = "");

  semver(const semver& other) = default;
  semver(semver&& other) noexcept = default;

  semver& operator=(const semver& other) = default;
  semver& operator=(semver&& other) noexcept = default;

  /**
   * \return `std::string` representation of the version.
   */
  DERPLIB_NODISCARD explicit operator std::string() const;

  DERPLIB_NODISCARD int major() const { return _major; }
  DERPLIB_NODISCARD int minor() const { return _minor; }
  DERPLIB_NODISCARD int patch() const { return _patch; }
  DERPLIB_NODISCARD const std::string& prerelease() const { return _prerelease; }
  DERPLIB_NODISCARD const std::string& build() const { return _build; }

  bool operator<(const semver& other) const;
  bool operator==(const semver& other) const;
  bool operator!=(const semver& other) const { return !(*this == other); }
  bool operator<=(const semver& other) const { return *this < other || *this == other; }
  bool operator>(const semver& other) const { return !(*this <= other); }
  bool operator>=(const semver& other) const { return !(*this < other); }

  /**
   * \brief Constructs a `semver` object from a version string.
   *
   * \param str version string to convert from
   * \return `semver` object representation of the version.
   * \throw `format_error` if `str` is a malformed version string
   */
  DERPLIB_NODISCARD static semver from_string(const std::string& str);

 private:
  int _major;
  int _minor;
  int _patch;
  std::string _prerelease;
  std::string _build;
};

#include "derplib/internal/common_macros_end.h"

}  // namespace derplib
