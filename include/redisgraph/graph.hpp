/**
 * @file   graph.hpp
 * @author Josep Sabater (pep.sabater@gmail.com)
 * @date   September, 2019
 * @version 1.0.0
 * @ingroup redisgraphcpp
 * @brief A graph class representing the real data contained in redisgraph
 *
 */
#ifndef REDISGRAPH_CPP_GRAPH_H_
#define REDISGRAPH_CPP_GRAPH_H_

#include <string>
#include <future>
#include <connnection_context.hpp>
#include <edge.hpp>
#include <node.hpp>
#include <result_set.hpp>
#ifdef CPP20
#include <coroutine>
#endif

namespace redisgraph {

	class graph 
	{
		private:
			std::string name_;
			int num_nodes_;
			connection_context context_;
		public:
			graph(const std::string &name, const connection_context& context) 
					: name_{name} 
					, num_nodes_{0}, context_{context} {}
			std::string name() const { return name_; }
			int num_nodes() const { return  num_nodes_; }
			node add_node(const string& name, const std::string& json);
			edge add_edge(const node& source, const node& dest);
			std::future<redisgraph::result_set> query_async(const std::string& query);
			#ifdef CPP20
			std::generator<row_item> query_async_generator(const std::string& query);
			#endif
			std::future<int> commit_async();
	};
}

#endif /* REDISGRAPH_CPP_GRAPH_H_ */