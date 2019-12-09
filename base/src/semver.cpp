#include "derplib/base/semver.h"

#include <algorithm>
#include <array>
#include <sstream>

namespace derplib {
inline namespace base {
namespace {
/**
 * \return Whether `c` is a valid identifier character as defined in the SemVer standard.
 */
inline bool _is_valid_identifier(const char c) noexcept {
  const auto uc{static_cast<unsigned char>(c)};
  return std::isalpha(uc) || std::isdigit(uc) || uc == '-' || uc == '.';
}

/**
 * \brief Parses the build identifier.
 *
 * \param tokens tokens as split by the character '+'.
 * \return String of the build identifier, or an empty string if none was found.
 * \throws semver::format_error if the SemVer label is malformed.
 */
std::string _parse_build(const std::vector<std::string>& tokens) {
  if (tokens.size() > 2 || tokens.empty()) {
    throw semver::format_error{"Malformed SemVer label"};
  }

  if (tokens.size() == 1) {
    return "";
  }

  const std::string& build{tokens.back()};
  if (!std::all_of(build.begin(), build.end(), _is_valid_identifier)) {
    throw semver::format_error{"Build metadata identifier contains invalid characters"};
  }
  return build;
}

/**
 * \brief Parses the prerelease identifier.
 *
 * \param tokens tokens as split by the character '-`, after the build identifier is trimmed.
 * \return String of the prerelease identifier, or an empty string if none was found.
 * \throws semver::format_error if the SemVer label is malformed.
 */
std::string _parse_prerelease(const std::vector<std::string>& tokens) {
  if (tokens.empty()) {
    throw semver::format_error{"Malformed SemVer label"};
  }

  if (tokens.size() == 1) {
    return "";
  }

  const auto prerelease{stdext::join_to_string<std::string>(tokens.begin() + 1, tokens.end(), '-')};
  if (!std::all_of(prerelease.begin(), prerelease.end(), _is_valid_identifier)) {
    throw semver::format_error{"Prerelease identifier contains invalid characters"};
  }
  return prerelease;
}

/**
 * \brief Parses the version identifiers.
 *
 * \param version_string the version string with the prerelease and build identifier trimmed.
 * \return Array of version integers, sequenced as MAJOR, MINOR, PATCH.
 * \throws semver::format_error if the SemVer label is malformed.
 */
std::array<int, 3> _parse_version(const std::string& version_string) {
  const auto is_digit_string{[](const std::string& s) {
    const auto is_digit_char{[](const unsigned char c) { return std::isdigit(c); }};

    return std::all_of(s.begin(), s.end(), is_digit_char);
  }};

  const std::vector<std::string> versions{stdext::split_string(version_string, '.')};
  if (versions.size() != 3) {
    throw semver::format_error{"Release section does not contain 3 fields"};
  }
  if (!std::all_of(versions.begin(), versions.end(), is_digit_string)) {
    throw semver::format_error{"Release section contains non-digit characters"};
  }

  return {{std::stoi(versions[0]), std::stoi(versions[1]), std::stoi(versions[2])}};
}
}  // namespace

semver::format_error::format_error(const char* what_arg) : runtime_error(what_arg) {}
semver::format_error::format_error(const std::string& what_arg) : runtime_error(what_arg) {}

semver::format_error::format_error(const semver::format_error&) = default;
semver::format_error::format_error(semver::format_error&&) noexcept = default;

semver::format_error& semver::format_error::operator=(const semver::format_error&) = default;
semver::format_error& semver::format_error::operator=(semver::format_error&&) noexcept = default;

semver::format_error::~format_error() = default;

semver::semver(int major, int minor, int patch, std::string prerelease, std::string build) :
    _major_{major}, _minor_{minor}, _patch_{patch}, _prerelease_{std::move(prerelease)}, _build_{std::move(build)} {}

semver::operator std::string() const {
  std::ostringstream oss{};
  oss << _major_ << '.' << _minor_ << '.' << _patch_;

  if (!_prerelease_.empty()) {
    oss << '-' << _prerelease_;
  }
  if (!_build_.empty()) {
    oss << '+' << _build_;
  }

  return oss.str();
}

bool semver::operator<(const semver& other) const noexcept {
  if (_major_ < other._major_) {
    return true;
  }
  if (_major_ > other._major_) {
    return false;
  }

  if (_minor_ < other._minor_) {
    return true;
  }
  if (_minor_ > other._minor_) {
    return false;
  }

  if (_patch_ < other._patch_) {
    return true;
  }
  if (_patch_ > other._patch_) {
    return false;
  }

  if (!_prerelease_.empty() && other._prerelease_.empty()) {
    return true;
  }
  if (_prerelease_.empty() && !other._prerelease_.empty()) {
    return false;
  }
  return _prerelease_ < other._prerelease_;
}

bool semver::operator==(const semver& other) const noexcept {
  return _major_ == other._major_ && _minor_ == other._minor_ && _patch_ == other._patch_
         && _prerelease_ == other._prerelease_;
}

semver semver::from_string(const std::string& str) {
  const std::vector<std::string> build_tokens{stdext::split_string(str, '+')};
  std::string build = _parse_build(build_tokens);

  const std::vector<std::string> prerelease_tokens{stdext::split_string(build_tokens.front(), '-')};
  std::string prerelease = _parse_prerelease(prerelease_tokens);

  const std::array<int, 3> versions = _parse_version(prerelease_tokens.front());

  return semver(versions[0], versions[1], versions[2], prerelease, build);
}
}  // namespace base
}  // namespace derplib
