#include "util/arena.h"
#include "util/random.h"
#include "util/testharness.h"

namespace qhleveldb {

class ArenaTest { };

TEST(ArenaTest, Empty) {
  Arena arena;
}

TEST(ArenaTest, Simple) {
   
}

}  // qhleveldb

int main(int argc, char** argv) {
  return qhleveldb::test::RunAllTests();
}
