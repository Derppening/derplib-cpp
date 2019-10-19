#include <gtest/gtest.h>

#include <derplib/base/log.h>

namespace {
using derplib::logger;

TEST(LogTest, OutputError) {
  std::ostringstream oss;

  logger l(oss, logger::level::error);

  l.v("Verbose");
  ASSERT_TRUE(oss.str().empty());

  l.d("Debug");
  ASSERT_TRUE(oss.str().empty());

  l.i("Info");
  ASSERT_TRUE(oss.str().empty());

  l.w("Warning");
  ASSERT_TRUE(oss.str().empty());

  l.e("Error");

  std::string oss_str = oss.str();
  ASSERT_FALSE(oss_str.empty());
  ASSERT_TRUE(oss_str.find("Error") != std::string::npos);
}

}  // namespace
