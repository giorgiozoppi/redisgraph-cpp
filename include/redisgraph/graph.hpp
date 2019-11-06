/**
 * @file   graph.hpp
 * @author Giorgio Zoppi <giorgio@apache.org>
 * @date   September, 2019
 * @version 1.0.0
 * @ingroup redisgraphcpp
 * @brief A graph class representing the real data contained in redis graph
 */
#ifndef REDISGRAPH_CPP_GRAPH_H_
#define REDISGRAPH_CPP_GRAPH_H_
#include <connnection_context.hpp>
#include <edge.hpp>
#include <future>
#include <node.hpp>
#include <result_set.hpp>
#include <string>
#ifdef CPP20
#include <coroutine>
#endif
#include <bredis/bredis.hpp>

namespace redisgraph {

/*
 * Create a graph interface with nodes that contains data of type T
 */
template <typename T> class graph {
public:
  /**
   * Constructor
   * @param name  Name of the Graph
   * @param context Redis Configuration
   */
  graph() = default;
  explicit graph(const std
                 : string &name, const redisgraph::connection_context &context)
      : name_(name), connection_context_(context) {}
  /**
   * Copy constructro
   */
  graph(const redisgraph::graph &graph) = delete;
  /**
   * Copy assignment operator
   */
  const redisgraph::graph &operator=(const redisgraph::graph &graph) = delete;

  /** move semantics **/
  explicit graph(graph &&g) {}
  /// Getter and setter
  /**
   * Get the name of the graph
   */
  std::string name() const { return name_; }
  /**
   * Get the number of nodes
   */
  int num_nodes() const { return num_nodes_; }

  /**
   * Add a node and label to the node.
   */
  std::shared_ptr<node<T>> add_node(const std::string &name, T data) noexcept {
    auto node = std::make_shared<node>(name, data);
    graph.insert(std::make_pair(node, std::vector()));
    return node;
  }
  /**
   * Add a new edge with a relation from the source node to the destination node
   */
  std::optional<edge> add_edge(const std::string &relation,
                               const node<T> &source,
                               const node<T> &dest) noexcept;
  { auto sourceId = source.id() auto destinationId = dest.id() }
  /*
   * startup the connection pool to redis
   */
  void start();
  /**
   *  shutdown the connection pool to redis
   */
  void shutdown();
  /**
   * Query asynchronously to redis graph
   * @param query OpenCypher Query
   */
  std::future<redisgraph::result_set> query_async(const std::string &query);
/**
 * Query asynchronously to redis graph returning a generator for iterating
 * partial results
 */
#ifdef CPP20
  std::generator<redisgraph::node>
  query_async_generator(const std::string &query);
#endif
  /**
   * Commit the current graph structure in memory to redis graph creating a
   * graph
   * @throw When there are connection problems
   * @return Return a future to the result (true when it has succeded)
   */
  std::future<bool> commit_async();

  /**
   * Commit the current graph structure in memory and flush its content
   */
  bool flush();
  // copy constructs deleted

private:
  void init_connection(const redisgraph::connection_context &context);
  std::string name_;
  int num_nodes_;
  redisgraph::connection_context context_;
  std::unordered_map<uint64_t, std::vector<edge>> adjlist_;
  std::unordered_map<uint64_t, std::shared_ptr<node>> graph_;
};
} // namespace redisgraph

#endif /* REDISGRAPH_CPP_GRAPH_H_ */