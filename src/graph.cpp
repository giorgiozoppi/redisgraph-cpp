/**
 * @file   graph.cpp
 * @author Josep Sabater (pep.sabater@gmail.com)
 * @date   September, 2019
 * @version 1.0.0
 * @ingroup redisgraphcpp
 * @brief A graph class representing the real data contained in redisgraph
 *
 */
#include <boost/asio.hpp>
#include <bredis/bredis.hpp>
#include <redisgraph/graph.hpp>

namespace redis = bredis;
namespace asio = boost::asio;
namespace sys = boost::system;


namespace redisgraph {

    graph::graph(const std::string& name, connection_context_t context): name_{name}, num_nodes_{0}, context_{context} 
	{
		init_connection(context_);
	}

	int graph::num_nodes() const { return  num_nodes_; }
	node graph::add_node(const std::string& name, const std::string& json)
	{
		node tmp{name, json};
		graph_.insert(std::make_pair<node, std::vector<edge>(node, std::vector<edge>()));
		num_nodes_++;
	}
	void graph::init_connection(const redisgraph::connection_context& context)
	{
		auto ip_address = boost::asio::ip::address::from_string(context.redis_host);
    	boost::asio::ip::tcp::endpoint end_point(ip_address, context.redis_port);
    	socket_t socket(io, end_point.protocol());
    	socket.connect(end_point);

	}
	edge graph::add_edges(const std::string& relation, const node& source, const node& dest)
	{
		edge tmp(relation, source.id, dest.id);
		auto pos = graph_.find(source);
		if (pos == graph_.end())
		{
			throw std::invalid_argument("Cannot add an edge without a node");
		}
		pos->second.push_back(tmp);	
		return tmp;
	}
	std::future<redisgraph::result_set> graph::query_async(const std::string& query)
	{

	}
	#ifdef CPP20
	std::generator<row_item> graph::query_async_generator(const std::string& query)
	{
	}
	#endif
	std::future<int> graph::commit_async()
	{
	}
}

