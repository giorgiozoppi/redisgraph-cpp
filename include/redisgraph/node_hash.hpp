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
#ifndef REDIS_NODE_HASH_H
#define REDIS_NODE_HASH_H
#include <node.hpp>
#include <boost/uuid/uuid.hpp>
#include <siphash.hpp>

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
    // sip_hash
      redisgraph::siphash hash;
      unsigned char uuid_data[16];
      // fill uuid_data
      boost::uuids::uuid u = c->id();
      memcpy(&uuid_data,&u,16);
      hash()((void*)&uuid_data, 16);
      return hash.computeHash();
  }
};
} // namespace std
#endif