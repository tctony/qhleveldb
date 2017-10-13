#include "util/testharness.h"

#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

#include "qhleveldb/env.h"
#include "qhleveldb/status.h"

namespace qhleveldb {
namespace test {

namespace {
struct TestCase {
  const char* base;
  const char* name;
  void (*func)();
};
std::vector<TestCase>* tests;
}

bool RegisterTest(const char* base, const char* name, void (*func)()) {
  if (tests == NULL) {
    tests = new std::vector<TestCase>;
  }
  TestCase tc;
  tc.base = base;
  tc.name = name;
  tc.func = func;
  tests->push_back(tc);
  return true;
}

int RunAllTests() {
  const char* matcher = getenv("QHLEVELDB_TESTS");

  int num = 0;
  if (tests != NULL) {
    for (size_t i = 0; i < tests->size(); ++i) {
      const TestCase& t = (*tests)[i];
      if (matcher != NULL) {
        std::string name = t.base;
        name.push_back('.');
        name.append(t.name);
        if (strstr(name.c_str(), matcher) == NULL) {
          continue;
        }
      }
      fprintf(stderr, "==== Test %s.%s\n", t.base, t.name);
      (*t.func)();
      ++num;
    }
  }
  fprintf(stderr, "==== PASSED %d tests\n", num);
  return 0;
}

std::string TmpDir() {
  std::string dir;
  Status s = Env::Default()->GetTestDirectory(&dir);
  ASSERT_TRUE(s.ok()) << s.ToString();
  return dir;
}

int RandomSeed() {
  const char* env = getenv("TEST_RANDOM_SEED");
  int result = (env != NULL ? atoi(env) : 301);
  if (result < 0) {
    result = 301;
  }
  return result;
}

}  // test
}  // qhleveldb
