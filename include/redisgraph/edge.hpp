/**
 * @file   edge.hpp
 * @author Giorgio Zoppi (giorgio@apache.org)
 * @date   11/01/2019
 * @version 1.0.0
 * @ingroup redisgraphcpp
 * @brief A edge class representing the edges in a graph
 */
#ifndef REDIS_GRAPH_EDGE_H
#define REDIS_GRAPH_EDGE_H
#include <cstdint>
#include <node.hpp>
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
       const redisgraph::node &dest) {
    relationShip_ = boost::algorithm::to_upper_copy(relation);
    source_node_ = source.id();
    dest_node_ = dest.id();
  }
  /*
   * Returns the source node index
   */
  uint64_t source() const { return source_node_; }
  /*
   * Returns the destination node index
   */
  uint64_t dest() const { return dest_node_; }
  /** Returns the relation ship between source and destination*/
  std::string relationship() const { return relationShip_; }

private:
  std::string relationShip_;
  uint64_t source_node_;
  uint64_t dest_node_;
};
} // namespace redisgraph
#endif
