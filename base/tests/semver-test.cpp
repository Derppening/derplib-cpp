#include <gtest/gtest.h>

#include <derplib/base/semver.h>

namespace {
using derplib::semver;

TEST(SemVerTest, ObjEquality) {
  // major version
  EXPECT_EQ((semver{1, 0, 0}), (semver{1, 0, 0}));
  EXPECT_EQ((semver{1, 0, 0}), semver::from_string("1.0.0"));

  // minor version
  EXPECT_EQ((semver{0, 1, 0}), (semver{0, 1, 0}));
  EXPECT_EQ((semver{0, 1, 0}), semver::from_string("0.1.0"));

  // patch version
  EXPECT_EQ((semver{0, 0, 1}), (semver{0, 0, 1}));
  EXPECT_EQ((semver{0, 0, 1}), semver::from_string("0.0.1"));

  // pre-release versions
  EXPECT_EQ((semver{1, 0, 0, "rc01"}), (semver{1, 0, 0, "rc01"}));
  EXPECT_EQ((semver{1, 0, 0, "rc01"}), semver::from_string("1.0.0-rc01"));

  // build labels
  EXPECT_EQ((semver{1, 0, 0, "", "abc"}), semver::from_string("1.0.0+abc"));
}
}  // namespace
