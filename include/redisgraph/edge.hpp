/**
 * @file   edge.hpp
 * @author Giorgio Zoppi (giorgio.zoppi@gmail.com)
 * @date   11/01/2019
 * @version 1.0.0
 * @ingroup redisgraphcpp
 * @brief A edge class representing the edges in a graph
 */
#ifndef REDIS_GRAPH_EDGE_H
#define REDIS_GRAPH_EDGE_H
#include <cstdint>
#include <node.hpp>
#include <boost/algorithms>

namespace redisgraph {
	
/**
 * Class that models an edge between nodes.
 */
class edge {
public:
  /**
   *  Constructor
   *  @param relation  relation that the edge express
   *  @param source    node source
   *  @param destination node destination
   */
  edge(const std::string &relation, const redisgraph::node &source,
       const redisgraph::node &dest) 
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
	static uint64_t make_id()
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
