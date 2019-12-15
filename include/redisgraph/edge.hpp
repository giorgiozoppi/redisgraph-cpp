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

#ifndef REDISGRAPH_CPP_EDGE_H
#define REDISGRAPH_CPP_EDGE_H
#include <cstdint>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <picojson/picojson.h>
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
       const redisgraph::node<T> &dest, const std::string& properties = "") 
  {
    relation_ = boost::algorithm::to_upper_copy(relation);
    source_node_ = source;
    dest_node_ = dest;
	edge_id_ = make_id();
    if (!properties.empty())
    {
       picojson::parse(this->edge_properties_, properties);
    }
  }
  /**
   * Get the source node index
   */
  uint64_t source() const { return source_node_.id(); }
  /*
   * Get the destination node index
   */
  uint64_t dest() const { return dest_node_.id(); }
  /** 
  * Get the relation ship between source and destination
  */
  std::string relationship() const { return relation_; }
  /**
  * Get edge properties.
  */
  picojson::value properties() const { return edge_properties_;  }
  /**
  * Get the identifier of the edege
  */
  uint64_t id() const { return edge_id_; }

  bool operator==(const edge& other) const
  {
      return (edge_id == other.edge_id_);
  }
  /** Get str of the node in cypher*/
  std::string str() const 
  {
      std::ostringstream egdestream;
      edgestream << "(";
      edgestream << this->source_node_.alias();
      edgestream << ")";
      edgestream << "-[";
      if (!this->relationship().empty())
      {
          edgestream << ":";
          edgestream << this->relationship():
      }
      edgestream << this->edge_properties_.to_str();
      edgestream << "]->";
      edgestream << "(";
      edgestream << this->destination_node_.alias();
      edgestream << ')';
      return edgestream.str();
  }

private:
	uint64_t make_id()
	{
		static uint64_t currentId = 0;
		currentId++;
		return currentId;
	}
  std::string relation_;
  redisgraph::node<T> source_node_;
  redisgraph::node<T> dest_node_;
  uint64_t edge_id_;
  picojson::value edge_properties_;
};
} // namespace redisgraph
#endif
