#ifndef QHLEVELDB_INCLUDE_ENV_H_
#define QHLEVELDB_INCLUDE_ENV_H_

#include "qhleveldb/export.h"
#include "qhleveldb/status.h"

namespace qhleveldb {

class QHLEVELDB_EXPORT Env {
 public:
  Env() { };
  virtual ~Env();

  static Env* Default();

  virtual Status CreateDir(const std::string& dirname) = 0;

  virtual Status GetTestDirectory(std::string* path) = 0;

private:
  Env(const Env&);
  void operator=(const Env&);
};

class QHLEVELDB_EXPORT EnvWrapper : public Env {
 public:
  explicit EnvWrapper(Env* t) : target_(t) { }
  virtual ~EnvWrapper();

  Env* target() const { return target_; }

  virtual Status CreateDir(const std::string& dirname) {
    return target_->CreateDir(dirname);
  }

  virtual Status GetTestDirectory(std::string* path) {
    return target_->GetTestDirectory(path);
  }
 private:
  Env* target_;
};

}  // qhleveldb

#endif /* QHLEVELDB_INCLUDE_ENV_H_ */
