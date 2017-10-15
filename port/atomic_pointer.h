#ifndef QHLEVELDB_PORT_ATOMIC_POINTER_H_
#define QHLEVELDB_PORT_ATOMIC_POINTER_H_

#include <atomic>

namespace qhleveldb {
namespace port {

inline void MemoryBarrier() {
  std::atomic_thread_fence(std::memory_order_seq_cst);
}

class AtomicPointer {
 private:
  void* rep_;
 public:
  AtomicPointer() { }
  explicit AtomicPointer(void *p) : rep_(p) { }
  inline void* NoBarrier_Load() const { return rep_; }
  inline void NoBarrier_Store(void* v) { rep_ = v; }
  inline void* Acquire_Load() const {
    void* result = rep_;
    MemoryBarrier();
    return result;
  }
  inline void Acquire_Store(void* v) {
    MemoryBarrier();
    rep_ = v;
  }
};

}  // port


}  // qhleveldb


#endif /* QHLEVELDB_PORT_ATOMIC_POINTER_H_ */
