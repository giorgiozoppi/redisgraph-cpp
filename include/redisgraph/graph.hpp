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
#include <string>
#include <future>
#include <node_hash.hpp>
#include <optional>
#include <connection_context.hpp>
#include <edge.hpp>
#include <node.hpp>
#include <experimental/coroutine>
#include <result_view.hpp>


namespace redisgraph {

	

	/*
	* Create a graph interface with nodes that contains data of type T
	*/
	template <class T> class graph 
	{
		public:
			typedef typename std::unique_ptr<node<T>> unique_node;
			/*
			* Constructor
			*/
			graph() = default;
			
			/**
		 	* Constructor
			* @param name  Name of the Graph
			* @param context Redis Configuration
			*/

			explicit graph(const std:string& name, const redisgraph::connection_context& context): name_(name), connection_context_(context)
			{
				std::unordered_map<unique_node, std::vector<edge>> map;
				graph_ = std::make_unique(map);
			} 
			/**
			* Copy constructor. The graph is not copyable.
			* @param graph Graph to be copied.
			*/
			graph(const redisgraph::graph& graph) = delete;
			/**
			* Copy assignment operator. The graph is not copyable
			* @param Graph to be assigned.
			*/
			const redisgraph::graph& operator=(const redisgraph::graph& graph) = delete;
			
			/** 
			 * Move semantics 
			 **/
			explicit graph(graph&& g)
			{
				graph_ = std::move(g.graph_);
				context_ = std::move(g.context_);
				name_ = std::move(g.name_);
				num_nodes_ = std::move(g.num_nodes_);
			}
			~graph() {}

			/// Getter and setter		
			/**
			* Get the name of the graph
			*/
			std::string name() const { return name_; }
			/**
			* Get the number of nodes
			*/
			int num_nodes() const { return  num_nodes_; }
			
			/**
			 * Add a node and label to the node.
			 * @return A copy of the added node if added with success.
			 */
			std::optional<node> add_node(const std::string& name, T data) noexcept
			{
				node current_node{ name, data };
				auto node = graph->find(current_node);
				if (node != node.end())
				{
					graph->insert(std::make_pair(std::make_unique<node>(current_node), std::vector()));
					return current_node;
				}
				return std::nullopt;
			}
			/*
			*  Remove a node 
			*  @return A copy of the removed node
			*/
			std::optional<node> remove_node(const std::string& name)
			{

			}
			/**
			* Add a new edge with a relation from the source node to the destination node
			*/
			std::optional<edge> add_edge(const std::string& relation, const node<T>& source, const node<T>& dest) noexcept
			{
					// find in the node relation if from source there is a node to destination.
				auto currentNode = graph_->find(source);
				bool existEdge = false;
				if (currentNode != currentNode.end())
				{
					// found.
					for (const auto& e : currentNode->second)
					{
						if (find_direct_connection(source, dest ,e))
						{
							existEdge = true;
						}
					}
					if (!existEdge)
					{
						// we can add
						auto currentEdge = edge{ relation, source.id(), dest.id() };
						currentNode->second.push_back(currentEdge);
						return currentEdge;
					}
				}
				// in all cases return edge
				return std::nullopt;
			}
			/*
			* startup the connection pool to redis 
			*/
			void start()
			{

			}
			/**
			 *  shutdown the connection pool to redis 
			 */
			void shutdown()
			{

			}
			/**
			 * Query asynchronously to redis graph
			 * @param query OpenCypher Query
			 */
			std::future<redisgraph::result_view> query_async(const std::string& query)
			{

			}

			/**
			* Query asynchronously to redis graph returning a generator for iterating partial results
			*/
			std::generator<redisgraph::node> query_async_generator(const std::string& query)
			{

			}
			/**
			 * Commit the current graph structure in memory to redis graph creating a graph 
			 * @throw When there are connection problems
			 * @return Return a future to the result (true when it has succeded)
			 */
			std::future<bool> commit_async()
			{

			}
			
			/**
			* Commit the current graph structure in memory and flush its content
			*/
			bool flush()
			{

			}			
		private:
			void init_connection(const redisgraph::connection_context& context)
			{

			}
			bool find_direct_connection(const node<T>& source, const node<T>& dest, const egde& e)
			{
				auto sourceId = source.id();
				auto destinationId = dest.id();
				return (e.source() == sourceId) && (e.dest() == destinationId)
			}
			std::string name_;
			int num_nodes_;
			redisgraph::connection_context context_;
			std::unique_ptr<std::unordered_map<std::unique_ptr<node>,std::vector<edge>>> graph_;		
			
	};
	template <typename T> graph<T> make_graph(const std::string& graph_name,
		const std::string& host = "127.0.0.1",
		const unsigned int& port = 6379,
		const unsigned int& concurrency = 4)
	{
		connection_context ctx{ host, port, concurrency };
		graph<T> g(graph_name, ctx);
		return g;
	};
}
#endif /* REDISGRAPH_CPP_GRAPH_H_ */