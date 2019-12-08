#ifndef _NAMEPAIR_H
#define _NAMEPAIR_H
#include <string>
namespace redisgraph {
template <typename T> class namepair {
public:
  namepair(const namepair &operator) = default;
  namepair &operator=(const namepair &operator) = default;
  std::string name() const { return name_; }
  T value() const { return value_; }

private:
  std::string name_;
  T value_;
};
} // namespace redisgraph
#endif