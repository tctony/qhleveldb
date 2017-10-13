#include "qhleveldb/env.h"
#include "util/testharness.h"

namespace qhleveldb {

class EnvTest {
 public:
  Env* env_;
  EnvTest() : env_(Env::Default()) {}
};

TEST(EnvTest, TestOpenNonExistentFile) {
  std::string test_dir;

  ASSERT_OK(env_->GetTestDirectory(&test_dir));
}

}  // qhleveldb

int main(int argc, char** argv) {
  return qhleveldb::test::RunAllTests();
}
