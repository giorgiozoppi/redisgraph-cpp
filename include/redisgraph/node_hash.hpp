/**
* Copyright 2019 RRD Software Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/
#ifndef REDISGRAPH_CPP_NODE_HASH_H
#define REDISGRAPH_CPP_NODE_HASH_H
#include <boost/uuid/uuid.hpp>
#include <picojson/picojson.h>
#include "node.hpp"
#include "siphash.hpp"


namespace std {
template <typename T> struct hash<std::unique_ptr<redisgraph::node<T>>> {
  using argument_type = std::unique_ptr<redisgraph::node<T>>;
  using result_type = uint64_t;
  /*5. The meat of this struct is the operator() definition. We are just
  adding the numeric member values of struct coord, which is a poor
  hashing technique, but for the sake of showing how to implement it, it's
  good enough. A good hash function tries to distribute values as evenly
  over the whole value range as possible, in order to reduce the amount of
  hash collisions.*/

  result_type operator()(const argument_type &c) const {
      // fill uuid_data
      return c->id();
  }
};
template <typename T> struct equal_to<std::unique_ptr<redisgraph::node<T>>> {
    using argument_type = std::unique_ptr<redisgraph::node<T>>;
    /*5. The meat of this struct is the operator() definition. We are just
    adding the numeric member values of struct coord, which is a poor
    hashing technique, but for the sake of showing how to implement it, it's
    good enough. A good hash function tries to distribute values as evenly
    over the whole value range as possible, in order to reduce the amount of
    hash collisions.*/
    bool operator()(const argument_type& lhs, const argument_type& rhs) const {
        // fill uuid_data
        return (lhs->id() ==  rhs->id());
    }
};
template<>
struct std::equal_to<std::unique_ptr<redisgraph::node<picojson::value>>> {
    using item_type = std::unique_ptr<redisgraph::node<picojson::value>>;
    bool operator() (const item_type& lhs, const item_type& rhs) const {
        return (lhs->id() == rhs->id());
    }
};
template <> struct hash<std::unique_ptr<redisgraph::node<picojson::value>>> {
    using argument_type = std::unique_ptr<redisgraph::node<picojson::value>>;
    using result_type = uint64_t;
    /*5. The meat of this struct is the operator() definition. We are just
    adding the numeric member values of struct coord, which is a poor
    hashing technique, but for the sake of showing how to implement it, it's
    good enough. A good hash function tries to distribute values as evenly
    over the whole value range as possible, in order to reduce the amount of
    hash collisions.*/

    result_type operator()(const argument_type& c) const {
        redisgraph::siphash hashFunction;
        const picojson::value currentData = c->data();
        auto str = currentData.serialize();
        hashFunction.set_data(str.c_str(), str.size());
        auto id = hashFunction.computeHash();
        return id;
    }
};

} // namespace std
#endif