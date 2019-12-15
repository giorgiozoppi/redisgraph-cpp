#ifndef REDISGRAPH_CPP_JSON_NODE_H
#define REDISGRAPH_CPP_JSON_NODE_H
#include <memory.h>
#include <cstdint>
#include <string>
#include <picojson/picojson.h>
#include <redisgraph/siphash.hpp>
#include <redisgraph/node.hpp>
#include <optional>

namespace redisgraph
{
	template <> std::unique_ptr<node<picojson::value>> make_unique_node(const std::string& label,
		const std::string& alias,
		const picojson::value& data,
		size_t data_size)
	{
		redisgraph::siphash hashFunction;
		auto str = data.serialize();
		hashFunction.set_data(str.c_str(), str.size());
		auto id = hashFunction.computeHash();
		return std::make_unique<node<picojson::value>>(id, label, alias, data);
	};
	std::optional<picojson::value> make_node_data(const std::string& json)
	{
		picojson::value node_data;
		std::string err = picojson::parse(node_data, json);
		if (!err.empty()) {
			return std::nullopt;
		}
		return node_data;		
	}
}
#endif