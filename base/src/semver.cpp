#include "derplib/base/semver.h"

#include <algorithm>

namespace derplib {
inline namespace base {
namespace {
inline bool _is_valid_identifier(const char c) {
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
    _major_(major),
    _minor_(minor),
    _patch_(patch),
    _prerelease_(std::move(prerelease)),
    _build_(std::move(build)) {}

semver::operator std::string() const {
  std::string s = std::to_string(_major_) + "." + std::to_string(_minor_) + "." + std::to_string(_patch_);
  if (!_prerelease_.empty()) {
    s += ("-" + _prerelease_);
  }
  if (!_build_.empty()) {
    s += ("+" + _build_);
  }

  return s;
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
  // TODO: Might have to start writing comments and separating into logical chunks

  std::vector<std::string> metadatas = stdext::split_string(str, '+');
  if (metadatas.size() > 2 || metadatas.empty()) {
    throw format_error("Malformed SemVer label");
  }
  if (metadatas.size() == 2 && !std::all_of(metadatas.back().begin(), metadatas.back().end(), [](const char c) {
        return _is_valid_identifier(c);
      })) {
    throw format_error("Build metadata identifier contains invalid characters");
  }
  std::string build = metadatas.size() == 2 ? metadatas.back() : "";

  std::vector<std::string> prereleases = stdext::split_string(metadatas.front(), '-');
  if (prereleases.size() > 2) {
    auto s = stdext::join_to_string<std::string>(prereleases.begin() + 1, prereleases.end(), '-');
    prereleases[1] = s;
    prereleases.erase(prereleases.begin() + 2, prereleases.end());
  }
  if (prereleases.size() > 2 || prereleases.empty()) {
    throw format_error("Malformed prerelease section");
  }
  if (prereleases.size() == 2 && !std::all_of(metadatas.back().begin(), metadatas.back().end(), [](const char c) {
        return _is_valid_identifier(c);
      })) {
    throw format_error("Prerelease identifier contains invalid characters");
  }
  if (prereleases.size() == 2 && std::isdigit(prereleases.back().front())) {
    throw format_error("Prerelease version cannot start with a digit");
  }
  std::string prerelease = prereleases.size() == 2 ? prereleases.back() : "";

  std::vector<std::string> versions = stdext::split_string(prereleases.front(), '.');
  if (versions.size() != 3) {
    throw format_error("Release section does not contain 3 fields");
  }
  if (!std::all_of(versions.begin(), versions.end(), [](const std::string& rstr) {
        return std::all_of(rstr.begin(), rstr.end(), [](const char c) { return std::isdigit(c); });
      })) {
    throw format_error("Release section contains non-digit characters");
  }

  return semver(std::stoi(versions[0]), std::stoi(versions[1]), std::stoi(versions[2]), prerelease, build);
}
}  // namespace base
}  // namespace derplib
