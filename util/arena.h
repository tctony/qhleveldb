#ifndef QHLEVELDB_UTIL_ARENA_H_
#define QHLEVELDB_UTIL_ARENA_H_

#include <vector>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "port/port.h"

namespace qhleveldb {

class Arena {
 public:
  Arena();
  ~Arena();

  char* Allocate(size_t types);

  char* AllocateAligned(size_t bytes);

  size_t MemoryUsage() const {
    return reinterpret_cast<uintptr_t>(memory_usage_.NoBarrier_Load());
  }

 private:
  char* alloc_ptr_;
  size_t alloc_bytes_remaining_;

  std::vector<char*> blocks_;

  port::AtomicPointer memory_usage_;

  char* AllocateFallback(size_t bytes);
  char* AllocateNewBlock(size_t block_bytes);

  // No copying allowed
  Arena(const Arena&);
  void operator=(const Arena&);
};

inline char* Arena::Allocate(size_t bytes) {
  assert(bytes > 0);
  if (bytes <= alloc_bytes_remaining_) {
    char* result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
  }
  return AllocateFallback(bytes);
}

}  // qhleveldb

#endif /* QHLEVELDB_UTIL_ARENA_H_ */
