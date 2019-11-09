#include "node.hpp"

namespace std {
template <typename T> struct hash<redisgraph::node<T>> {
  using argument_type = redisgraph::node<T>>;
  using result_type = uint64_t;
  /*5. The meat of this struct is the operator() definition. We are just
  adding the numeric member values of struct coord, which is a poor
  hashing technique, but for the sake of showing how to implement it, it's
  good enough. A good hash function tries to distribute values as evenly
  over the whole value range as possible, in order to reduce the amount of
  hash collisions.*/

  result_type operator()(const argument_type &c) const {
    // sip_hash
    return c.id();
  }
};
template <> struct hash<redisgraph::node<std::string>> {
  using argument_type = redisgraph::node<std::string>;
  using result_type = uint64_t;
  /*5. The meat of this struct is the operator() definition. We are just
  adding the numeric member values of struct coord, which is a poor
  hashing technique, but for the sake of showing how to implement it, it's
  good enough. A good hash function tries to distribute values as evenly
  over the whole value range as possible, in order to reduce the amount of
  hash collisions.*/

  result_type operator()(const argument_type &c) const {
    // sip_hash
    return c.id();
  }
};

} // namespace std