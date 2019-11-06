#include "node.hpp"

namespace std {
template <> struct hash<node> {
  using argument_type = coord;
  using result_type = size_t;
  /*5. The meat of this struct is the operator() definition. We are just
  adding the numeric member values of struct coord, which is a poor
  hashing technique, but for the sake of showing how to implement it, it's
  good enough. A good hash function tries to distribute values as evenly
  over the whole value range as possible, in order to reduce the amount of
  hash collisions.*/

  result_type operator()(const argument_type &c) const {
    // sip_hash
    return static_cast<result_type>(c.x) + static_cast<result_type>(c.y);
  }
};
} // namespace std