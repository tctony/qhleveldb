#include <assert.h>

#include "util/arena.h"

namespace qhleveldb {

static const int kBlockSize = 4096;

Arena::Arena() : memory_usage_(0) {
  alloc_ptr_ = NULL;
  alloc_bytes_remaining_ = 0;
}

Arena::~Arena() {
  for (size_t i = 0; i < blocks_.size(); i++) {
    delete[] blocks_[i];
  }
}

char* Arena::AllocateFallback(size_t bytes) {
  if (bytes > kBlockSize / 4) {
    char* result = AllocateNewBlock(bytes);
    return result;
  }

  alloc_ptr_ = AllocateNewBlock(kBlockSize);
  alloc_bytes_remaining_ = kBlockSize;

  char* result = alloc_ptr_;
  alloc_ptr_ += bytes;
  alloc_bytes_remaining_ -= bytes;
  return result;
}

char* Arena::AllocateAligned(size_t bytes) {
  const int align = (sizeof(void*) > 8) ? sizeof(void*) : 8;
  assert((align & (align -1)) == 0); // pointer size should be a power  of 2
  size_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr_) & (align - 1);
  size_t forward = (current_mod == 0 ? 0 : align - current_mod);
  size_t needed = forward + bytes;
  char* result;
  if (needed <= alloc_bytes_remaining_) {
    result = alloc_ptr_ + forward;
    alloc_ptr_ += needed;
    alloc_bytes_remaining_ -= needed;
  } else {
    result = AllocateFallback(bytes);
  }
  assert((reinterpret_cast<uintptr_t>(result) & (align-1)) == 0);
  return result;
}

char* Arena::AllocateNewBlock(size_t block_bytes) {
  char* result = new char[block_bytes];
  blocks_.push_back(result);
  memory_usage_.NoBarrier_Store(
      reinterpret_cast<void*>(MemoryUsage() + block_bytes + sizeof(char*)));
  return result;
}

}  // qhleveldb
