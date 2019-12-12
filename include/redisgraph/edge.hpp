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

#ifndef REDIS_GRAPH_EDGE_H
#define REDIS_GRAPH_EDGE_H
#include <cstdint>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "node.hpp"
#include "node_hash.hpp"

namespace redisgraph {
	
/**
 * Class that models an edge between nodes.
 */
template <class T> class edge {
public:
  /**
   *  Constructor
   *  @param relation  relation that the edge express
   *  @param source    node source
   *  @param destination node destination
   */
  edge(const std::string &relation, const redisgraph::node<T> &source,
       const redisgraph::node<T> &dest) 
  {
    relation_ = boost::algorithm::to_upper_copy(relation);
    source_node_ = source.id();
    dest_node_ = dest.id();
	edge_id_ = make_id();
  }
  /**
   * Returns the source node index
   */
  uint64_t source() const { return source_node_; }
  /*
   * Returns the destination node index
   */
  uint64_t dest() const { return dest_node_; }
  /** 
  * Returns the relation ship between source and destination
  */
  std::string relationship() const { return relation_; }
  
  /**
  * Return the identifier of the edege
  */
  uint64_t id() const { return edge_id_; }

private:
	uint64_t make_id()
	{
		static uint64_t currentId = 0;
		currentId++;
		return currentId;
	}
  std::string relation_;
  uint64_t source_node_;
  uint64_t dest_node_;
  uint64_t edge_id_;
};
} // namespace redisgraph
#endif
