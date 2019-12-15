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

#ifndef REDISGRAPH_CPP_GRAPH_H_
#define REDISGRAPH_CPP_GRAPH_H_
#include <string>
#include <future>
#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>
#include <thread>
#include <vector>
#include <algorithm>

#include "connection_context.hpp"
#include "edge.hpp"
#include "node.hpp" 
#include "node_hash.hpp"
#include "result_view.hpp"
#include "redis_executor.hpp"

namespace redisgraph {



	/*
	* Create a graph interface with nodes that contains data of type T
	*/
	template <class T> class graph
	{
	public:
		typedef typename std::unique_ptr<node<T>> unique_node;
		typedef typename std::hash<std::unique_ptr<redisgraph::node<T>>> node_hash;
		typedef typename std::equal_to<std::unique_ptr<redisgraph::node<T>>> node_equalto;
		typedef typename std::unordered_map <unique_node, std::vector<edge<T>>, node_hash, node_equalto> adj_matrix;
		//
		/*
		* Constructor
		*/
		graph() = default;

		/**
		* Constructor
		* @param name  Name of the Graph
		* @param context Redis Configuration
		*/

		explicit graph(const std::string& name, const redisgraph::connection_context& context) : name_(name),
			context_(context),
			executor_(context)
		{
			adj_matrix map;
			graph_ = std::make_unique<adj_matrix>(std::move(map));
			started_ = false;
		}


		/**
		* Copy constructor. The graph is not copyable.
		* @param graph Graph to be copied.
		*/
		graph(const redisgraph::graph<T>& graph) = delete;
		/**
		* Copy assignment operator. The graph is not copyable
		* @param Graph to be assigned.
		*/
		const redisgraph::graph<T>& operator=(const redisgraph::graph<T>& graph) = delete;

		/**
		 * Move semantics
		 **/
		explicit graph(graph&& g)
		{
			graph_ = std::move(g.graph_);
			context_ = std::move(g.context_);
			name_ = std::move(g.name_);
			executor_ = std::move(g.executor_);

		}
		~graph() {
			if (started_)
			{
				started_ = executor_.shutdown();
			}
		}
		/**
		* Get the name of the graph
		*/
		std::string name() const { return name_; }
		/**
		* Get the number of nodes
		*/
		size_t num_nodes() const { return graph_->size(); }

		/**
		* Get the number of threads that will handle the graph
		*/
		uint16_t concurrency() const noexcept
		{
			return context_.n_threads();

		}
		/**
		* Add a new edge with a relation from the source node to the destination node
		*/
		std::optional<edge<T>> add_edge(const std::string& relation, const node<T>& source, const node<T>& dest, const std::string& properties = "") noexcept
		{
			// find in the node relation if from source there is a node to destination.
			auto node_ptr = std::make_unique<node<T>>(source);
			auto currentNode = graph_->find(node_ptr);
			bool existEdge = false;
			if (currentNode != graph_->end())
			{
				// found.
				for (const auto& e : currentNode->second)
				{
					if (find_direct_connection(source, dest, e))
					{
						existEdge = true;
					}
				}
				if (!existEdge)
				{
					// we can add
					auto currentEdge = edge<T>{ relation, source, dest, properties };
					currentNode->second.push_back(currentEdge);
					return currentEdge;
				}
			}
			// in all cases return edge
			return std::nullopt;
		}
		/**
		 * Add a node and label to the node.
		 * @return A copy of the added node if added with success.
		 */
		std::optional<node<T>> add_node(const std::string& name, const std::string& alias, T data, const size_t dataSize = 0) noexcept
		{
			auto node_ptr = redisgraph::make_unique_node(name, alias, data, dataSize);
			auto node_iterator = graph_->find(node_ptr);
			if (node_iterator == graph_->end())
			{
				node<T> current_node(node_ptr->id(), name, alias, data);
				graph_->insert(std::make_pair(std::move(node_ptr), std::vector<edge<T>>()));
				return current_node;
			}
			return std::nullopt;
		}
		/*
		*  Remove a node
		*  @return A copy of the removed node
		*/
		std::optional<node<T>> remove_node(const node<T>& current_node)
		{
			auto node_ptr = std::make_unique<node<T>>(current_node);
			auto node_iterator = graph_->find(node_ptr);
			if (node_iterator != graph_->end())
			{
				graph_->erase(node_iterator);
				return node<T>(node_ptr->id(),
					node_ptr->label(),
					node_ptr->alias(),
					node_ptr->data());
			}
			return std::nullopt;
		}
		/**
		*  Get the edges from a given node
		*/
		std::vector<edge<T>> get_edges(const node<T>& source)
		{
			auto node_ptr = std::make_unique<node<T>>(source);
			auto currentNode = graph_->find(node_ptr);
			return currentNode->second;

		}

		/*
		* startup the connection pool to redis
		*/
		void start()
		{
			started_ = executor_.start();
		}
		/**
		 *  shutdown the connection pool to redis
		 */
		void shutdown()
		{
			started_ = executor_.shutdown();
		}
		/**
		 * Query asynchronously to redis graph
		 * @param query OpenCypher Query
		 */

		void query_async(const std::string& query, std::promise<redisgraph::result_view>&& result_promise)
		{
			executor.send_message(query, std::move(result_promise));
		}

		/**
		 * Commit the current graph structure in memory to redis graph creating a graph
		 * @throw When there are connection problems
		 * @return Return a future to the result (true when it has succeded)
		 */
		bool commit()
		{
			if (graph_->size() == 0)
			{
				return false;
			}
			std::vector<redisgraph::edge<T>> edges;
			std::ostringstream query_message;
			query_message << "CREATE ";
			for (const auto& n : *graph_)
			{
				query_message << n.first->str();
				query_message << ",";
				for (const auto& e : n.second)
				{
					edges.emplace_back(e);
				}
			}
			query_message << ",";
			auto num_edges = edges.size();
			// todo sort and remove dups.
			auto count = 0;
			for (const auto& edge : edges)
			{
				query_message << edge.str();
				if (count < num_edges - 1)
				{
					query_message << ",";
				}
				count++;
			}
			std::promise<redisgraph::result_view> view;
			std::future<redisgraph::result_view> f = view.get_future();
			executor.send_message(query_message.str(), view);
			auto resultset = view.get();
			return resultset.valid();
		}
	private:
		bool find_direct_connection(const node<T>& source, const node<T>& dest, const edge<T>& e)
		{
			auto sourceId = source.id();
			auto destinationId = dest.id();
			return (e.source() == sourceId) && (e.dest() == destinationId);
		}
		std::string name_;
		redisgraph::connection_context context_;
		redisgraph::redis_executor executor_;
		bool started_;
		std::unique_ptr<adj_matrix> graph_;
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