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