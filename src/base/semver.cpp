#include "derplib/base/semver.h"

#include <algorithm>

namespace derplib {
namespace {
inline bool is_valid_identifier(const char c) {
  return std::isalpha(c) || std::isdigit(c) || c == '-' || c == '.';
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
    _major(major), _minor(minor), _patch(patch), _prerelease(std::move(prerelease)), _build(std::move(build)) {}

semver::operator std::string() const {
  std::string s = std::to_string(_major) + "." + std::to_string(_minor) + "." + std::to_string(_patch);
  if (!_prerelease.empty()) {
    s += ("-" + _prerelease);
  }
  if (!_build.empty()) {
    s += ("+" + _build);
  }

  return s;
}

bool semver::operator<(const semver& other) const {
  if (_major < other._major) {
    return true;
  }
  if (_major > other._major) {
    return false;
  }

  if (_minor < other._minor) {
    return true;
  }
  if (_minor > other._minor) {
    return false;
  }

  if (_patch < other._patch) {
    return true;
  }
  if (_patch > other._patch) {
    return false;
  }

  if (!_prerelease.empty() && other._prerelease.empty()) {
    return true;
  }
  if (_prerelease.empty() && !other._prerelease.empty()) {
    return false;
  }
  return _prerelease < other._prerelease;
}

bool semver::operator==(const semver& other) const {
  return _major == other._major && _minor == other._minor && _patch == other._patch
      && _prerelease == other._prerelease;
}

semver semver::from_string(const std::string& str) {
  std::vector<std::string> metadatas = util::split_string(str, '+');
  if (metadatas.size() > 2 || metadatas.empty()) {
    throw format_error("Malformed SemVer label");
  }
  if (metadatas.size() == 2 && !std::all_of(metadatas.back().begin(), metadatas.back().end(), [](const char c) {
    return is_valid_identifier(c);
  })) {
    throw format_error("Build metadata identifier contains invalid characters");
  }
  std::string build = metadatas.size() == 2 ? metadatas.back() : "";

  std::vector<std::string> prereleases = util::split_string(metadatas.front(), '-');
  if (prereleases.size() > 2) {
    auto s = util::join_to_string<std::string>(prereleases.begin() + 1, prereleases.end(), '-');
    prereleases[1] = s;
    prereleases.erase(prereleases.begin() + 2, prereleases.end());
  }
  if (prereleases.size() > 2 || prereleases.empty()) {
    throw format_error("Malformed prerelease section");
  }
  if (prereleases.size() == 2 && !std::all_of(metadatas.back().begin(), metadatas.back().end(), [](const char c) {
    return is_valid_identifier(c);
  })) {
    throw format_error("Prerelease identifier contains invalid characters");
  }
  if (prereleases.size() == 2 && std::isdigit(prereleases.back().front())) {
    throw format_error("Prerelease version cannot start with a digit");
  }
  std::string prerelease = prereleases.size() == 2 ? prereleases.back() : "";

  std::vector<std::string> versions = util::split_string(prereleases.front(), '.');
  if (versions.size() != 3) {
    throw format_error("Release section does not contain 3 fields");
  }
  if (!std::all_of(versions.begin(), versions.end(), [](const std::string& rstr) {
    return std::all_of(rstr.begin(), rstr.end(), [](const char c) {
      return std::isdigit(c);
    });
  })) {
    throw format_error("Release section contains non-digit characters");
  }

  return semver(std::stoi(versions[0]), std::stoi(versions[1]), std::stoi(versions[2]), prerelease, build);
}
}  // namespace derplib
