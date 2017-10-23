#include "db/skiplist.h"
#include <set>
#include "qhleveldb/env.h"
#include "util/testharness.h"

int main(int argc, char** argv) {
  return qhleveldb::test::RunAllTests();
}
